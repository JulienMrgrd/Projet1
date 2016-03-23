package server;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import other.Protocole;
import other.ProtocoleCreator;

public class Server{
	
	public static final int PORT = 2016;
	private Map<String, Joueur> mapJoueurs;
	private int nbJoueurs=0;
	private Session session;
	private Object sync;
	private int secondsBeforeStartSession;
	
	public Server() throws UnknownHostException {
		mapJoueurs = new HashMap<String, Joueur>();
		session = new Session(mapJoueurs, this);
		sync = new Object(); // Objet pour notification
		
		System.out.println("--------");
		System.out.println("Le serveur ("+InetAddress.getLocalHost().getHostAddress()+") demarre sur le port : "+PORT);
		System.out.println("--------\n");
	}
	
	public void start() {
		
		ServerSocket serverSocket = null;
		try	{
			serverSocket = new ServerSocket(PORT);
			
			startSessionIfPossible();
			while (true){ // attente en boucle de connexion (bloquant sur ss.accept)
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
	
	/** Démarre la session (ou en relance une) si il y a au moins 2 joueurs. */
	private void startSessionIfPossible(){
		new Thread(new Runnable() {
			@Override
			public void run() {
				while(true){
					System.out.println("NbJoueurs="+nbJoueurs+" et sessionHasStarted="+session.hasStarted());
					synchronized (sync) {
						while(nbJoueurs<2){ // Préconditions pour lancer une session
							try {
								sync.wait();
							} catch (InterruptedException e) {
								System.out.println("(startSessionIfNeeded) wait impossible");
							}
						}
					}
					
					secondsBeforeStartSession = Session.SECONDS_BEFORE_START;
					sendAll(ProtocoleCreator.create(Protocole.START_TIME_SESSION, Integer.toString(Session.SECONDS_BEFORE_START)));
					System.out.println("Début de partie dans "+Session.SECONDS_BEFORE_START+" secondes");
					try { // TODO: décommenter avant soumission !
//						LeSaviezVousGenerator gen = new LeSaviezVousGenerator();
						do{
							Thread.sleep(Session.SECONDS_FOR_DISPLAY_SAVIEZVOUS*1000);
//							sendAll(ProtocoleCreator.create(Protocole.LE_SAVIEZ_VOUS,gen.get()));
							secondsBeforeStartSession -= Session.SECONDS_FOR_DISPLAY_SAVIEZVOUS;
						} while(secondsBeforeStartSession>0);
					} catch (InterruptedException e) {
						System.out.println("(startSessionIfPossible) attente impossible");
					}
					secondsBeforeStartSession=0;
					
					if(nbJoueurs<2){ // Un ou plusieurs joueurs ont quittés
						sendAll(ProtocoleCreator.create(Protocole.START_CANCEL_SESSION));
						continue;
					}
					
					session.startSession(); // Bloquant
					
				}
			}
		}).start();
	}

	public synchronized void sendAll(String message) {
		List<Joueur> aSuppr = new ArrayList<>(mapJoueurs.size());
		for (Entry<String, Joueur> onejoueur : mapJoueurs.entrySet()){ // parcours de la table des connectés
			try {
				onejoueur.getValue().sendToJoueur(message);
			} catch (IOException e) {
				aSuppr.add(onejoueur.getValue());
			}
		}
		for(Joueur unASuppr : aSuppr){
			removeJoueur(unASuppr);
		}
	}
	
	public void sendToThem(String message, List<Joueur> joueurs) {
		if(joueurs == null) return;
		List<Joueur> aSuppr = new ArrayList<>(joueurs.size());
		synchronized (joueurs) {
			for (Joueur onejoueur : joueurs){ // parcours tous les joueurs
				try {
					onejoueur.sendToJoueur(message);
				} catch (IOException e) {
					aSuppr.add(onejoueur);
				}
			}
		}
		for(Joueur unASuppr : aSuppr){
			removeJoueur(unASuppr);
		}
	}
	
	public synchronized void sendToThemButThis(String message, List<Joueur> joueurs, Joueur toNotInclude) {
		if(joueurs == null) return;
		List<Joueur> aSuppr = new ArrayList<>(joueurs.size());
		synchronized (joueurs) {
			for (Joueur onejoueur : joueurs){ // parcours tous les joueurs
				if(onejoueur==toNotInclude) continue;
				try {
					onejoueur.sendToJoueur(message);
				} catch (IOException e) {
					aSuppr.add(onejoueur);
				}
			}
		}
		for(Joueur unASuppr : aSuppr){
			removeJoueur(unASuppr);
		}
	}
	
	public synchronized void sendAllButThis(String message, Joueur toNotInclude) {
		List<Joueur> aSuppr = new ArrayList<>(mapJoueurs.size());
		for (Entry<String, Joueur> onejoueur : mapJoueurs.entrySet()){ // parcours de la table des connectés
			try {
				if( !onejoueur.getKey().equals(toNotInclude.getPseudo()) ){
					onejoueur.getValue().sendToJoueur(message);
				}
			} catch (IOException e) {
				aSuppr.add(onejoueur.getValue());
			}
		}
		for(Joueur unASuppr : aSuppr){
			removeJoueur(unASuppr);
		}
	}
	
	synchronized public boolean addJoueur(Joueur joueur) {
		if(containsJoueur(joueur.getPseudo())) return false;
		mapJoueurs.put(joueur.getPseudo(), joueur);
		nbJoueurs++;
		sendAllButThis(ProtocoleCreator.create(Protocole.BIENVENUE,joueur.getPseudo()), joueur);
		try {
			joueur.sendToJoueur(ProtocoleCreator.create(Protocole.CONNECTE));
		} catch (IOException e1) {
			removeJoueur(joueur);
		}
		if(session.hasStarted()){
			try {
				joueur.sendToJoueur(ProtocoleCreator.create(Protocole.WAIT));
			} catch (IOException e) { /* Un joueur s'est connecté et immédiatement déconnecté*/ }
		} else {
			try {
				if(secondsBeforeStartSession!=0)
					joueur.sendToJoueur(ProtocoleCreator.create(Protocole.START_TIME_SESSION, Integer.toString(secondsBeforeStartSession)));
			} catch (IOException e) { removeJoueur(joueur); }
		}
		synchronized (sync) {
			sync.notify(); // Notify le server qu'un joueur a été ajouté (voir startSessionIfNeeded)
		}
		return true;
	}
	
	private boolean containsJoueur(String pseudo) {
		for(String key : mapJoueurs.keySet()){
			if(key.equalsIgnoreCase(pseudo)) return true;
		}
		return false;
	}

	synchronized public boolean removeJoueur(Joueur joueur) {
		if( !this.mapJoueurs.containsKey(joueur.getPseudo()) ) return false;
		mapJoueurs.remove(joueur.getPseudo());
		session.removeJoueur(joueur);
		nbJoueurs--;
		sendAll(ProtocoleCreator.create(Protocole.SORTI, joueur.getPseudo()));
		return true;
	}

	synchronized public int getNbJoueurs() {
		return nbJoueurs;
	}
	
	public Session getSession(){
		return session;
	}
	

	public static void main(String args[]) throws UnknownHostException
	{
		Server server = new Server(); // instance de la classe principale
		server.start();
	}


}