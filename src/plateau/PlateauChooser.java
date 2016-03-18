package plateau;

import java.util.Random;

public class PlateauChooser {

	public static final int NB_PLATEAU = 2;
	private Case[][] plat;
	private int chosenPlateau;
	private Case[] casesAvecDeuxMursContigus;
	
	public PlateauChooser(Case[][]plat){
		this.plat = plat;
	}
	
	public int applyRandomPlateau(){
		int plateauAlea = 1 + new Random().nextInt(PlateauChooser.NB_PLATEAU);
		chosenPlateau = plateauAlea;
		System.out.print("Plateau n°"+plateauAlea+": " );
		if(plateauAlea==1){
			applyPlateau1();
			System.out.println("76 murs\n");
		} else if(plateauAlea==2){
			applyPlateau2();
			System.out.println("84 murs\n");
		}
		return chosenPlateau;
	}
	
	//http://www.vindjeu.eu/prd/wp-content/uploads/2012/03/419-Ricochet-Robot-2.jpg
	private void applyPlateau2() {
		plat[0][1].addMur(Mur.H);
		plat[0][2].addMur(Mur.B);
		plat[0][4].addMurInvisible(Mur.D);
		plat[0][8].addMur(Mur.H);
		plat[0][9].addMur(Mur.B);
		plat[0][11].addMurInvisible(Mur.D);
		
		plat[1][4].addMur(Mur.H);
		plat[1][4].addMur(Mur.G);
		plat[1][5].addMur(Mur.B);
		plat[1][10].addMur(Mur.H);
		plat[1][11].addMur(Mur.G);
		plat[1][11].addMur(Mur.B);
		plat[1][15].addMur(Mur.D);
		
		plat[2][15].addMurInvisible(Mur.G);
		plat[2][14].addMurInvisible(Mur.D);
		
		plat[3][0].addMur(Mur.H);
		plat[3][1].addMur(Mur.B);
		plat[3][1].addMur(Mur.D);
		plat[3][14].addMur(Mur.H);
		plat[3][14].addMur(Mur.G);
		plat[3][15].addMur(Mur.B);
		
		plat[4][1].addMurInvisible(Mur.G);
		plat[4][5].addMurInvisible(Mur.D);
		plat[4][9].addMur(Mur.H);
		plat[4][9].addMur(Mur.D);
		plat[4][10].addMur(Mur.B);
		
		plat[5][0].addMurInvisible(Mur.D);
		plat[5][4].addMur(Mur.H);
		plat[5][5].addMur(Mur.B);
		plat[5][5].addMur(Mur.G);
		plat[5][9].addMurInvisible(Mur.G);
		
		plat[6][0].addMur(Mur.G);
		plat[6][3].addMur(Mur.H);
		plat[6][3].addMur(Mur.D);
		plat[6][4].addMur(Mur.B);
		plat[6][11].addMur(Mur.H);
		plat[6][12].addMur(Mur.D);
		plat[6][12].addMur(Mur.B);
		
		plat[7][3].addMurInvisible(Mur.G);
		plat[7][12].addMurInvisible(Mur.G);
		
		plat[9][0].addMurInvisible(Mur.D);
		plat[9][13].addMur(Mur.H);
		plat[9][14].addMur(Mur.D);
		plat[9][14].addMur(Mur.B);
		
		plat[10][0].addMur(Mur.G);
		plat[10][3].addMur(Mur.H);
		plat[10][3].addMur(Mur.D);
		plat[10][4].addMur(Mur.B);
		plat[10][5].addMurInvisible(Mur.D);
		plat[10][9].addMur(Mur.H);
		plat[10][9].addMur(Mur.D);
		plat[10][10].addMur(Mur.B);
		plat[10][12].addMurInvisible(Mur.D);
		plat[10][14].addMurInvisible(Mur.G);
		
		plat[11][3].addMurInvisible(Mur.G);
		plat[11][5].addMur(Mur.G);
		plat[11][5].addMur(Mur.H);
		plat[11][6].addMur(Mur.B);
		plat[11][9].addMurInvisible(Mur.G);
		plat[11][10].addMur(Mur.H);
		plat[11][12].addMur(Mur.G);
		plat[11][12].addMur(Mur.B);
		plat[11][15].addMurInvisible(Mur.D);
		
		plat[12][1].addMur(Mur.H);
		plat[12][2].addMur(Mur.B);
		plat[12][2].addMur(Mur.D);
		plat[12][4].addMurInvisible(Mur.D);
		plat[12][8].addMurInvisible(Mur.D);
		plat[12][15].addMur(Mur.G);
		
		plat[13][2].addMurInvisible(Mur.G);
		plat[13][3].addMur(Mur.H);
		plat[13][4].addMur(Mur.G);
		plat[13][4].addMur(Mur.B);
		plat[13][7].addMur(Mur.H);
		plat[13][8].addMur(Mur.G);
		plat[13][8].addMur(Mur.B);
		plat[13][10].addMurInvisible(Mur.D);
		
		plat[14][10].addMur(Mur.G);
		plat[14][10].addMur(Mur.H);
		plat[14][11].addMur(Mur.B);
		
		plat[15][3].addMur(Mur.H);
		plat[15][4].addMur(Mur.B);
		plat[15][12].addMur(Mur.H);
		plat[15][13].addMur(Mur.B);
		
		casesAvecDeuxMursContigus = new Case[17];
		casesAvecDeuxMursContigus[0] = plat[1][4];
		casesAvecDeuxMursContigus[1] = plat[1][11];
		casesAvecDeuxMursContigus[2] = plat[3][1];
		casesAvecDeuxMursContigus[3] = plat[3][14];
		casesAvecDeuxMursContigus[4] = plat[4][9];
		casesAvecDeuxMursContigus[5] = plat[5][5];
		casesAvecDeuxMursContigus[6] = plat[6][3];
		casesAvecDeuxMursContigus[7] = plat[6][12];
		casesAvecDeuxMursContigus[8] = plat[9][14];
		casesAvecDeuxMursContigus[9] = plat[10][3];
		casesAvecDeuxMursContigus[10] = plat[10][9];
		casesAvecDeuxMursContigus[11] = plat[11][5];
		casesAvecDeuxMursContigus[12] = plat[11][12];
		casesAvecDeuxMursContigus[13] = plat[12][2];
		casesAvecDeuxMursContigus[14] = plat[13][4];
		casesAvecDeuxMursContigus[15] = plat[13][8];
		casesAvecDeuxMursContigus[16] = plat[14][10];
	} // fin plateau 2
	
	// http://www.jeuxadeux.com/images/ricochet_robots_2_gd.jpg
	private void applyPlateau1() {
		plat[0][2].addMur(Mur.H);
		plat[0][3].addMur(Mur.B);
		plat[0][9].addMur(Mur.H);
		plat[0][10].addMur(Mur.B);
		plat[0][12].addMurInvisible(Mur.D);
		
		plat[1][12].addMur(Mur.G);
		plat[1][6].addMurInvisible(Mur.D);
		plat[1][12].addMur(Mur.H);
		plat[1][13].addMur(Mur.B);
				
		plat[2][6].addMur(Mur.G);
		plat[2][5].addMurInvisible(Mur.D);
		plat[2][5].addMur(Mur.H);
		plat[2][6].addMur(Mur.B);
		plat[2][9].addMurInvisible(Mur.D);
		plat[2][9].addMur(Mur.H);
		plat[2][10].addMur(Mur.B);
		
		plat[3][9].addMur(Mur.G);
		plat[3][5].addMur(Mur.G);
		plat[3][8].addMur(Mur.H);
		plat[3][9].addMur(Mur.B);
		
		plat[5][0].addMur(Mur.H);
		plat[5][1].addMur(Mur.B);
		plat[5][1].addMur(Mur.D);
		plat[5][15].addMurInvisible(Mur.D);

		plat[6][1].addMurInvisible(Mur.G);
		plat[6][15].addMur(Mur.G);
		plat[6][0].addMurInvisible(Mur.D);
		plat[6][4].addMurInvisible(Mur.D);
		plat[6][10].addMur(Mur.H);
		plat[6][11].addMur(Mur.B);
		plat[6][11].addMur(Mur.D);

		plat[7][0].addMur(Mur.G);
		plat[7][4].addMur(Mur.G);
		plat[7][11].addMurInvisible(Mur.G);
		plat[7][4].addMur(Mur.H);
		plat[7][5].addMur(Mur.B);
		plat[7][10].addMurInvisible(Mur.D);

		plat[8][10].addMur(Mur.G);
		plat[8][2].addMurInvisible(Mur.D);
		plat[8][9].addMur(Mur.H);
		plat[8][10].addMur(Mur.B);
		plat[8][15].addMurInvisible(Mur.D);
		
		plat[9][2].addMur(Mur.G);
		plat[9][15].addMur(Mur.G);
		plat[9][2].addMur(Mur.H);
		plat[9][3].addMur(Mur.B);
		plat[9][14].addMurInvisible(Mur.D);

		plat[10][14].addMur(Mur.G);
		plat[10][0].addMurInvisible(Mur.D);
		plat[10][5].addMur(Mur.H);
		plat[10][6].addMur(Mur.B);
		plat[10][6].addMur(Mur.D);
		plat[10][13].addMur(Mur.H);
		plat[10][14].addMur(Mur.B);

		plat[11][0].addMur(Mur.G);
		plat[11][6].addMurInvisible(Mur.G);
		plat[11][12].addMurInvisible(Mur.D);

		plat[12][12].addMur(Mur.G);
		plat[12][4].addMur(Mur.H);
		plat[12][5].addMur(Mur.B);
		plat[12][4].addMurInvisible(Mur.D);
		plat[12][12].addMur(Mur.H);
		plat[12][13].addMur(Mur.B);
		plat[12][13].addMur(Mur.D);

		plat[13][4].addMur(Mur.G);
		plat[13][13].addMurInvisible(Mur.G);
		plat[13][3].addMur(Mur.H);
		plat[13][4].addMur(Mur.B);
		plat[13][9].addMur(Mur.H);
		plat[13][10].addMur(Mur.B);
		plat[13][9].addMur(Mur.D);
		
		plat[14][9].addMurInvisible(Mur.G);
		
		plat[15][2].addMur(Mur.H);
		plat[15][3].addMur(Mur.B);
		plat[15][10].addMur(Mur.H);
		plat[15][11].addMur(Mur.B);
		
		casesAvecDeuxMursContigus = new Case[17];
		casesAvecDeuxMursContigus[0] = plat[1][12];
		casesAvecDeuxMursContigus[1] = plat[2][5];
		casesAvecDeuxMursContigus[2] = plat[2][6];
		casesAvecDeuxMursContigus[3] = plat[2][9];
		casesAvecDeuxMursContigus[4] = plat[3][9];
		casesAvecDeuxMursContigus[5] = plat[5][1];
		casesAvecDeuxMursContigus[6] = plat[6][11];
		casesAvecDeuxMursContigus[7] = plat[7][4];
		casesAvecDeuxMursContigus[8] = plat[8][10];
		casesAvecDeuxMursContigus[9] = plat[9][2];
		casesAvecDeuxMursContigus[10] = plat[10][6];
		casesAvecDeuxMursContigus[11] = plat[10][14];
		casesAvecDeuxMursContigus[12] = plat[12][4];
		casesAvecDeuxMursContigus[13] = plat[12][12];
		casesAvecDeuxMursContigus[14] = plat[12][13];
		casesAvecDeuxMursContigus[15] = plat[13][4];
		casesAvecDeuxMursContigus[16] = plat[13][9];
	} // fin plateau 1
	
	public int getChosenPlateau(){
		return chosenPlateau;
	}
	
	public Case[] getCasesAvecDeuxMursContigus(){
		return casesAvecDeuxMursContigus;
	}
}
