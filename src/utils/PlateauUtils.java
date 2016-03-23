package utils;

import java.util.ArrayList;
import java.util.List;

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
	
	public static void display(Plateau plateauDeBase, String plateau, String enigme) {
		plateau = plateau.substring(1, plateau.length()); // retire première parenthese
		plateau = plateau.substring(0, plateau.length()-1); // dernière parenthèse
		plateau = plateau.replaceAll("\\)", "");
		String[] mursObstacles = plateau.split("\\(");
		
		enigme = enigme.replaceAll("\\(", "");
		enigme = enigme.replaceAll("\\)", "");
		String[] enigmeTab = enigme.split(",");
		
		Case[][] cases = plateauDeBase.getPlat();
		for(int i=0; i<mursObstacles.length; i++){
			String[] strs = mursObstacles[i].split(",");
			int x = Integer.parseInt(strs[0]);
			int y = Integer.parseInt(strs[1]);
			Mur monMur = Mur.getADirByName(strs[2]);
			if(strs[2].equals("D")){
				List<Mur> getMursVoisinDroite = getMurs(mursObstacles, cases[x+1][y].getMursDeBase(), x+1, y);
				if( getMursVoisinDroite.contains(Mur.H) || getMursVoisinDroite.contains(Mur.B) ){
					cases[x][y].addMurInvisible(monMur);
				} else {
					cases[x][y].addMur(monMur);
				}
			} else if(strs[2].equals("G")){
				List<Mur> mursVoisinGauche = getMurs(mursObstacles, cases[x-1][y].getMursDeBase(), x-1, y);
				List<Mur> mesMurs = getMurs(mursObstacles, cases[x][y].getMursDeBase(), x, y);
				if( (mursVoisinGauche.contains(Mur.H) || mursVoisinGauche.contains(Mur.B)) 
						&& (!mesMurs.contains(Mur.H) && !mesMurs.contains(Mur.B)) ){
					cases[x][y].addMurInvisible(monMur);
				} else {
					cases[x][y].addMur(monMur);
				}
			} else {
				cases[Integer.parseInt(strs[0])][Integer.parseInt(strs[1])].addMur(monMur);
			}
			
		}
		cases[Integer.parseInt(enigmeTab[0])][Integer.parseInt(enigmeTab[1])].addRobot(Couleur.R);
		cases[Integer.parseInt(enigmeTab[2])][Integer.parseInt(enigmeTab[3])].addRobot(Couleur.B);
		cases[Integer.parseInt(enigmeTab[4])][Integer.parseInt(enigmeTab[5])].addRobot(Couleur.J);
		cases[Integer.parseInt(enigmeTab[6])][Integer.parseInt(enigmeTab[7])].addRobot(Couleur.V);
		cases[Integer.parseInt(enigmeTab[8])][Integer.parseInt(enigmeTab[9])].addCible(Couleur.getCouleurByName(enigmeTab[10]));
		
		PlateauUtils.display(plateauDeBase.getPlat());
	}
	
	private static List<Mur> getMurs(String[] mursObstacles, List<Mur> mursDeBaseDeCaseXY, int x, int y){
		List<Mur> allMurs = new ArrayList<>();
		for(String obst : mursObstacles){
			String[] strs = obst.split(",");
			if(Integer.parseInt(strs[0])==x && Integer.parseInt(strs[1])==y){
				allMurs.add(Mur.getADirByName(strs[2]));
			}
		}
		allMurs.addAll(mursDeBaseDeCaseXY);
		return allMurs;
	}
	
	public static void main(String[] args) {
		Plateau plateau = getPlateauDeBase();
		Plateau plateauAlea = new Plateau();
		plateauAlea.init();
		
		PlateauUtils.display(plateau, plateauAlea.plateau(), plateauAlea.enigme());
	}
}
