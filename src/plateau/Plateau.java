package plateau;

import java.util.Random;
import java.util.Scanner;

import utils.PlateauUtils;
import utils.ResolutionUtils;
import utils.StringUtils;

public class Plateau {
	
	private Case[][] plat = new Case[16][16];
	private Couleur[] robots = new Couleur[4];
	private PlateauChooser chooser;
	private Case caseCible;
	private Case[] caseRobots = new Case[4];
	
	public Plateau(){}
	
	/**
	 * Constructeur par copie partielle
	 * @param plateau à copier
	 */
	public Plateau(Plateau plateau){
		this.plat=new Case[plateau.plat.length][plateau.plat[0].length];
		for(int i=0;i<this.plat.length;i++){
			for(int j=0;j<this.plat[0].length;j++){
				this.plat[i][j]=new Case(plateau.plat[i][j]);
			}
		}
		
		this.robots=plateau.robots;
		this.chooser=plateau.chooser;
		this.caseCible=plateau.caseCible;
		
		this.caseRobots=new Case[plateau.caseRobots.length];
		
		for(int i=0;i<this.caseRobots.length;i++) this.caseRobots[i]=new Case(plateau.caseRobots[i]);
	}
	
	public Plateau(Case[][] plat, Couleur[] robots) {
		this.plat = plat;
		this.robots = robots;
	}

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
		
		robots[0] = Couleur.R; // Conserver cette ordre pour respecter le protocole.
		robots[1] = Couleur.B;
		robots[2] = Couleur.J;
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
		
		for(int i=0; i<robots.length; i++){  // Ajout des 4 robots
			caseRobots[i] = getRandomCaseForRobots();
			if((caseRobots[i].getX()==caseCible.getX() || caseRobots[i].getY()==caseCible.getY()) 
					&& !containsMursBetweenTwoAlignedCases(caseRobots[i], caseCible)){
				i--; // Si rien ne sépare le robot de la cible (coup en 1 coup), on retente de placer le robot
			} else {
				caseRobots[i].addRobot(robots[i]);
			}
		}
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
	
	/** Affiche "plateau" de l'énoncé (la suite des murs avec leurs positions) */
	public String plateau(){
		String res = "";
		System.out.println("\n");
		for(Case[] oneLine : plat){
			for(Case oneCase : oneLine){
				String disp = oneCase.toString();
				if(disp.isEmpty()) continue;
				else res += disp;
			}
		}
		return StringUtils.deleteCommaIfExists(res);
	}
	
	/** Affiche "enigme" de l'énoncé (la suite des robots et cible avec leurs positions) */
	public String enigme(){
		String enigme = "(";
		
		if(caseRobots!=null){
			for(Case c : caseRobots){
				enigme += c.getX()+","+c.getY()+",";
			}
		}
		
		if(caseCible!=null){
			enigme += caseCible.getX()+","+caseCible.getY()+","+caseCible.getCible();
		}
		
		enigme+=")";
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

	
	public Case getCaseCible() {
		return caseCible;
	}

	public Case[][] getPlat() {
		return plat;
	}
	
	public Case getCaseRobotByCouleur(Couleur coul) {
		for(Case robotCouleur : caseRobots){
			if(robotCouleur.getRobot()==coul) return robotCouleur;
		}
		return null;
	}

	public void setPositionRobots(Case caseRobots, Mur direction) {
		int x=caseRobots.getX();
		int y=caseRobots.getY();
		switch (direction) {
		case D:
			plat[x][y].removeRobot();
			caseRobots.removeAllMur();
			caseRobots.setX(caseRobots.getX()+1);
			plat[x+1][y].addRobot(caseRobots.getRobot());	
			caseRobots.addAllMurFromCase(plat[x+1][y]);
			break;
		case G:
			plat[x][y].removeRobot();
			caseRobots.removeAllMur();
			caseRobots.setX(caseRobots.getX()-1);
			plat[x-1][y].addRobot(caseRobots.getRobot());
			caseRobots.addAllMurFromCase(plat[x-1][y]);
			break;
		case H:
			plat[x][y].removeRobot();
			caseRobots.removeAllMur();
			caseRobots.setY(caseRobots.getY()+1);
			plat[x][y+1].addRobot(caseRobots.getRobot());
			caseRobots.addAllMurFromCase(plat[x][y+1]);
			break;
		case B:
			plat[x][y].removeRobot();
			caseRobots.removeAllMur();
			caseRobots.setY(caseRobots.getY()-1);
			plat[x][y-1].addRobot(caseRobots.getRobot());
			caseRobots.addAllMurFromCase(plat[x][y-1]);
			break;
		default:
		}
	}
	
	public static void main(String[] args ){
		Plateau plateau = new Plateau();
		plateau.init();
		System.out.println("\nPlateau avant ...");
		PlateauUtils.display(plateau.getPlat());
		System.out.println("\n"+plateau.plateau());
		System.out.println(plateau.enigme());
		Scanner sc = new Scanner(System.in);
		System.out.print("Votre solution : ");
		String str = sc.nextLine();
		
		boolean solution = ResolutionUtils.isGoodSolution(str,plateau);
		System.out.println("Votre solution est : "+solution+"\n");
		
		System.out.println("\nPlateau après ...");
		PlateauUtils.display(plateau.getPlat());
		
		while(!solution){
			System.out.print("\n\nRedonnez une solution : ");
			str = sc.nextLine();
			solution = ResolutionUtils.isGoodSolution(str,plateau);
			System.out.println("Votre solution est : "+solution+"\n\n");
		}
		
		sc.close();
	}


}
