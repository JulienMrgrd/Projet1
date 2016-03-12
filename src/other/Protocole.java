package other;

public enum Protocole {
	
	// connexion
	CONNEX("CONNEX"),
	BIENVENUE("BIENVENUE"),
	CONNECTE("CONNECTE"),
	
	// deconnexion
	SORT("SORT"),
	SORTI("SORTI"),
	
	// debut partie
	SESSION("SESSION"),
	VAINQUEUR("VAINQUEUR"),
	
	// Phase reflexion
	TOUR("TOUR"),
	TROUVE("TROUVE"),
	TUASTROUVE("TUASTROUVE"),
	ILATROUVE("ILATROUVE"),
	FINREFELXION("FINREFELXION"),
	
	// Phase d'enchere
	ENCHERE("ENCHERE"),
	TUENCHERE("TUENCHERE"),
	ECHECENCHERE("ECHECENCHERE"),
	ILENCHERE("ILENCHERE"),
	FINENCHERE("FINENCHERE"),
	
	// Phase de résolution
	SOLUTION("SOLUTION"),
	SASOLUTION("SASOLUTION"),
	BONNE("BONNE"),
	MAUVAISE("MAUVAISE"),
	FINRESO("FINRESO"),
	TROPLONG("TROPLONG"),
	
	// Autres : Protocole ajouté
	USERNAME_ALREADY_USED("USERNAME_ALREADY_USED"), // Nom déjà utilisé
	UNKNOWN_CMD("UNKNOWN_CMD"), // Commande inconnue
	BAD_PARAMETERS("BAD_PARAMETERS"), 
	BYE("BYE"), 
	NOT_ENOUGH_PLAYERS("NOT_ENOUGH_PLAYERS"), // Pas assez de joueurs
	WAIT("WAIT"), // indique au client d'attendre le prochain tour/lancement de la partie
	SESSION_START_TIME("SESSION_START_TIME"), // indique au client que la partie va commencer dans X secondes
	SESSION_START_CANCEL("SESSION_START_CANCEL"),
	LE_SAVIEZ_VOUS("LE_SAVIEZ_VOUS"),
	;
	
	public String title;
	
	Protocole(String title){
		this.setTitle(title);	
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}
	
	@Override
    public String toString() {
        return title;
    }
}
