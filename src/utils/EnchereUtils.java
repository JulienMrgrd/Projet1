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
	
	public static void addIfPossibleInGoodPosition(List<Enchere> listEnch, Enchere ench){
		if(listEnch==null){
			listEnch = new ArrayList<Enchere>();
			listEnch.add(ench);
			return;
		}
		
		System.out.println("(addIfPossibleInGoodPosition) ajoute enchere de "+ench.getJoueur().toString()+","+ench.getNbCoups());
		System.out.print("Liste existante avant : (");
		for(Enchere oneEnch : listEnch){
			System.out.print("("+oneEnch.getJoueur()+","+oneEnch.getNbCoups()+"),"); 
		}
		System.out.println(")");
		
		Enchere enchExistante = getEnchereByJoueur(listEnch, ench.getJoueur());
		if(enchExistante!=null){
			if(enchExistante.getNbCoups()<=ench.getNbCoups()) return;
			else enchExistante.setNbCoups(ench.getNbCoups());
		} else {
			listEnch.add(ench);
		}
		Collections.sort(listEnch);
		System.out.print("Liste existante après : (");
		for(Enchere oneEnch : listEnch){
			System.out.print("("+oneEnch.getJoueur()+","+oneEnch.getNbCoups()+"),"); 
		}
		System.out.println(")");
	}

}
