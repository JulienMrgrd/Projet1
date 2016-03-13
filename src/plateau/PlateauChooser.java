package plateau;

public abstract class PlateauChooser {

	public static final int NB_PLATEAU = 1;

	public static void applyPlateau1(Case[][] plat) {
		plat[0][2].addMur(Mur.H);
		plat[0][3].addMur(Mur.B);
		plat[0][9].addMur(Mur.H);
		plat[0][10].addMur(Mur.B);
		plat[0][12].addMurInvisible(Mur.D);
		plat[1][12].addMur(Mur.G);
		
		plat[1][6].addMurInvisible(Mur.D);
		plat[2][6].addMur(Mur.G);
		plat[1][12].addMur(Mur.H);
		plat[1][13].addMur(Mur.B);
				
		plat[2][5].addMurInvisible(Mur.D);
		plat[3][5].addMur(Mur.G);
		plat[2][5].addMur(Mur.H);
		plat[2][6].addMur(Mur.B);
		
		plat[2][9].addMurInvisible(Mur.D);
		plat[3][9].addMur(Mur.G);
		plat[2][9].addMur(Mur.H);
		plat[2][10].addMur(Mur.B);
		
		plat[3][8].addMur(Mur.H);
		plat[3][9].addMur(Mur.B);
		
		plat[5][0].addMur(Mur.H);
		plat[5][1].addMur(Mur.B);
		plat[5][1].addMurInvisible(Mur.D);
		plat[6][1].addMur(Mur.G);
		
		plat[5][15].addMurInvisible(Mur.D);
		plat[6][15].addMur(Mur.G);
		
		plat[6][0].addMurInvisible(Mur.D);
		plat[7][0].addMur(Mur.G);
		
		plat[6][4].addMurInvisible(Mur.D);
		plat[7][4].addMur(Mur.G);
		
		plat[6][10].addMur(Mur.H);
		plat[6][11].addMur(Mur.B);
		
		plat[6][11].addMur(Mur.D);
		plat[7][11].addMurInvisible(Mur.G);
		
		plat[7][4].addMur(Mur.H);
		plat[7][5].addMur(Mur.B);
		
		plat[7][10].addMurInvisible(Mur.D);
		plat[8][10].addMur(Mur.G);
		
		plat[8][2].addMurInvisible(Mur.D);
		plat[9][2].addMur(Mur.G);
		
		plat[8][9].addMur(Mur.H);
		plat[8][10].addMur(Mur.B);
		
		plat[8][15].addMurInvisible(Mur.D);
		plat[9][15].addMur(Mur.G);
		
		plat[9][2].addMur(Mur.H);
		plat[9][3].addMur(Mur.B);
		
		plat[9][14].addMurInvisible(Mur.D);
		plat[10][14].addMur(Mur.G);
		
		plat[10][0].addMurInvisible(Mur.D);
		plat[11][0].addMur(Mur.G);
		
		plat[10][5].addMur(Mur.H);
		plat[10][6].addMur(Mur.B);
		
		plat[10][6].addMur(Mur.D);
		plat[11][6].addMurInvisible(Mur.G);
		
		plat[10][13].addMur(Mur.H);
		plat[10][14].addMur(Mur.B);
		
		plat[11][12].addMurInvisible(Mur.D);
		plat[12][12].addMur(Mur.G);
		
		plat[12][4].addMur(Mur.H);
		plat[12][5].addMur(Mur.B);
		
		plat[12][4].addMurInvisible(Mur.D);
		plat[13][4].addMur(Mur.G);
		
		plat[12][12].addMur(Mur.H);
		plat[12][13].addMur(Mur.B);
		
		plat[12][13].addMur(Mur.D);
		plat[13][13].addMurInvisible(Mur.G);
		
		plat[13][3].addMur(Mur.H);
		plat[13][4].addMur(Mur.B);
		
		plat[13][9].addMur(Mur.H);
		plat[13][10].addMur(Mur.B);
		
		plat[13][9].addMurInvisible(Mur.D);
		plat[14][9].addMur(Mur.G);
		
		plat[15][2].addMur(Mur.H);
		plat[15][3].addMur(Mur.B);
		
		plat[15][10].addMur(Mur.H);
		plat[15][11].addMur(Mur.B);
	}
}
