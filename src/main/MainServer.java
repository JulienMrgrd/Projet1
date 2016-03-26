package main;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Collections;
import java.util.Enumeration;

import server.Server;

public class MainServer {
	
	public static final int PORT = 2016;

	public static void main(String args[]) throws UnknownHostException, SocketException
	{
		Enumeration<NetworkInterface> nets = NetworkInterface.getNetworkInterfaces();
		for (NetworkInterface netint : Collections.list(nets)){
	        displayInterfaceInformation(netint);
		}
		Server server = new Server(PORT); // instance de la classe principale
		server.start();
	}
	
	static void displayInterfaceInformation(NetworkInterface netint) throws SocketException {
	    Enumeration<InetAddress> inetAddresses = netint.getInetAddresses();
	    for (InetAddress inetAddress : Collections.list(inetAddresses)) {
	        if(inetAddress instanceof Inet4Address) System.out.println("IP="+inetAddress+" : "+netint.getDisplayName());
	    }
	 }
	
}
