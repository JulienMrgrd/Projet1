package plateau;

import java.util.Random;

import utils.StringUtils;

public class Plateau {
	
	private Case[][] plat = new Case[16][16];
	private Couleur[] robots = new Couleur[4];
	private PlateauChooser chooser;
	private Case caseCible;
	private Case[] caseRobots;
	
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
		
		robots[0] = Couleur.B;
		robots[1] = Couleur.J;
		robots[2] = Couleur.R;
		robots[3] = Couleur.V;
		
		// TODO : ajouter les robots
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
	}
	
	public static void main(String[] args ){
		Plateau plateau = new Plateau();
		plateau.init();
		plateau.display();
		System.out.println("\n"+plateau.plateau());
	}

}
