package plateau;

import java.util.Random;

import utils.StringUtils;

public class Plateau {
	
	private Case[][] plat = new Case[16][16];
	private Couleur[] robots = new Couleur[4];
	private PlateauChooser chooser;
	private Case caseCible;
	private Case[] caseRobots = new Case[4];
	
	public Plateau(){}
	
	public void init() {
		for(int i=0; i<plat.length;i++){
			for(int j=0; j<plat[i].length;j++){
				plat[i][j]=new Case(i,j);
				if(i==0) plat[i][j].addMurDeBase(Mur.G);//Ajout des mur a la bordure gauche
				if(j==0) plat[i][j].addMurDeBase(Mur.B);//Ajout des mur a la bordure basse
				if(i==plat.length-1) plat[i][j].addMurDeBase(Mur.D);//Ajout des mur a la bordure droite
				if(j==plat[i].length-1) plat[i][j].addMurDeBase(Mur.H);//Ajout des mur a la bordure haute
			}
		}
		
		addMursCentre(); // AJouter tous les murs du centre
		
		robots[0] = Couleur.B;
		robots[1] = Couleur.J;
		robots[2] = Couleur.R;
		robots[3] = Couleur.V;
		
		chooser = new PlateauChooser(plat);
		chooser.applyRandomPlateau(); // ajoute murs obstacles
		updatePlateau(); // Ajoute obstacles, robots et cible
	}
	
	public void updatePlateau(){
		if(chooser==null) init();
		Case[] mursContigus = chooser.getCasesAvecDeuxMursContigus();

		int pos = new Random().nextInt(mursContigus.length);
		mursContigus[pos].addCible(Couleur.randomCouleur()); // Ajoute la cible
		caseCible = mursContigus[pos];
		
		System.out.println("Ajout des 4 robots");
		for(int i=0; i<robots.length; i++){  // Ajout des 4 robots
			caseRobots[i] = getRandomCaseForRobots();
			if((caseRobots[i].getX()==caseCible.getX() || caseRobots[i].getY()==caseCible.getY()) 
					&& !containsMursBetweenTwoAlignedCases(caseRobots[i], caseCible)){
				System.out.println("Robot "+i+" aligné avec la cible : robot=("+caseRobots[i].getX()+","
					    +caseRobots[i].getY()+") et cible=("+caseCible.getX()+","+caseCible.getY()+")");
				i--; // Si rien ne sépare le robot de la cible (coup en 1 coup), on retente de placer le robot
			} else {
				caseRobots[i].addRobot(robots[i]);
			}
		}
		System.out.println("Fin d'ajout des 4 robots");
	}
	
	private Case getRandomCaseForRobots() {
		int xRand, yRand;
		Random r = new Random();
		do {
			xRand = r.nextInt(plat.length);
			yRand = r.nextInt(plat[plat.length-1].length);
		} while( !plat[xRand][yRand].canContainRobotsOrCible() && !plat[xRand][yRand].containsCible() );
		return plat[xRand][yRand];
	}

	private boolean containsMursBetweenTwoAlignedCases(Case one, Case two){
		if(one.getX()==two.getX() && one.getY()==two.getY()) return false; // meme case
		else if(one.getX()==two.getX()){ // Si alignés en x
			int nbCasesBetween;
			if(one.getY()>two.getY()){ // si one au dessus de two
				if(one.containsMurAtPosition(Mur.B) || two.containsMurAtPosition(Mur.H)) return true;
				nbCasesBetween = one.getY()-two.getY() - 1;
			} else{
				if(one.containsMurAtPosition(Mur.H) || two.containsMurAtPosition(Mur.B)) return true;
				nbCasesBetween = two.getY()-one.getY() - 1;
			}
			
			for(int i=1; i<=nbCasesBetween; i++){
				if(one.getY()>two.getY() && plat[one.getX()][one.getY()-i].containsMurAtPosition(Mur.B)) return true;
				else if (one.getY()<two.getY() && plat[one.getX()][one.getY()+i].containsMurAtPosition(Mur.H)) return true;
			}
			
		} else if (one.getY()==two.getY()){
			int nbCasesBetween;
			if(one.getX()>two.getX()){ // si one au dessus de two
				if(one.containsMurAtPosition(Mur.G) || two.containsMurAtPosition(Mur.D)) return true;
				nbCasesBetween = one.getX()-two.getX() - 1;
			} else{
				if(one.containsMurAtPosition(Mur.D) || two.containsMurAtPosition(Mur.G)) return true;
				nbCasesBetween = two.getX()-one.getX() - 1;
			}
			
			for(int i=1; i<=nbCasesBetween; i++){
				if(one.getX()>two.getX() && plat[one.getX()-i][one.getY()].containsMurAtPosition(Mur.G)) return true;
				else if (one.getX()<two.getX() && plat[one.getX()+i][one.getY()].containsMurAtPosition(Mur.D)) return true;
			}
		}
		return false;
	}
	
	public void display(){
		System.out.print("    ");
		for(int i=0; i<plat.length; i++){
			if(i<10) System.out.print(i+"  ");
			else System.out.print(i+" ");
		}
		System.out.print("\n    ");
		for(int i=0; i<plat.length; i++){
			System.out.print("_  ");
		}
		System.out.println();
		for(int j=plat.length-1; j>=0; j--){
			for(int i=0; i<plat[j].length; i++){
				if(i==0 && j<10) System.out.print(j+"  "+plat[i][j].display());
				else if(i==0 && j>=10) System.out.print(j+" "+plat[i][j].display());
				else System.out.print(plat[i][j].display());
			}
			System.out.println();
		}
		System.out.print("    ");
		for(int i=0; i<plat.length; i++){
			if(i<10) System.out.print(i+"  ");
			else System.out.print(i+" ");
		}
	}
	
	/** Affiche "plateau" de l'énoncé (la suite des murs avec leurs positions) */
	public String plateau(){
		String res = "";
		System.out.println("\n");
		for(Case[] oneLine : plat){
			for(Case oneCase : oneLine){
				String disp = oneCase.toString();
				if(disp.isEmpty()) continue;
				else res += disp + ",";
			}
		}
		return StringUtils.deleteCommaIfExists(res);
	}
	
	/** Affiche "enigme" de l'énoncé (la suite des robots et cible avec leurs positions) */
	public String enigme(){
		String enigme = "";
		return enigme;
	}
	
	private void addMursCentre() {
		plat[6][7].addMurDeBase(Mur.D);
		plat[6][8].addMurDeBase(Mur.D);
		plat[7][9].addMurDeBase(Mur.B);
		plat[8][9].addMurDeBase(Mur.B);
		plat[9][7].addMurDeBase(Mur.G);
		plat[9][8].addMurDeBase(Mur.G);
		plat[7][7].addMurDeBase(Mur.B);
		plat[8][7].addMurDeBase(Mur.B);
		plat[7][6].addMurDeBase(Mur.H);
		plat[8][6].addMurDeBase(Mur.H);
		plat[7][7].setCanContainsRobotsOrCible(false);
		plat[8][7].setCanContainsRobotsOrCible(false);
		plat[7][8].setCanContainsRobotsOrCible(false);
		plat[8][8].setCanContainsRobotsOrCible(false);
	}
	
	public static void main(String[] args ){
		Plateau plateau = new Plateau();
		plateau.init();
		plateau.display();
		System.out.println("\n"+plateau.plateau());
	}

}
