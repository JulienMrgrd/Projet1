package other;

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
		System.out.println("Le random est : "+valeur);
		while(allAlreadyDisplayed.contains(valeur)){
			valeur = r.nextInt(nbPhrases);
		}
		allAlreadyDisplayed.add(valeur);
		return allPhrases()[valeur];
	}
	
	private String[] allPhrases(){
		return new String[]{
			"L'excellent Emmanuel Chailloux a intégré l'UPMC en 2006",
			"Le campus de Jussieu s’étend sur environ 13 hectares",
			"Les travaux de désamientage de Jussieu ont commencés en 1996, et se sont terminés en 2011",
			"L'incroyable Romain Demangeon s'occupe de 6 UEs différentes à l'UPMC"	 
		};
	}
}
