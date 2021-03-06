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
	
	public Plateau(Case[][] plat, Couleur[] robots) {
		this.plat = plat;
		this.robots = robots;
	}

	/**
	 * Initialisation des murs cerclant le plateau, plus choix du plateau a utiliser pour la session et initialisation
	 * du tableau representant les 4 robots.
	 */
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
	
	
	/**
	 * Methode de mise a jour du plateau, ne modifiant pas les murs du plateaux mais plaçant les robots et la cible sur le plateau
	 */
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
				i--; // Si rien ne sépare le robot de la cible (coup en 1 coup), ou qu'il y a déjà un robot on retente de placer le robot
			} else {
				caseRobots[i].addRobot(robots[i]);
			}
		}
	}
	
	/**
	 * Methode permettant de récupérer une case aléatoire sur le plateau
	 * @return case où on pourra eventuellement placer un robot
	 */
	private Case getRandomCaseForRobots() {
		int xRand, yRand;
		Random r = new Random();
		do {
			xRand = r.nextInt(plat.length);
			yRand = r.nextInt(plat[plat.length-1].length);
		} while( !plat[xRand][yRand].canContainRobotsOrCible() 
				|| plat[xRand][yRand].containsCible() || plat[xRand][yRand].containsRobot() );
		return plat[xRand][yRand];
	}

	
	/**
	 * Methode permettant de savoir si 2 cases sont alignées et ne possédent aucun obbstacle entre les 2
	 * @param one Case du robot a tester
	 * @param two Case de la cible
	 * @return true si il y a un obstacle entre les deux cases, faux sinon
	 */
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
	
	/**
	 * Methode permettant d'ajouter les murs centraux du plateau
	 */
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

	/**
	 * Methode retournant la case de la cible
	 * @return la case de la cible
	 */
	public Case getCaseCible() {
		return caseCible;
	}
	
	/**
	 * Methode retournant les cases des robots
	 * @return les cases des robots
	 */
	public Case[] getCasesRobots(){
		return caseRobots;
	}

	/**
	 * Methode retournant le plateau
	 * @return le plateau
	 */
	public Case[][] getPlat() {
		return plat;
	}
	
	/**
	 * Methode retournant la case d'un robot
	 * @param coul Couleur du robot voulu
	 * @return la case du plateau où est situé le robot de Couleur "coul"
	 */
	public Case getCaseRobotByCouleur(Couleur coul) {
		for(Case robotCouleur : caseRobots){
			if(robotCouleur.getRobot()==coul) return robotCouleur;
		}
		return null;
	}
	
	/**
	 * Methode permettant de modifier la case du robot
	 * @param oldRobot ancienne case du robot
	 * @param newRobot nouvelle case du robot
	 */
	public void setCaseRobot(Case oldRobot, Case newRobot){
		for(int i=0; i<caseRobots.length; i++){
			if(caseRobots[i]==oldRobot){
				caseRobots[i]=newRobot;
				return;
			}
		}
	}

	/**
	 * Déplace le robot dans cette direction
	 * @param caseRobot : case ou se situe le robot avant le deplacement
	 * @param direction : direction dans laquelle on souhaite deplacer le robot
	 * @return la nouvelle case
	 */
	public Case setPositionRobot(Case caseRobot, Mur direction) {
		int x=caseRobot.getX();
		int y=caseRobot.getY();
		Couleur robot = caseRobot.getRobot();
		plat[x][y].removeRobot();
		switch (direction) {
		case D:
			plat[x+1][y].addRobot(robot);
			caseRobot = plat[x+1][y];
			break;
		case G:
			plat[x-1][y].addRobot(robot);
			caseRobot = plat[x-1][y];
			break;
		case H:
			plat[x][y+1].addRobot(robot);
			caseRobot = plat[x][y+1];
			break;
		case B:
			plat[x][y-1].addRobot(robot);
			caseRobot = plat[x][y-1];
			break;
		}
		setCaseRobot(plat[x][y], caseRobot);
		return caseRobot;
		
	}
	
}
