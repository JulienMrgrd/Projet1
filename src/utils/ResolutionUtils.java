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
	
	private static boolean hasARobotNextCase(Plateau plateau, Mur direction,Case caseRobots){
			int x=caseRobots.getX();
			int y=caseRobots.getY();
			switch (direction) {
			case D:
				return plateau.getPlat()[x+1][y].containsRobot();
			case G:
				return plateau.getPlat()[x-1][y].containsRobot();
			case H:
				return plateau.getPlat()[x][y+1].containsRobot();
			case B:
				return plateau.getPlat()[x][y-1].containsRobot();
			default:
				return false;
			}
	}

	public static boolean isGoodSolution(String mouv,Plateau plateau){
		if(solutionIsGoodFormed(mouv)){
			int[] xPositionsForReset = new int[4];
			int[] yPositionsForReset = new int[4];
			Case[] allRobots = plateau.getCasesRobots();
			for(int i=0; i<allRobots.length; i++){
				System.out.printf("(before isGoodSolution) Robot %s en %s,%s.\n", allRobots[i].getRobot().name(), allRobots[i].getX(), allRobots[i].getY());
				xPositionsForReset[i] = allRobots[i].getX();
				yPositionsForReset[i] = allRobots[i].getY();
			}
			
			Case caseCible = plateau.getCaseCible();
			
			for(int i=0;i<mouv.length();i+=2){
				String coul=mouv.substring(i, i+1);
				Couleur couleur=Couleur.getCouleurByName(coul);
				
				String direct=mouv.substring(i+1,i+2);
				Mur direction=Mur.getADirByName(direct);
				
				//recuperation de la case du plateau ou se trouve le robot
				Case caseRobot = plateau.getCaseRobotByCouleur(couleur);
				
				//Deplacement du robot jusqu'à tomber sur un mur
				while(!caseRobot.containsMurAtPosition(direction)&&!hasARobotNextCase(plateau, direction, caseRobot)){
					caseRobot = plateau.setPositionRobot(caseRobot, direction);
				}
			}
			//Recuperation de la couleur du robots voulu dans la cible
			Couleur couleurCible = caseCible.getCible();
			
			//Recuperation des coordonnees du robot devant aller à la cible
			Case robot = plateau.getCaseRobotByCouleur(couleurCible);
			int posRobotsCibleX = robot.getX();
			int posRobotsCibleY = robot.getY();
			
			if(posRobotsCibleX==caseCible.getX() && posRobotsCibleY==caseCible.getY()){
				System.out.println("Bonne solution");
				allRobots = plateau.getCasesRobots();
				for(int i=0; i<allRobots.length; i++){
					System.out.printf("(after isGoodSolution) Robot %s en %s,%s.\n", allRobots[i].getRobot().name(), allRobots[i].getX(), allRobots[i].getY());
					xPositionsForReset[i] = allRobots[i].getX();
					yPositionsForReset[i] = allRobots[i].getY();
				}
				return true;
			} else {
				System.out.println("Mauvaise solution");
				allRobots = plateau.getCasesRobots();
				for(int i=0; i<allRobots.length; i++){
					System.out.printf("(after isGoodSolution, before reset) Robot %s en %s,%s.\n", allRobots[i].getRobot().name(), allRobots[i].getX(), allRobots[i].getY());
				}
				Couleur[] allCouleurs = Couleur.values();
				for(int i=0; i<allCouleurs.length; i++){
					Case oldCaseRobotByCouleur = plateau.getCaseRobotByCouleur(allCouleurs[i]);
					Case originCase = plateau.getPlat()[xPositionsForReset[i]][yPositionsForReset[i]];
					oldCaseRobotByCouleur.removeRobot();
					plateau.setCaseRobot(oldCaseRobotByCouleur, originCase);
					originCase.addRobot(allCouleurs[i]);
				}
				allRobots = plateau.getCasesRobots();
				for(int i=0; i<allRobots.length; i++){
					System.out.printf("(after isGoodSolution, after reset) Robot %s en %s,%s.\n", allRobots[i].getRobot().name(), allRobots[i].getX(), allRobots[i].getY());
				}
			}
		}
		return false;
	}
}
