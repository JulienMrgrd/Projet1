package server;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

public class Server{
	
	public static final int PORT = 2016;
	private Map<String, Joueur> mapJoueurs;
	private int nbJoueurs=0;
	private Session session;
	
	public Server() {
		mapJoueurs = new HashMap<String, Joueur>();
		session = new Session(mapJoueurs, this);
		
		System.out.println("--------");
		System.out.println("Serveur demarre sur le port : "+PORT);
		System.out.println("--------\n");
	}
	
	public void start() {
		
		ServerSocket serverSocket = null;
		try	{
			serverSocket = new ServerSocket(PORT);
			
			while (true) // attente en boucle de connexion (bloquant sur ss.accept)
			{
				Socket client = serverSocket.accept();
				System.out.println("Ah, il y a un nouveau joueur !...");
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
	
	public void mapChange(){
		if( !session.hasStarted() && nbJoueurs>1 ){
			
		}
	}


	synchronized public void sendAll(String message) {
		for (Entry<String, Joueur> onejoueur : mapJoueurs.entrySet()){ // parcours de la table des connectés
			try {
				onejoueur.getValue().sendToJoueur(message);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	synchronized public void sendAllButThis(String message, Joueur toNotInclude) {
		for (Entry<String, Joueur> onejoueur : mapJoueurs.entrySet()){ // parcours de la table des connectés
			try {
				if( !onejoueur.getKey().equals(toNotInclude.getPseudo()) ){
					onejoueur.getValue().sendToJoueur(message);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	synchronized public boolean addJoueur(Joueur joueur) {
		if(this.mapJoueurs.containsKey(joueur.getPseudo())) return false;
		mapJoueurs.put(joueur.getPseudo(), joueur);
		nbJoueurs++;
		mapChange();
		return true;
	}
	
	synchronized public boolean removeJoueur(Joueur joueur) {
		if( !this.mapJoueurs.containsKey(joueur.getPseudo()) ) return false;
		mapJoueurs.remove(joueur.getPseudo());
		nbJoueurs--;
		mapChange();
		return true;
	}

	synchronized public int getNbJoueurs() {
		return nbJoueurs;
	}
	

	public static void main(String args[])
	{
		Server server = new Server(); // instance de la classe principale
		server.start();
	}


}