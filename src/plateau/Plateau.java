package plateau;

import java.util.Random;

public class Plateau {
	
	public static final String ANSI_RESET = "\u001B[0m";
	public static final String ANSI_BLACK = "\u001B[30m";
	public static final String ANSI_RED = "\u001B[31m";
	public static final String ANSI_GREEN = "\u001B[32m";
	public static final String ANSI_YELLOW = "\u001B[33m";
	public static final String ANSI_BLUE = "\u001B[34m";
	public static final String ANSI_PURPLE = "\u001B[35m";
	public static final String ANSI_CYAN = "\u001B[36m";
	public static final String ANSI_WHITE = "\u001B[37m";
	
	public final Case[][] plat=new Case[16][16];
	
	public Plateau(){ }
	
	public void init() {
		for(int i=0; i<plat.length;i++){
			for(int j=0; j<plat[i].length;j++){
				plat[i][j]=new Case(i,j);
				if(i==0) plat[i][j].addMur(Mur.G);//Ajout des mur a la bordure gauche
				if(j==0) plat[i][j].addMur(Mur.B);//Ajout des mur a la bordure basse
				if(i==plat.length-1) plat[i][j].addMur(Mur.D);//Ajout des mur a la bordure droite
				if(j==plat[i].length-1) plat[i][j].addMur(Mur.H);//Ajout des mur a la bordure haute
			}
		}
		
		// AJouter tous les murs du centre
		plat[6][7].addMur(Mur.D);
		plat[6][8].addMur(Mur.D);
		plat[7][9].addMur(Mur.B);
		plat[8][9].addMur(Mur.B);
		plat[9][7].addMur(Mur.G);
		plat[9][8].addMur(Mur.G);
		plat[7][7].addMur(Mur.B);
		plat[8][7].addMur(Mur.B);
		plat[7][6].addMurInvisible(Mur.H);
		plat[8][6].addMurInvisible(Mur.H);
		
		applyPlateau();
		// appeler update qui placera les robots et la cible
	}
	
	private void applyPlateau() {
		int plateauAlea = 1 + new Random().nextInt(PlateauChooser.NB_PLATEAU);
		System.out.println("Plateau n°"+plateauAlea+"\n");
		if(plateauAlea==1) PlateauChooser.applyPlateau1(plat);
		else if(plateauAlea==2) PlateauChooser.applyPlateau2(plat);
//		else if(plateauAlea==3) PlateauChooser.applyPlateau3(plat);
//		else if(plateauAlea==4) PlateauChooser.applyPlateau4(plat);
	}

	public void init(String d) {
		// TODO Auto-generated method stub
		// Initialis�e la liste des murs et appeler update qui placera les robots et la cible
	}
	
	public void update(/* parameters ?? */){
		//TODO
		// Mettre a jour la cible et la position des robots
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
//				System.out.print(plat[i][j].getMur()+"\t");
				if(i==0 && j<10) System.out.print(j+"  "+plat[i][j].toString());
				else if(i==0 && j>=10) System.out.print(j+" "+plat[i][j].toString());
				else System.out.print(plat[i][j].toString());
			}
			System.out.println();
		}
		System.out.print("    ");
		for(int i=0; i<plat.length; i++){
			if(i<10) System.out.print(i+"  ");
			else System.out.print(i+" ");
		}
	}
	
	public String toString(){
		return ""; // Chaine a envoyé aux clients
	}
	
	public static void main(String[] args ){
		Plateau plateau = new Plateau();
		plateau.init();
		plateau.display();
	}

}
