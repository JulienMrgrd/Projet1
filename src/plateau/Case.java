package plateau;

import java.util.ArrayList;
import java.util.List;

public class Case {
	private int x;
	private int y;
	private boolean canContainRobotsOrCible;
	private List<Mur> murs;
	private List<Mur> mursInvisibles;
	private List<Mur> mursDeBase;
	private Couleur robot;
	private Couleur cible;

	public Case(int x, int y){
		this.x=x;
		this.y=y;
		canContainRobotsOrCible = true;
		murs = new ArrayList<>(2);
		mursInvisibles = new ArrayList<>(2);
		mursDeBase = new ArrayList<>(2);
	}
	
	
	public boolean containsMur(){
		if(murs.size()>0 || mursInvisibles.size()>0 || mursDeBase.size()>0){
			return true;
		}
		return false;
	}
	public boolean containsOnlyMursDeBase(){
		if(murs.isEmpty() && mursInvisibles.isEmpty() && mursDeBase.size()>0){
			return true;
		}
		return false;
	}
	
	public void addMur(Mur posMur){
		if(!murs.contains(posMur)) murs.add(posMur);
	}
	
	public void addMurInvisible(Mur posMur){
		if(!mursInvisibles.contains(posMur)) mursInvisibles.add(posMur);
	}
	
	public void addMurDeBase(Mur posMur){
		if(!mursDeBase.contains(posMur)) mursDeBase.add(posMur);
	}
	
	public String display(){
		String gauche = " ";
		String bas = " ";
		String droite = " ";
		
		if(murs.contains(Mur.B) || mursDeBase.contains(Mur.B)) bas = "_";
		if(murs.contains(Mur.G) || mursDeBase.contains(Mur.G)) gauche = "|";
		if(murs.contains(Mur.D) || mursDeBase.contains(Mur.D)) droite = "|";
		
		return gauche+bas+droite;
	}
	
	public String toString(){
		if(!containsMur()) return "";
		String res = "";
		for(Mur m : murs){
			res+="("+x+","+y+","+m+"),";
		}
		for(Mur m : mursInvisibles){
			res+="("+x+","+y+","+m+"),";
		}
		if(res.endsWith(",")) return res.substring(0, res.length()-1); // remove the last "," if exists 
		return res;
	}
	
	// =================== GETTERS / SETTERS ===============//
	
	public int getX() { return x; }
	public void setX(int x) { this.x = x; }
	public int getY() { return y; }
	public void setY(int y) { this.y = y;}
	
	public List<Mur> getMurs(){ return murs; }
	public List<Mur> getMursInvisibles(){ return mursInvisibles; }
	
	public void addRobot(Couleur robot){ this.robot = robot; }
	public void removeRobot(){ this.robot = null; }
	public Couleur getRobot(){ return robot;	}
	public boolean containsRobot(){ return robot!=null; }
	
	public void addCible(Couleur cible){ this.cible = cible; }
	public void removeCible(){ this.cible = null; }
	public Couleur getCible(){ return cible;	}
	public boolean containsCible(){ return cible!=null; }

	public boolean canContainRobotsOrCible() { return canContainRobotsOrCible; }
	public void setCanContainsRobotsOrCible(boolean canContainRobotsOrCible) { this.canContainRobotsOrCible = canContainRobotsOrCible; }
}
