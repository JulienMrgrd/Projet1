package protocole;

public enum Protocole {
	
	// connexion
	CONNEXION,
	BIENVENUE,
	CONNECTE,
	
	// deconnexion
	SORT,
	DECONNEXION,
	
	// debut partie
	SESSION,
	VAINQUEUR,
	
	// Phase reflexion
	TOUR,
	TROUVE,
	TUASTROUVE,
	ILATROUVE,
	FINREFLEXION,
	
	// Phase d'enchere
	ENCHERE,
	VALIDATION,
	ECHEC,
	NOUVELLEENCHERE,
	FINENCHERE,
	
	// Phase de résolution
	SOLUTION,
	SASOLUTION,
	BONNE,
	MAUVAISE,
	FINRESO,
	TROPLONG,
	
	// Autres : Protocole ajouté
	USERNAME_ALREADY_USED, // Nom déjà utilisé (à la connexion)
	UNKNOWN_CMD, // Commande inconnue
	BAD_PARAMETERS, 
	BYE, // (à la déconnexion avec SORT)
	WAIT, // indique au client d'attendre le prochain tour/lancement de la partie
	START_TIME_SESSION, // indique au client que la partie va commencer dans X secondes
	START_CANCEL_SESSION, // indique au client que la partie est annulée, faute de 2 joueurs
	LE_SAVIEZ_VOUS,
	CHAT,
	PING,
	BEFORE_BAN,  // (BEFORE_BAN/message) indique à user qu'il lui reste X insultes avant d'être banni
	BANNI		// (BANNI/User/messageServer) indique à tous que user a été banni
	;
	
}
