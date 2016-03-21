package test;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

import other.Protocole;
import server.Server;

public class Client
{  
	private Socket socket = null;
	private Scanner  console = null;
	private PrintWriter streamOut = null;
	private boolean hasBeenStopped = false;

	public Client(String serverName, int serverPort){  
		System.out.println("Establishing connection. Please wait ...");
		try {  
			socket = new Socket(serverName, serverPort);
			System.out.println("Connected: " + socket);
			
			runThreadEcouteSocket();
			// Out
			console   = new Scanner(System.in);
			streamOut = new PrintWriter(socket.getOutputStream());
		}
		catch(UnknownHostException uhe) {  
			System.out.println("Host unknown: " + uhe.getMessage());
		}
		catch(IOException ioe) {  
			System.out.println("Unexpected exception: " + ioe.getMessage());
		}
		String line = "";
		
		System.out.println("Vous pouvez parler au serveur.");
		try{
			while ( !hasBeenStopped && (line = console.nextLine())!="exit."){  
				streamOut.println(line);
				streamOut.flush();
			}
		} catch (Exception exc){ }
		finally{
			if (console != null) console.close();
		}
		System.out.println("bye bye.");
	}

	private void runThreadEcouteSocket() {
		new Thread(new Runnable() {
			@Override
			public void run() {
				if(socket!=null){
					
					BufferedReader reader = null;
					try {
						reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
						while(true){
							
							String str;
							try {
								str = reader.readLine();
								if(str==null){
									serverHS();
									return;
								} else if(str.isEmpty()){
									System.out.println("message vide du server");
								} else {
									System.out.println(str);
								}
								
								if(str.equals(Protocole.BYE.name())){
									stop();
									return;
								}
								
							} catch (IOException e) {
								serverHS();
								return;
							}
							
						}
					} catch (IOException e1) {
						System.out.println("Problème de création du reader");
					} finally {
						try {
							if(reader != null) reader.close();
						} catch (IOException e) {
							e.printStackTrace();
						}
					}
				}
			}
		}).start();
	}
	
	public void stop(){
		hasBeenStopped = true;
		try {  
			PrintWriter pw = new PrintWriter(System.out);
			pw.println("exit by press ENTER");
			pw.flush();
			pw.close();
			if (streamOut != null)  streamOut.close();
			if (socket    != null)  socket.close();
		} catch(IOException ioe){  
			System.out.println("Error closing ...");
		}
	}
	
	private void serverHS() {
		System.out.println("Serveur HS");
		stop();
	}

	public static void main(String args[]){
//		new Client("JulienM-HP", Server.PORT);
		new Client("localhost", Server.PORT);
	}
}