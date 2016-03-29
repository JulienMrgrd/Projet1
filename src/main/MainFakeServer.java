package main;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;
import java.util.Scanner;

public class MainFakeServer {

	private static Scanner  console = null;
	private static boolean hasBeenStopped = false;
	public static final int PORT = 2016;
	private static List<Socket> all =  new ArrayList<>();

	public static void main(String args[]) throws UnknownHostException, SocketException
	{
		System.out.println("Démarrage du serveur ...\n");
		Enumeration<NetworkInterface> nets = NetworkInterface.getNetworkInterfaces();
		for (NetworkInterface netint : Collections.list(nets)){
			displayInterfaceInformation(netint);
		}
		try	{
			
			final ServerSocket serverSocket = new ServerSocket(PORT);
		
			System.out.println("--------");
			System.out.println("Le faux serveur demarre sur le port : "+PORT);
			System.out.println("--------\n");

			console   = new Scanner(System.in); // On lance l'écoute du clavier dans un thread (SERVER -> CLIENT)
			new Thread(new Runnable() {

				@Override
				public void run() {
					try{
						String line = "";
						synchronized (all) {
							while(all.isEmpty()){
								all.wait();
							}
						}
						while ( !hasBeenStopped && !(line = console.nextLine()).equals("exit.")){  
							sendAll(line);
						}
					} catch (Exception exc){ }
					finally{
						if (console != null) console.close();
					}
					System.out.println("Fin d'envoi des messages");
					try {
						serverSocket.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}).start();

			while (!hasBeenStopped){ // acceptation des nouveaux clients
				System.out.println("Attente de nouveaux clients");
				final Socket client = serverSocket.accept();
				System.out.println("Ah, il y a un nouveau client !...");
				new Thread(new Runnable() {
					
					@Override
					public void run() {
						BufferedReader reader = null;
						try {
							reader = new BufferedReader(new InputStreamReader(client.getInputStream()));
						} catch (IOException e1) {
							e1.printStackTrace();
							return;
						}
						while(true){
							String str;
							try {
								str = reader.readLine();
								if(str==null){
									return;
								} else {
									System.out.println(str);
								}
								
							} catch (IOException e) {
								return;
							}
						}
					}
				}).start();
				synchronized (all) {
					all.add(client);
					all.notify();
				}
			}

		}
		catch (Exception e) { }
		
		System.out.println("bye bye.");
	}

	private synchronized static void sendAll(String line) throws IOException {
		if(all.isEmpty()) return;
		for(Socket sock : all){
			PrintWriter streamOut = new PrintWriter(sock.getOutputStream());
			streamOut.println(line);
			streamOut.flush();
		}
	}

	static void displayInterfaceInformation(NetworkInterface netint) throws SocketException {
		Enumeration<InetAddress> inetAddresses = netint.getInetAddresses();
		for (InetAddress inetAddress : Collections.list(inetAddresses)) {
			if(inetAddress instanceof Inet4Address) System.out.println("IP="+inetAddress+" : "+netint.getDisplayName());
		}
	}

}
