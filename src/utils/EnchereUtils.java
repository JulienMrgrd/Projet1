package utils;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import server.Enchere;
import server.Joueur;

public abstract class EnchereUtils {
	
	public static Enchere getEnchereByJoueur(List<Enchere> listEnch, Joueur j){
		for(Enchere ench : listEnch){
			if(ench.getJoueur()==j) return ench;
		}
		return null;
	}
	
	public static Enchere getEnchereByNbCoups(List<Enchere> listEnch, int nbCoups){
		for(Enchere ench : listEnch){
			if(ench.getNbCoups()==nbCoups) return ench;
		}
		return null;
	}
	
	/** Ajoute l'enchère si elle n'existe pas, si aucune autre n'existe avec ce nombre de coups ou moins, ou si
	 * la précédente enchère du Joueur est plus grande en nombre de coups. 
	 * @return Le pseudo du joueur ayant déjà effectué une enchère du même type (ou son pseudo si supérieure), 
	 * null si OK */
	public static String addIfPossibleInGoodPosition(List<Enchere> listEnch, Enchere ench){
		if(listEnch==null){
			listEnch = new ArrayList<Enchere>();
			listEnch.add(ench);
			return null;
		}
		
		Enchere enchExistanteDuJoueur = getEnchereByJoueur(listEnch, ench.getJoueur());
		Enchere enchExistanteAvecCeNbCoups = getEnchereByNbCoups(listEnch, ench.getNbCoups());
		if(enchExistanteAvecCeNbCoups!=null) return enchExistanteAvecCeNbCoups.getJoueur().getPseudo();
		
		if(enchExistanteDuJoueur!=null){
			if(enchExistanteDuJoueur.getNbCoups()<=ench.getNbCoups()) return ench.getJoueur().getPseudo();
			else enchExistanteDuJoueur.setNbCoups(ench.getNbCoups());
		
		} else {
			listEnch.add(ench);
		}
		Collections.sort(listEnch);
		return null;
	}

}
