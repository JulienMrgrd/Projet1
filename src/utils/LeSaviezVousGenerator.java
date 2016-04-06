package utils;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class LeSaviezVousGenerator {
	
	List<Integer> allAlreadyDisplayed;
	int nbPhrases;
	
	public LeSaviezVousGenerator(){
		nbPhrases = allPhrases().length;
		allAlreadyDisplayed = new ArrayList<>(nbPhrases);
	}
	
	public String get(){
		if(nbPhrases == allAlreadyDisplayed.size()) allAlreadyDisplayed.clear();
		
		Random r = new Random();
		int valeur = r.nextInt(nbPhrases);
		while(allAlreadyDisplayed.contains(valeur)){
			valeur = r.nextInt(nbPhrases);
		}
		allAlreadyDisplayed.add(valeur);
		return allPhrases()[valeur];
	}
	
	private String[] allPhrases(){
		return new String[]{
			"Emmanuel Chailloux a integre l'UPMC en 2006",
			"Romain Demangeon s'occupe de 6 UEs differentes à l'UPMC",
			"Philippe ESLING donne en tout 128 heures de cours aux L1/L2 de l'UPMC",
			"Le campus de Jussieu s’etend sur environ 13 hectares",
			"Les travaux de désamientage de Jussieu ont commences en 1996, et se sont termines en 2011",
			"L'UPMC avait ete cree le 1er janvier 1971",
			"Le LIP6 est compose de 219 personnels permanents",
			"6 chercheurs de l'INRIA font partis de l'equipe du LIP6",
			"20 chercheurs du CNRS font partis de l'equipe du LIP6",
			"Jean-Claude Bajard est le directeur du LIP6",
			"Jean Chambaz a ete reelu president de l'UPMC pour la deuxieme fois, le 7 mars 2016",
			"L'UPMC est la 2e universite française dans le classement de Shanghai dans la categorie informatique",
			"Le site lip6.fr est traduit en 10 langues",
			"191 doctorants sont actuellement au LIP6",
			"Le LIP6 est divise en 6 departements",
			"La page facebook de l'UPMC est aimee par plus de 39 000 personnes",
			"Le compte Twitter de l'UPMC est suivi par plus de 14 000 followers"
			
			
			
		};
	}
}
