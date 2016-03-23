package utils;

import plateau.Couleur;

public class StringUtils {
	
	private StringUtils(){}
	
	public static String deleteCommaIfExists(String str){
		if(str != null && str.endsWith(",")) return str.substring(0, str.length()-1);
		else return str;
	}
	
	public static String transformInto(String str, Couleur coul){
		return transformInto(str, AnsiColors.getAnsiByCouleur(coul));
	}

	public static String transformInto(String str, AnsiColors color){
		return color.code+str+AnsiColors.ANSI_RESET.code;
	}
	
	public static String underline(String str){
		return "\u001B[4m"+str+AnsiColors.ANSI_RESET;
	}

}
