package test;
import java.net.*;
import java.util.Scanner;
import java.io.*;

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
		while ( !hasBeenStopped && (line = console.nextLine())!="bye"){  
			streamOut.println(line);
			streamOut.flush();
		}
		System.out.println("bye bye.");
	}

	private void runThreadEcouteSocket() {
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				if(socket!=null){
					
					BufferedReader reader;
					try {
						reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
						while(true){
							
							String str;
							try {
								str = reader.readLine();
								System.out.println("Le server répond : "+str);
								if(str==null){
									System.out.println("Serveur HS");
									stop();
									return;
								} else if(str.equals(Protocole.BYE.title)){
									stop();
									return;
								}
								
							} catch (IOException e) {
								System.out.println("Problème d'écoute du server");
							}
							
						}
					} catch (IOException e1) {
						System.out.println("Problème de création du reader");
					}
					
				}
			}
		}).start();
	}

	public void stop(){
		hasBeenStopped = true;
		try {  
			if (console   != null)  console.close();
			if (streamOut != null)  streamOut.close();
			if (socket    != null)  socket.close();
		} catch(IOException ioe){  
			System.out.println("Error closing ...");
		}
	}

	public static void main(String args[]){
		new Client("localhost", Server.PORT);
	}
}