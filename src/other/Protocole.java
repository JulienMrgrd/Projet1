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
	USERNAME_ALREADY_USED("USERNAME_ALREADY_USED"),
	UNKNOWN_CMD("UNKNOWN_CMD"),
	BAD_PARAMETERS("BAD_PARAMETERS"), 
	BYE("BYE");
	
	
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
