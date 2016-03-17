package server;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import other.GameState;
import other.Protocole;
import other.ProtocoleCreator;
import plateau.Plateau;
import utils.StringUtils;
import utils.Tuple;

public class Session {

	public static final int SCORE_MAX = 10;
	public static final int SECONDS_BEFORE_START = 30;
	public static final int SECONDS_FOR_DISPLAY_SAVIEZVOUS = 5;
	private final int STEP_REFLEXION=1, STEP_ENCHERES=2, STEP_RESOLUTION=3;

	private Server server;
	private Plateau plateau;
	private Map<String, Joueur> mapPseudo_Joueur; // Référence tous les joueurs de la session, actif ou non
	private List<Joueur> allActifs;
	
	private int nbTours;
	private Tuple<Integer, Joueur> bestJoueurAndScore;
	private boolean hasStarted;

	public Session(Map<String, Joueur> mapPseudo_Joueur, Server server) {
		this.server = server;
		this.mapPseudo_Joueur = mapPseudo_Joueur; // Passage par référence, pas par copie
		plateau = new Plateau();
		allActifs=new ArrayList<>();
		hasStarted = false;
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
		nbTours ++;
		allActifs = getAllJoueurs();

		System.out.println("(nextStep) nbJoueurs="+getNbActifs());
		if(getNbActifs()<=1){
			sendVainqueur(allActifs.get(0));
			stopSession();
			return;
		}

		// On préviens du début de la manche
		synchronized (allActifs) { // TODO: retirer
			for(Joueur joueur : allActifs){ 
				System.out.println("(Session nextStep) forAll joueurs : "+joueur.getPseudo());
			}
		}
		try{
			server.sendToThem(ProtocoleCreator.create(Protocole.SESSION, plateau.toString()), allActifs);
		} catch (Exception e){
			System.out.println(e);
		}
		if(getNbActifs()<=1){
			sendVainqueur(allActifs.get(0));
			stopSession();
			return; // On arrete la partie puisque tout le monde a quitté
		}

		for(int i=1; i<=3; i++){
			switch (i) {
			case STEP_REFLEXION:
				startReflexion(allActifs);
				break;
			case STEP_ENCHERES:
				startEncheres(allActifs);
				break;
			case STEP_RESOLUTION:
				startResolution(allActifs);
				break;
			}
		}

		System.out.println("Traitement du tour n°"+nbTours);
		try {
			Thread.sleep(5000);
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
			if(allActifs.size()==1){
				System.out.println("Vainqueur : "+allActifs.get(0));
				sendVainqueur(allActifs.get(0));
			}
			stopSession();
			break;
		}
	}

	private void sendVainqueur(Joueur joueur) {
		server.sendAll(ProtocoleCreator.create(Protocole.VAINQUEUR, joueur.getPseudo()));
	}

	private void startResolution(List<Joueur> allActifs) {
		// TODO Auto-generated method stub
		System.out.println("startResolution");
	}

	private void startEncheres(List<Joueur> allActifs) {
		// TODO Auto-generated method stub
		System.out.println("startEncheres");
	}

	private void startReflexion(List<Joueur> allActifs) {
		// TODO Auto-generated method stub
		System.out.println("startReflexion");
	}

	private GameState shouldStop(){
		if(getNbActifs() <= 1) return GameState.NotEnoughPlayers;

		for(Joueur joueur : allActifs){
			if(bestJoueurAndScore.left < joueur.getScore()){
				bestJoueurAndScore = new Tuple<>(joueur.getScore(), joueur);
			}
		}
		if(bestJoueurAndScore.left >= SCORE_MAX) return GameState.MaxScoreReached;

		return GameState.CanContinue;
	}
	
	public void removeJoueur(Joueur joueur){
		if(allActifs.contains(joueur)) allActifs.remove(joueur);
	}
	
	private synchronized List<Joueur> getAllJoueurs() {
		return new ArrayList<>(mapPseudo_Joueur.values());
	}

	public boolean hasStarted() {
		return hasStarted;
	}

	public int getNbJoueurs(){ return mapPseudo_Joueur.size(); }
	
	public int getNbActifs(){ return allActifs.size(); }
	
	/**
	 * Ping les joueurs, et met à jour map/allActifs si certains sont déconnectés
	 */
	public void updateActifs(){
		synchronized (allActifs) {
			server.sendToThem("", allActifs);
		}
	}
	
	/** Affiche "bilan" de l'énoncé (le tour + les scores des joueurs) */
	public String bilan(){
		String bilan = Integer.toString(nbTours);
		if(allActifs != null){
			List<Joueur> toRemove = new ArrayList<>(2); // Peu de joueurs se déconnectent
			for(Joueur joueur : allActifs){
				if(joueur.estEnVie()) bilan += "("+ joueur.getPseudo()+","+joueur.getScore()+"),";
				else toRemove.add(joueur);
			}
			for(Joueur joueur : toRemove){
				server.removeJoueur(joueur);
			}
		}
		return StringUtils.deleteCommaIfExists(bilan);
	}

}
