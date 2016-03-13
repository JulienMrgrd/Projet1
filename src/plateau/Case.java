package plateau;

import java.util.ArrayList;
import java.util.List;

public class Case {
	public int x;
	public int y;
	public List<Mur> murs;
	public List<Mur> mursInvisibles;

	public Case(int x, int y){
		this.x=x;
		this.y=y;
		murs = new ArrayList<>();
		mursInvisibles = new ArrayList<>();
	}
	
	public void addMur(Mur posMur){
		if(!murs.contains(posMur)) murs.add(posMur);
	}
	
	public void addMurInvisible(Mur posMur){
		if(!mursInvisibles.contains(posMur)) mursInvisibles.add(posMur);
	}
	
	public List<Mur> getMurs(){
		return murs;
	}
	
	public List<Mur> getMursInvisibles(){
		return mursInvisibles;
	}
	
	public String toString(){
		String gauche = " ";
		String bas = " ";
		String droite = " ";
		
		if(murs.contains(Mur.B)) bas = "_";
		if(murs.contains(Mur.G)) gauche = "|";
		if(murs.contains(Mur.D)) droite = "|";
		
		return gauche+bas+droite;
	}
}
