package utils;

import plateau.Case;
import plateau.Couleur;
import plateau.Mur;
import plateau.Plateau;

public class ResolutionUtils {
	
	private static boolean solutionIsGoodFormed(String mouv){
		if(mouv.length()%2!=0) return false;
		for(int i=0;i<mouv.length();i+=2){
			String couleur=mouv.substring(i, i+1);
			String deplace=mouv.substring(i+1,i+2);
			if(!Mur.isADir(deplace) || !Couleur.isACouleur(couleur)) return false; 
		}
		return true;
	}
	

	public static boolean isGoodSolution(String mouv,Plateau plateau){
		
		if(solutionIsGoodFormed(mouv)){
			
			Plateau plateauCopy=new Plateau(plateau);
			//Copier le plateau pour garder en memoire au cas ou l'utilisateur c'est tromper.
			Case caseCible = plateauCopy.getCaseCible();
			
			for(int i=0;i<mouv.length();i+=2){
				String coul=mouv.substring(i, i+1);
				Couleur couleur=Couleur.getCouleurByName(coul);
				
				String direct=mouv.substring(i+1,i+2);
				Mur direction=Mur.getADirByName(direct);
				
				//recuperation de la case du plateau ou se trouve le robot
				Case caseRobot=plateauCopy.getCaseRobotByCouleur(couleur);
				
				//Deplacement du robot jusqu'à tomber sur un mur
				while(!caseRobot.containsMurAtPosition(direction)){
					plateauCopy.setPositionRobots(caseRobot, direction);
				}
			}
			//Recuperation de la couleur du robots voulu dans la cible
			Couleur couleurCible = caseCible.getCible();
			
			//Recuperation des coordonnees du robot devant aller à la cible
			Case robot = plateauCopy.getCaseRobotByCouleur(couleurCible);
			int posRobotsCibleX = robot.getX();
			int posRobotsCibleY = robot.getY();
			
			if(posRobotsCibleX==caseCible.getX() && posRobotsCibleY==caseCible.getY()) return true;
		}
		return false;
	}
}
