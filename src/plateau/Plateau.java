package plateau;

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
	
	
	public Plateau(){
		
	}
	
	public void init() {
		for(int i=0; i<plat.length;i++)
			for(int j=0; j<plat[i].length;j++){
				plat[i][j]=new Case(i,j);
				if(i==0) plat[i][j].addMur(Mur.G);//Ajout des mur a la bordure gauche
				if(j==0) plat[i][j].addMur(Mur.B);//Ajout des mur a la bordure basse
				if(i==plat.length-1) plat[i][j].addMur(Mur.D);//Ajout des mur a la bordure droite
				if(j==plat[i].length-1) plat[i][j].addMur(Mur.H);//Ajout des mur a la bordure haute
			}
		
		// TODO Auto-generated method stub
		// Initialis�e la liste des murs et appeler update qui placera les robots et la cible
	}
	
	public void createPlateau(){
		plat[0][2].addMur(Mur.H);
		plat[0][3].addMur(Mur.B);
		plat[0][9].addMur(Mur.H);
		plat[0][10].addMur(Mur.B);
		plat[0][12].addMur(Mur.D);
		plat[1][12].addMur(Mur.G);
		
		plat[1][6].addMur(Mur.D);
		plat[2][6].addMur(Mur.G);
		plat[1][12].addMur(Mur.H);
		plat[1][13].addMur(Mur.B);
		
		
		plat[2][5].addMur(Mur.D);
		plat[3][5].addMur(Mur.G);
		plat[2][5].addMur(Mur.H);
		plat[2][6].addMur(Mur.B);
		
		plat[2][9].addMur(Mur.D);
		plat[3][9].addMur(Mur.G);
		plat[2][9].addMur(Mur.H);
		plat[2][10].addMur(Mur.B);
		
		
		
		plat[3][8].addMur(Mur.H);
		plat[3][9].addMur(Mur.B);
		
		plat[5][0].addMur(Mur.H);
		plat[5][1].addMur(Mur.B);
		plat[5][1].addMur(Mur.D);
		plat[6][1].addMur(Mur.G);
		
		plat[5][15].addMur(Mur.D);
		plat[6][15].addMur(Mur.G);
		
		plat[6][0].addMur(Mur.D);
		plat[7][0].addMur(Mur.G);
		
		plat[6][4].addMur(Mur.D);
		plat[7][4].addMur(Mur.G);
		
		plat[6][10].addMur(Mur.H);
		plat[6][11].addMur(Mur.B);
		
		plat[6][11].addMur(Mur.D);
		plat[7][11].addMur(Mur.G);
		
		plat[7][4].addMur(Mur.H);
		plat[7][5].addMur(Mur.B);
		
		plat[7][10].addMur(Mur.D);
		plat[8][10].addMur(Mur.G);
		
		plat[8][2].addMur(Mur.D);
		plat[9][2].addMur(Mur.G);
		
		plat[8][9].addMur(Mur.H);
		plat[8][10].addMur(Mur.B);
		
		plat[8][15].addMur(Mur.D);
		plat[9][15].addMur(Mur.G);
		
		plat[9][2].addMur(Mur.H);
		plat[9][3].addMur(Mur.B);
		
		plat[9][14].addMur(Mur.D);
		plat[10][14].addMur(Mur.G);
		
		plat[10][0].addMur(Mur.D);
		plat[11][0].addMur(Mur.G);
		
		plat[10][5].addMur(Mur.H);
		plat[10][6].addMur(Mur.B);
		
		plat[10][13].addMur(Mur.H);
		plat[10][14].addMur(Mur.B);
		
		plat[11][12].addMur(Mur.D);
		plat[12][12].addMur(Mur.G);
		
		plat[12][4].addMur(Mur.H);
		plat[12][5].addMur(Mur.B);
		
		plat[12][4].addMur(Mur.D);
		plat[13][4].addMur(Mur.G);
		
		plat[12][12].addMur(Mur.H);
		plat[12][13].addMur(Mur.B);
		
		plat[12][13].addMur(Mur.D);
		plat[13][13].addMur(Mur.G);
		
		plat[13][3].addMur(Mur.H);
		plat[13][4].addMur(Mur.B);
		
		plat[13][9].addMur(Mur.H);
		plat[13][10].addMur(Mur.B);
		
		plat[13][9].addMur(Mur.D);
		plat[14][9].addMur(Mur.G);
		
		plat[15][2].addMur(Mur.H);
		plat[15][3].addMur(Mur.B);
		
		plat[15][10].addMur(Mur.H);
		plat[15][11].addMur(Mur.B);
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
		for(int j=plat.length-1; j>=0; j--){
			for(int i=0; i<plat[j].length; i++){
				System.out.print(plat[i][j].getMur()+"\t");
			}
			System.out.println();
		}
	}
	
	public String toString(){
		return ""; // Chaine a envoyé aux clients
	}
	
	public static void main(String[] args ){
		Plateau plateau = new Plateau();
		plateau.init();
		plateau.createPlateau();
		plateau.display();
	}

}
