package utils;

import plateau.Case;
import plateau.Couleur;
import plateau.Mur;
import plateau.Plateau;

public abstract class PlateauUtils {

	public static Plateau getPlateauDeBase() {
		Case[][] plat = new Case[16][16];
		Couleur[] robots = new Couleur[4];
		
		for(int i=0; i<plat.length;i++){
			for(int j=0; j<plat[i].length;j++){
				plat[i][j]=new Case(i,j);
				if(i==0) plat[i][j].addMurDeBase(Mur.G);//Ajout des mur a la bordure gauche
				if(j==0) plat[i][j].addMurDeBase(Mur.B);//Ajout des mur a la bordure basse
				if(i==plat.length-1) plat[i][j].addMurDeBase(Mur.D);//Ajout des mur a la bordure droite
				if(j==plat[i].length-1) plat[i][j].addMurDeBase(Mur.H);//Ajout des mur a la bordure haute
			}
		}
		
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
		
		robots[0] = Couleur.R; // Conserver cette ordre pour respecter le protocole.
		robots[1] = Couleur.B;
		robots[2] = Couleur.J;
		robots[3] = Couleur.V;
		
		return new Plateau(plat, robots);
	}

	public static void display(Plateau plateauDeBase, String plateau, String enigme) {
		plateau = plateau.substring(1, plateau.length()); // retire première parenthese
		plateau = plateau.substring(0, plateau.length()-1); // dernière parenthèse
		plateau = plateau.replaceAll("\\)", "");
		System.out.println("Le plateau devient : "+plateau);
		String[] mursObstacles = plateau.split("\\(");
		enigme = enigme.replaceAll("\\(", "");
		enigme = enigme.replaceAll("\\)", "");
		String[] enigmeTab = enigme.split(",");
		
		Case[][] cases = plateauDeBase.getPlat();
		for(int i=0; i<mursObstacles.length; i++){
			String[] strs = mursObstacles[i].split(",");
			cases[Integer.parseInt(strs[0])][Integer.parseInt(strs[1])].addMur(Mur.getADirByName(strs[2]));
		}
		cases[Integer.parseInt(enigmeTab[0])][Integer.parseInt(enigmeTab[1])].addRobot(Couleur.R);
		cases[Integer.parseInt(enigmeTab[2])][Integer.parseInt(enigmeTab[3])].addRobot(Couleur.B);
		cases[Integer.parseInt(enigmeTab[4])][Integer.parseInt(enigmeTab[5])].addRobot(Couleur.J);
		cases[Integer.parseInt(enigmeTab[6])][Integer.parseInt(enigmeTab[7])].addRobot(Couleur.V);
		cases[Integer.parseInt(enigmeTab[8])][Integer.parseInt(enigmeTab[9])].addCible(Couleur.getCouleurByName(enigmeTab[10]));
		
		PlateauUtils.display(plateauDeBase.getPlat());
	}
	
	public static void display(Case[][] plat){
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
				else{
					if(plat[i][j].containsObstacleAtPosition(Mur.D) && i+1<plat.length
						&& plat[i+1][j].containsObstacleAtPosition(Mur.G)
						&& !plat[i+1][j].containsObstacleAtPosition(Mur.B)
						&& !plat[i+1][j].containsObstacleAtPosition(Mur.H)){
						
						if(plat[i][j].containsObstacleAtPosition(Mur.B)) System.out.print(" _ ");
						else System.out.print("   ");
						
					} else if(plat[i][j].containsObstacleAtPosition(Mur.D) && i+1<plat.length
							&& plat[i+1][j].containsObstacleAtPosition(Mur.G)
							&& plat[i+1][j].containsObstacleAtPosition(Mur.B)){
							
							if(plat[i][j].containsObstacleAtPosition(Mur.B)
								&& plat[i][j].containsObstacleAtPosition(Mur.G)) System.out.print("|_ ");
							else if(plat[i][j].containsObstacleAtPosition(Mur.B)) System.out.print(" _ ");
							else System.out.print("   ");
							
					} else {
						System.out.print(plat[i][j].display());
					}
				}
			}
			System.out.println();
		}
		System.out.print("    ");
		for(int i=0; i<plat.length; i++){
			if(i<10) System.out.print(i+"  ");
			else System.out.print(i+" ");
		}
		System.out.println("\n");
	}

}
