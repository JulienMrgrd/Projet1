package utils;

import plateau.Couleur;

public enum AnsiColors {
	
	ANSI_RESET("\u001B[0m"),
	ANSI_BLACK("\u001B[30m"),
	ANSI_RED("\u001B[31m"),
	ANSI_GREEN("\u001B[32m"),
	ANSI_YELLOW("\u001B[33m"),
	ANSI_BLUE("\u001B[34m"),
	ANSI_PURPLE("\u001B[35m"),
	ANSI_CYAN("\u001B[36m"),
	ANSI_WHITE("\u001B[37m");
	
	public String code;
	
	AnsiColors(String code){
		this.code = code;	
	}

	@Override
    public String toString() {
        return code;
    }
	
	public static AnsiColors getAnsiByCouleur(Couleur coul){
		if(coul==Couleur.B) return AnsiColors.ANSI_BLUE;
		else if(coul==Couleur.R) return AnsiColors.ANSI_RED;
		else if(coul==Couleur.V) return AnsiColors.ANSI_GREEN;
		else return AnsiColors.ANSI_YELLOW;
	}
	
}
