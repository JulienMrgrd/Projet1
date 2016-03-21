package server;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import other.GameState;
import other.Protocole;
import other.ProtocoleCreator;
import plateau.Plateau;
import utils.EnchereUtils;
import utils.StringUtils;
import utils.Tuple;

public class Session {

	public static final int SCORE_MAX = 10;
	public static final int SECONDS_BEFORE_START = 10;
	public static final int SECONDS_FOR_DISPLAY_SAVIEZVOUS = 5;
	public static final int SECONDS_REFLEXION = 10;
	public static final int SECONDS_ENCHERES = 30;
	private final int STEP_REFLEXION=1, STEP_ENCHERES=2, STEP_RESOLUTION=3;

	private Server server;
	private Plateau plateau;
	private Map<String, Joueur> mapPseudo_Joueur; // Référence tous les joueurs de la session, actif ou non
	private List<Joueur> allPlaying;
	
	private int nbTours;
	private Tuple<Integer, Joueur> bestJoueurAndScore;
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
	
	public Session(Map<String, Joueur> mapPseudo_Joueur, Server server) {
		this.server = server;
		this.mapPseudo_Joueur = mapPseudo_Joueur; // Passage par référence, pas par copie
		plateau = new Plateau();
		allPlaying=new ArrayList<>();
		hasStarted = false;
		isInReflexion = false;
		isInEnchere = false;
	}

	public void startSession(){
		System.out.println("Nouvelle partie");
		bestJoueurAndScore = new Tuple<>(0, null);
		plateau.init();
		nbTours = 0;
		hasStarted = true;
		nextStep();
	}

	public void stopSession(){
		System.out.println("Fin de la partie");
		hasStarted = false;
	}

	public void nextStep(){
		boolean hasAWinner = false;
		
		nbTours ++;
		allPlaying = getAllJoueurs();

		System.out.println("(nextStep) nbJoueurs="+getNbActifs());
		if(getNbActifs()<=1){
			sendVainqueur(allPlaying.get(0));
			stopSession();
			return;
		}

		// On préviens du début de la manche
		synchronized (allPlaying) { // TODO: retirer
			for(Joueur joueur : allPlaying){ 
				System.out.println("(Session nextStep) forAll joueurs : "+joueur.getPseudo());
			}
		}
		try{
			server.sendToThem(ProtocoleCreator.create(Protocole.SESSION, plateau.plateau()), allPlaying);
		} catch (Exception e){
			System.out.println(e);
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
				isInEnchere = true;
				startResolution();
				isInEnchere = false;
				break;
			}
			
			updateActifs();
			if(allPlaying.size()<2){
				System.out.println("Fin d'une phase ("+i+"), allPlaying.size="+allPlaying.size());
				break;
			}
		}
		
		System.out.println("Traitement du tour n°"+nbTours);
		try {
			Thread.sleep(2000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		System.out.println("Vérification de fin du tour n°"+nbTours);
		
		updateActifs();
		GameState state = shouldStop();
		switch (state) {
		case CanContinue:
			System.out.println("case : Ca repars pour un tour");
			nextStep(); // Ca repars pour un tour !
			break;

		case MaxScoreReached:
			System.out.println("case : Max score reached");
			server.sendAll(ProtocoleCreator.create(Protocole.VAINQUEUR,bestJoueurAndScore.right.getPseudo()));
			stopSession();
			break;

		case NotEnoughPlayers:
			System.out.println("case : not enough");
			if(allPlaying.size()==1){
				System.out.println("Vainqueur : "+allPlaying.get(0));
				sendVainqueur(allPlaying.get(0));
			}
			stopSession();
			break;
		}
	}

	private boolean startReflexion() {
		System.out.println("startReflexion");
		server.sendToThem(ProtocoleCreator.create(Protocole.TOUR, plateau.enigme(), bilan()), allPlaying);
		
		System.out.println("Start waiting for solution "+SECONDS_REFLEXION+" sec"); // TODO: 5 min
		synchronized (this) {
			try {
				this.wait(SECONDS_REFLEXION*1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		if(vainqueurReflexion!=null && nbCoupsVainqueurReflexion!=null) {
			System.out.println(vainqueurReflexion.getPseudo() + "a une solution en "+nbCoupsVainqueurReflexion+" coups !");
		} else {
			System.out.println("Temps terminé, aucune solution");
		}
		return true;
	}
	
	private boolean startEncheres() {
		System.out.println("startEncheres durant "+SECONDS_ENCHERES+" sec");
		encheres = new ArrayList<>();
		try {
			Thread.sleep(SECONDS_ENCHERES*1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		System.out.println("fin enchères");
		System.out.print("Liste enchères finale : (");
		for(Enchere oneEnch : encheres){
			System.out.print("("+oneEnch.getJoueur()+","+oneEnch.getNbCoups()+"),"); 
		}
		System.out.println(")");
		return true;
	}
	
	private boolean startResolution() {
		return true;
		// TODO:
	}

	private GameState shouldStop(){
		if(getNbActifs() <= 1) return GameState.NotEnoughPlayers;

		for(Joueur joueur : allPlaying){
			if(bestJoueurAndScore.left < joueur.getScore()){
				bestJoueurAndScore = new Tuple<>(joueur.getScore(), joueur);
			}
		}
		if(bestJoueurAndScore.left >= SCORE_MAX) return GameState.MaxScoreReached;

		return GameState.CanContinue;
	}
	
	private void sendVainqueur(Joueur joueur) {
		System.out.println("Vainqueur : "+joueur.getPseudo());
		server.sendAll(ProtocoleCreator.create(Protocole.VAINQUEUR, joueur.getPseudo()));
	}
	
	/** Ping les joueurs, et met à jour map/allActifs si certains sont déconnectés */
	public void updateActifs(){
		synchronized (allPlaying) {
			server.sendToThem("", allPlaying);
		}
	}
	
	/** Affiche "bilan" de l'énoncé (le tour + les scores des joueurs) */
	public String bilan(){
		String bilan = Integer.toString(nbTours);
		if(allPlaying != null){
			List<Joueur> toRemove = new ArrayList<>(2); // Peu de joueurs se déconnectent
			for(Joueur joueur : allPlaying){
				if(joueur.estEnVie()) bilan += "("+ joueur.getPseudo()+","+joueur.getScore()+"),";
				else toRemove.add(joueur);
			}
			for(Joueur joueur : toRemove){
				server.removeJoueur(joueur);
			}
		}
		return StringUtils.deleteCommaIfExists(bilan);
	}
	
	//==========  GETTERS // SETTERS =============//
	
	public void removeJoueur(Joueur joueur){
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
	
	public int getNbActifs(){ return allPlaying.size(); }

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

	/** Ajoute l'enchère si elle n'existe pas, si aucune autre n'existe avec ce nombre de coups, ou si
	 * la précédente enchère du Joueur est plus grande en nombre de coups. 
	 * 
	 * @return Le pseudo du joueur ayant déjà effectué une enchère du même type (ou son pseudo si supérieure), 
	 * null si OK
	 * */
	public synchronized String addEncheres(Enchere enchere) { 
		return EnchereUtils.addIfPossibleInGoodPosition(encheres, enchere); 
	}

	public boolean isInReflexion(){ return isInReflexion; }
	public boolean isInEnchere(){ return isInEnchere; }
	public boolean isInResolution(){ return isInResolution; }
}
