import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

public class Server{
	
	private final static int PORT = 2016;
	private Map<String, Joueur> joueurs;
	private int nbJoueurs=0;
	
	public Server() {
		joueurs = new HashMap<String, Joueur>();
		printWelcome();
	}
	
	public void start() {
		
		ServerSocket serverSocket = null;
		try	{
			serverSocket = new ServerSocket(PORT);
			
			while (true) // attente en boucle de connexion (bloquant sur ss.accept)
			{
				Socket client = serverSocket.accept();
				new Joueur(client,this).start(); // un client se connecte, un nouveau thread client est lancé
			}

		}
		catch (Exception e) { }
		finally {
			if(serverSocket!=null && !serverSocket.isClosed()){
				try {
					serverSocket.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	} 

	static private void printWelcome() {
		System.out.println("--------");
		System.out.println("Serveur demarre sur le port : "+PORT);
		System.out.println("--------\n");
	}


	synchronized public void sendAll(String message,String sLast)
	{
		for (Entry<String, Joueur> onejoueur : joueurs.entrySet()) // parcours de la table des connectés
		{
			try {
				onejoueur.getValue().sendToJoueur(message+sLast);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	synchronized public int getNbJoueurs() {
		return nbJoueurs; // retourne le nombre de clients connectés
	}
	

	public static void main(String args[])
	{
		Server server = new Server(); // instance de la classe principale
		server.start();
	}

}