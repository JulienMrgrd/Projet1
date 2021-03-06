package server;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import plateau.Plateau;
import protocole.Protocole;
import protocole.ProtocoleCreator;
import utils.EnchereUtils;
import utils.GameState;
import utils.ResolutionUtils;
import utils.StringUtils;

public class Session {

	//Variable globales pour pouvoir modifier rapidement les paramétres important d'une partie à un seul endroit
	public static final int SCORE_MAX = 10;
	public static final int SECONDS_BEFORE_START = 30;
	public static final int SECONDS_FOR_DISPLAY_SAVIEZVOUS = 5;
	public static final int SECONDS_REFLEXION = 300;
	public static final int SECONDS_ENCHERES = 40;
	public static final int SECONDS_RESOLUTION = 60;
	public static final int TEMPS_RAFRAICHISSEMENT = 15; 
	private final int STEP_REFLEXION=1, STEP_ENCHERES=2, STEP_RESOLUTION=3;
	
	private Server server;
	private Plateau plateau;
	private Map<String, Joueur> mapPseudo_Joueur; // Référence tous les joueurs de la session, actif ou non
	private List<Joueur> allPlaying;
	
	private int nbTours;
	private boolean hasStarted;
	
	// Reflexion
	private boolean isInReflexion;
	private Joueur vainqueurReflexion;
	private Integer nbCoupsVainqueurReflexion;
	
	// Enchères
	private boolean isInEnchere;
	private List<Enchere> encheres;
	
	// Resolution
	private boolean isInResolution;
	private int indexEnch;
	private Joueur actif;
	private String deplacement;
	
	/**
	 * Constructeur de la création d'une session
	 * @param mapPseudo_Joueur map contenant tous les joueurs avec comme clé leur pseudo
	 * @param server auquel est rattaché la session
	 */
	public Session(Map<String, Joueur> mapPseudo_Joueur, Server server) {
		this.server = server;
		this.mapPseudo_Joueur = mapPseudo_Joueur; // Passage par référence, pas par copie
		plateau = new Plateau();
		allPlaying=new ArrayList<>();
		hasStarted = false;
		isInReflexion = false;
		isInEnchere = false;
	}

	/**
	 * Méthode lançant la session
	 */
	public void startSession(){
		System.out.println("Nouvelle partie");
		plateau.init();
		nbTours = 0;
		hasStarted = true;
		nextStep();
	}

	/**
	 * Methode stoppant la session
	 */
	public void stopSession(){
		System.out.println("Fin de la partie");
		hasStarted = false;
	}
	
	/**
	 * Methode lançant un nouveau tour complet, jusqu'à ce qu'un joueur atteigne le score maximum ou que le nombre de joueur est inférieur à 2
	 */
	public void nextStep(){
		resetAttributes();
		nbTours ++;
		if(nbTours>1) plateau.updatePlateau();
		
		allPlaying = getAllJoueurs();

		if(getNbActifs()<=1){
			sendVainqueur(allPlaying.get(0));
			stopSession();
			return;
		}

		// On préviens du début de la manche
		System.out.println("\nDébut du tour "+nbTours);
		System.out.print("Tous les joueurs : ");
		synchronized (allPlaying) {
			for(int i=0; i<allPlaying.size(); i++){ 
				if(i==allPlaying.size()-1) System.out.println(allPlaying.get(i).getPseudo());
				else System.out.print(allPlaying.get(i).getPseudo()+", ");
			}
		}

		if(nbTours==1) sendToAllPlaying(ProtocoleCreator.create(Protocole.SESSION, plateau.plateau()));
		else {
			for(Joueur j : allPlaying){
				if(j.isWaiting()){
					try {
						//Envoie du protocole SESSION/plateau aux joueurs qui était en phase d'attente
						j.sendToJoueur(ProtocoleCreator.create(Protocole.SESSION, plateau.plateau()));
						j.setIsWaiting(false);
					} catch (IOException e) {
						server.removeJoueur(j);
					}
				}
			}
		}
		
		if(getNbActifs()<=1){
			if(getNbActifs()==1) sendVainqueur(allPlaying.get(0));
			stopSession();
			return; // On arrete la partie puisque tout le monde a quitté
		}

		for(int i=1; i<=3; i++){
			switch (i) {
			case STEP_REFLEXION:
				isInReflexion = true;
				startReflexion();
				isInReflexion = false;
				break;
			case STEP_ENCHERES:
				isInEnchere = true;
				startEncheres();
				isInEnchere = false;
				break;
			case STEP_RESOLUTION:
				isInResolution = true;
				startResolution();
				isInResolution = false;
				break;
			}
			
			updateActifs();
			if(allPlaying.size()<2 || (actif!=null && actif.getScore()>=SCORE_MAX)){
				break;
			}
		}
		
		System.out.println("Fin du tour n°"+nbTours);
		server.sleep(1000);
		
		updateActifs();
		GameState state = shouldStop();
		switch (state) {
		case CanContinue:
			System.out.println("case : Ca repars pour un tour");
			nextStep(); // Ca repars pour un tour !
			break;

			//Si le score maximum est atteint arréter la session et signaler qui est le vainqueur
		case MaxScoreReached:
			System.out.println("case : Max score reached");
			sendVainqueur(actif);
			stopSession();
			break;

			//Si le nombre de joueur est inférieur à 2 arreter la partie et envoyer au joueur restant qu'il est le vainqueur
		case NotEnoughPlayers:
			System.out.println("case : not enough");
			if(allPlaying.size()==1){
				sendVainqueur(allPlaying.get(0));
			}
			stopSession();
			break;
		}
	}

	/**
	 * Methode lançant la phase de reflexion
	 */
	private void startReflexion() {
		System.out.println("startReflexion ("+SECONDS_REFLEXION+" sec)");
		server.sleep(1000); // Pour éviter que le client reçoive SESSION et TOUR en même temps.
		
		sendToAllPlaying(ProtocoleCreator.create(Protocole.TOUR, plateau.enigme(), bilan()));
		
		int temps = (SECONDS_REFLEXION+3)*1000; // Ajout de 3sec (lenteur réseau, ...)
		synchronized (this) {
			try {
				while(temps>0 && vainqueurReflexion==null){
					this.wait(TEMPS_RAFRAICHISSEMENT*1000); 
					temps -= TEMPS_RAFRAICHISSEMENT*1000;
					
					if(getNbActifs()>=2) updateActifs(); // Evite de ping l'unique joueur restant
					else break;
				}
				server.sleep(1000); // Pour éviter que le client PING et FINREFLEXION en même temps.
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		if(vainqueurReflexion!=null && nbCoupsVainqueurReflexion!=null) {
			System.out.println(vainqueurReflexion.getPseudo() + " a une solution en "+nbCoupsVainqueurReflexion+" coups !");
		} else {
			sendToAllPlaying(ProtocoleCreator.create(Protocole.FINREFLEXION));
			System.out.println("Temps terminé, aucune solution");
		}
		
	}
	
	/**
	 * Methode lançant la phase d'enchere
	 */
	private void startEncheres() {
		System.out.println("startEncheres ("+SECONDS_ENCHERES+" sec)");
		if(nbCoupsVainqueurReflexion!=null && vainqueurReflexion!=null) addEncheres(new Enchere(vainqueurReflexion, nbCoupsVainqueurReflexion));
		server.sleep((SECONDS_ENCHERES+3)*1000); // Ajout de 3sec (lenteur réseau, ...)
		
		System.out.print("Fin des enchères : ");
		for(Enchere oneEnch : encheres){
			System.out.print("("+oneEnch.getJoueur()+","+oneEnch.getNbCoups()+"),"); 
		}
		System.out.println(")");
		
		if(encheres.size()>0){
			sendToAllPlaying(ProtocoleCreator.create(Protocole.FINENCHERE, 
					encheres.get(0).getJoueur().getPseudo(), Integer.toString(encheres.get(0).getNbCoups())));
		} else if (vainqueurReflexion!=null && nbCoupsVainqueurReflexion!=null){
			sendToAllPlaying(ProtocoleCreator.create(Protocole.FINENCHERE, vainqueurReflexion.getPseudo(), nbCoupsVainqueurReflexion.toString()));
		} else {
			sendToAllPlaying(ProtocoleCreator.create(Protocole.FINENCHERE));
		}
	}
	
	/**
	 * Methode lançant la phase de resolution
	 */
	private void startResolution() {
		int nbCoups = 0;
		if(encheres != null && encheres.size()>0){
			actif = encheres.get(indexEnch).getJoueur();
		}
		
		if(actif==null){
			sendToAllPlaying(ProtocoleCreator.create(Protocole.FINRESO));
			return;
		}
		nbCoups = encheres.get(indexEnch).getNbCoups();
		
		System.out.println("startResolution ("+SECONDS_RESOLUTION+" sec) avec "+actif);
		
		int temps = (SECONDS_RESOLUTION+3)*1000; // Ajout de 3sec (lenteur réseau, ...)
		synchronized (this) {
			try {
				while(temps>0 && deplacement==null){
					this.wait(TEMPS_RAFRAICHISSEMENT*1000); // rafraichissement toutes les secondes pour éviter de boucler en continu inutilement
					temps -= TEMPS_RAFRAICHISSEMENT*1000;	
					
					if(getNbActifs()>=2) updateActifs(); //Vérification du nombre de joueur restant
					else break;
				}
				server.sleep(1000); // Pour éviter que le client PING et BONNE,MAUVAISE,FINRESO en même temps.
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		if(deplacement!=null && (deplacement.length()/2)<=nbCoups){ //Vérification de la solution proposer
			System.out.println("Deplacement correctement formé");
			if(ResolutionUtils.isGoodSolution(deplacement, plateau)){
				System.out.println(actif.getPseudo()+" gagne 1 point ! (nb Points = "+(actif.getScore()+1)+")");
				actif.addOnePoint();
				sendToAllPlaying(ProtocoleCreator.create(Protocole.BONNE));
			} else {
				System.out.println("Mauvaise solution");
				if(indexEnch+1>=encheres.size()){
					sendToAllPlaying(ProtocoleCreator.create(Protocole.MAUVAISE));
					server.sleep(1000);
					sendToAllPlaying(ProtocoleCreator.create(Protocole.FINRESO));
					return;
				} else {
					String joueurSuivant = encheres.get(indexEnch+1).getJoueur().getPseudo();
					sendToAllPlaying(ProtocoleCreator.create(Protocole.MAUVAISE, joueurSuivant));
					indexEnch++; // enchere suivante
					startResolution(); // Refais résolution avec enchère suivante
				}
			}
		} else {
			if(deplacement!=null) System.out.printf("Erreur deplacement (%s), (length=%s)",deplacement, deplacement.length());
			else System.out.println("Pas de déplacements");
			if(indexEnch+1>=encheres.size()){
				System.out.println("Plus assez de joueurs, on passe au tour suivant.");
				sendToAllPlaying(ProtocoleCreator.create(Protocole.FINRESO));
				return;
			} else {
				System.out.println("TROPLONG ou MAUVAIS, joueur suivant.");
				String joueurSuivant = encheres.get(indexEnch+1).getJoueur().getPseudo();
				if(deplacement==null) sendToAllPlaying(ProtocoleCreator.create(Protocole.TROPLONG, joueurSuivant));
				else sendToAllPlaying(ProtocoleCreator.create(Protocole.MAUVAISE, joueurSuivant));
				indexEnch++; // enchere suivante
				startResolution(); // Refais résolution avec enchère suivante
			}
		}
	}
	
	/**
	 * Methode permettant de réinitialiser tous les attributs à chaque step
	 */
	private void resetAttributes(){
		isInReflexion=false;
		vainqueurReflexion=null;
		nbCoupsVainqueurReflexion=null;
		
		isInEnchere=false;
		encheres = new ArrayList<>();
		
		isInResolution=false;
		actif=null;
		indexEnch = 0;
		deplacement=null;
	}

	/**
	 * Methode permettant de vérifier si on peux continuer ou non
	 * @return "CanContinue" si on peux continuer sinon "NotEnoughPlayers" ou "MaxScoreReached" si on ne peux plus continuer
	 */
	private GameState shouldStop(){
		if(getNbActifs() <= 1) return GameState.NotEnoughPlayers;

		if(actif != null && actif.getScore()>=SCORE_MAX){
			return GameState.MaxScoreReached;
		}

		return GameState.CanContinue;
	}
	
	/**
	 * Envoie à tous les joueurs le pseudo du vainqueur
	 * @param joueur vainqueur de la partie
	 */
	private void sendVainqueur(Joueur joueur) {
		System.out.println("Vainqueur : "+joueur.getPseudo());
		sendToAllPlaying(ProtocoleCreator.create(Protocole.VAINQUEUR, bilan()));
	}
	
	/** Affiche "bilan" de l'énoncé (le tour + les scores des joueurs) */
	public String bilan(){
		String bilan = Integer.toString(nbTours);
		if(allPlaying != null){
			List<Joueur> toRemove = new ArrayList<>(2); // Peu de joueurs se déconnectent
			for(Joueur joueur : allPlaying){
				if(joueur.estEnVie()) bilan += "("+ joueur.getPseudo()+","+joueur.getScore()+")";
				else toRemove.add(joueur);
			}
			for(Joueur joueur : toRemove){
				server.removeJoueur(joueur);
			}
		}
		return StringUtils.deleteCommaIfExists(bilan);
	}
	
	/** Ping les joueurs, et met à jour map/allActifs si certains sont déconnectés */
	public void updateActifs(){
		synchronized (allPlaying) {
			sendToAllPlaying(Protocole.PING.name());
		}
	}
	
	/**
	 * Methode permettant d'envoyer un message à tous les joueurs entrain de jouer
	 * @param message à envoyer aux joueurs entrain de jouer
	 */
	public void sendToAllPlaying(String message) {
		server.sendToThem(message, allPlaying);
	}
	
	//==========  GETTERS // SETTERS =============//
	
	public synchronized void removeJoueur(Joueur joueur){
		if(allPlaying.contains(joueur)) allPlaying.remove(joueur);
	}
	
	public boolean isPlaying(Joueur joueur){
		return allPlaying.contains(joueur);
	}
	
	public List<Joueur> getAllPlaying(){
		return allPlaying;
	}
	
	private synchronized List<Joueur> getAllJoueurs() {
		return new ArrayList<>(mapPseudo_Joueur.values());
	}

	public boolean hasStarted() {
		return hasStarted;
	}

	public int getNbJoueurs(){ return mapPseudo_Joueur.size(); }
	
	public synchronized int getNbActifs(){ return allPlaying.size(); }

	public synchronized Joueur getVainqueurReflexion() {
		return vainqueurReflexion;
	}

	public synchronized void setVainqueurReflexion(Joueur vainqueurReflexion) {
		this.vainqueurReflexion = vainqueurReflexion;
	}

	public synchronized int getNbCoupsVainqueurReflexion() {
		return nbCoupsVainqueurReflexion;
	}

	public synchronized void setNbCoupsVainqueurReflexion(int nbCoupsVainqueurReflexion) {
		this.nbCoupsVainqueurReflexion = nbCoupsVainqueurReflexion;
	}
	
	public boolean isJoueurActifResolution(Joueur j){ return actif==j; } 

	/** Ajoute l'enchère si elle n'existe pas, si aucune autre n'existe avec ce nombre de coups ou moins, ou si
	 * la précédente enchère du Joueur est > en nombre de coups. 
	 * @return Le pseudo du joueur ayant déjà effectué une enchère <= en nombre de coups, 
	 * null si OK */
	public synchronized String addEncheres(Enchere enchere) {
		return EnchereUtils.addIfPossibleInGoodPosition(encheres, enchere); 
	}
	
	public void addDeplacement(String deplacement){ this.deplacement = deplacement; }

	public boolean isInReflexion(){ return isInReflexion; }
	public boolean isInEnchere(){ return isInEnchere; }
	public boolean isInResolution(){ return isInResolution; }
}
