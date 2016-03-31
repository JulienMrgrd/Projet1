package plateau;

import java.util.ArrayList;
import java.util.List;

import utils.StringUtils;

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
	
	public Case(Case c){
		this.x=c.getX();
		this.y=c.getY();
		this.canContainRobotsOrCible=c.canContainRobotsOrCible();
		this.murs=c.getMurs();
		this.mursInvisibles=c.getMursInvisibles();
		this.mursDeBase=c.getMursDeBase();
		this.robot=c.getRobot();
		this.cible=c.getCible();		
	}
	
	public boolean containsMur(){
		if( (murs!=null && murs.size()>0) 
				|| (mursInvisibles!=null && mursInvisibles.size()>0) 
				|| (mursDeBase!=null && mursDeBase.size()>0)){
			return true;
		}
		return false;
	}
	
	public boolean containsMurAtPosition(Mur m){
		if(containsMur() && 
				( (murs!=null && murs.contains(m)) 
				  || (mursInvisibles!=null && mursInvisibles.contains(m)) 
				  || (mursDeBase!=null && mursDeBase.contains(m)) ) ){
			return true;
		} else {
			return false;
		}
	}
	
	public boolean containsObstacleAtPosition(Mur m){
		if(!containsOnlyMursDeBase() && 
			murs!=null && murs.contains(m) ){
			return true;
		} else {
			return false;
		}
	}
	
	public boolean containsOnlyMursDeBase(){
		if( (murs==null || murs.isEmpty()) && (mursInvisibles==null || mursInvisibles.isEmpty())
				&& (mursDeBase!=null && mursDeBase.size()>0) ){
			return true;
		}
		return false;
	}
	
	public void addAllMurFromCase(Case c){
		if(c.murs != null){
			for(Mur m : c.murs){
				this.addMur(m);
			}
		}
		if(c.mursDeBase != null){
			for(Mur m : c.mursDeBase){
				this.addMurDeBase(m);
			}
		}
		if(c.mursInvisibles != null){
			for(Mur m : c.mursInvisibles){
				this.addMurInvisible(m);
			}
		}
	}
	
	public void addMur(Mur posMur){
		if(murs==null){
			murs = new ArrayList<>(2);
			murs.add(posMur);
		} else if(!murs.contains(posMur)) murs.add(posMur);
	}
	
	public void addMurInvisible(Mur posMur){
		if(mursInvisibles==null){
			mursInvisibles = new ArrayList<>(2);
			mursInvisibles.add(posMur);
		} else if(!mursInvisibles.contains(posMur)) mursInvisibles.add(posMur);
	}
	
	public void addMurDeBase(Mur posMur){
		if(mursDeBase==null){
			mursDeBase = new ArrayList<>(2);
			mursDeBase.add(posMur);
		} else if(!mursDeBase.contains(posMur)) mursDeBase.add(posMur);
	}
	
	public void removeAllMur(){
		if(this.murs!=null) this.murs=new ArrayList<>(2);
		if(this.mursDeBase!=null) this.mursDeBase=new ArrayList<>(2);
		if(this.mursInvisibles!=null) this.mursInvisibles=new ArrayList<>(2);
	}
	
	public String display(){
		String gauche = " ";
		String bas = " ";
		String droite = " ";
		
		if(murs.contains(Mur.B) || mursDeBase.contains(Mur.B)) bas = "_";
		if(murs.contains(Mur.G) || mursDeBase.contains(Mur.G)) gauche = "|";
		if(murs.contains(Mur.D) || mursDeBase.contains(Mur.D)) droite = "|";
		
		if(cible!=null && robot!=null){
			if(bas.equals("_")){
				if(gauche.equals(" ") && droite.equals(" ")){
					gauche="_";
					droite="_";
				} else if(gauche.equals(" ")){
					gauche="_";
				} else if (droite.equals(" ")){
					droite="_";
				}
				bas = StringUtils.underline("V");
			}
			else bas = "V";
		} else if(cible!=null){
			if(bas.equals("_")){
				if(gauche.equals(" ") && droite.equals(" ")){
					gauche="_";
					droite="_";
				} else if(gauche.equals(" ")){
					gauche="_";
				} else if (droite.equals(" ")){
					droite="_";
				}
				bas = StringUtils.underline("C");
			}
			else bas = "C";
			bas = StringUtils.transformInto(bas, cible);
			
		} else if(robot!=null){
			if(bas.equals("_")){
				if(gauche.equals(" ") && droite.equals(" ")){
					gauche="_";
					droite="_";
				} else if(gauche.equals(" ")){
					gauche="_";
				} else if (droite.equals(" ")){
					droite="_";
				}
				bas = StringUtils.underline("R");
			}
			else bas = "R";
			bas = StringUtils.transformInto(bas, robot);
		}
		
		return gauche+bas+droite;
	}
	
	public String toString(){
		if(!containsMur()) return "";
		String res = "";
		for(Mur m : murs){
			res+="("+x+","+y+","+m+")";
		}
		for(Mur m : mursInvisibles){
			res+="("+x+","+y+","+m+")";
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
	public List<Mur> getMursDeBase(){ return mursDeBase; }
	
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
