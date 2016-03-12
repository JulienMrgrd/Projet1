package server;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import other.GameState;
import other.Protocole;
import other.Tuple;

public class Session {
	
	private static final int SCORE_MAX = 10;
	private Server server;
	private Plateau plateau;
	private Map<String, Joueur> mapPseudo_Joueur; // Référence tous les joueurs de la session, actif ou non
	private int nbTours;
	private int nbJoueurs;
	private Tuple<Integer, Joueur> bestJoueurAndScore;
	private boolean hasStarted;
	
	public Session(Map<String, Joueur> mapPseudo_Joueur, Server server) {
		this.server = server;
		this.mapPseudo_Joueur = mapPseudo_Joueur;
		plateau = new Plateau();
		hasStarted = false;
	}
	
	public void startSession(){
		bestJoueurAndScore = new Tuple<>(0, null);
		plateau.init();
		nbTours = 1;
		hasStarted = true;
		nextStep();
	}
	
	public void stopSession(){
		nbTours = 0; // reset
		hasStarted = false;
	}
	
	public void nextStep(){
		nbTours ++;
		List<Joueur> allActifs = getAllJoueurs();
		if(allActifs.size()<=1){
			server.sendAll(Protocole.NOT_ENOUGH_PLAYERS.title);
			return;
		}
		// TODO:
		
		
		GameState state = shouldStop();
		switch (state) {
		case CanContinue:
			nextStep();
			break;
			
		case MaxScoreReached:
			// TODO : stopSession() and prévenir allJoueurs
			break;
		
		case NotEnoughPlayers:
			// TODO : stopSession() and prévenir allJoueurs
			break;
		}
	}
	
	public GameState shouldStop(){
		if(mapPseudo_Joueur.size() <= 1) return GameState.NotEnoughPlayers;
		 
		for(Entry<String, Joueur> entry : mapPseudo_Joueur.entrySet()){
			int score = entry.getValue().getScore();
			if(bestJoueurAndScore.left < score){
				bestJoueurAndScore = new Tuple<>(score, entry.getValue());
			}
		}
		if(bestJoueurAndScore.left >= SCORE_MAX) return GameState.MaxScoreReached;
		
		return GameState.CanContinue;
	}
	
	public synchronized void addJoueur(Joueur joueur){
		if(mapPseudo_Joueur != null && !mapPseudo_Joueur.containsKey(joueur.getPseudo())){
			mapPseudo_Joueur.put(joueur.getPseudo(), joueur);
		}
	}
	
	public synchronized void removeJoueur(Joueur joueur){
		if(mapPseudo_Joueur != null && mapPseudo_Joueur.containsKey(joueur.getPseudo())){
			mapPseudo_Joueur.remove(joueur.getPseudo());
		}
	}
	
	private synchronized List<Joueur> getAllJoueurs() {
		return (List<Joueur>) mapPseudo_Joueur.values();
	}

	public boolean hasStarted() {
		return hasStarted;
	}

}
