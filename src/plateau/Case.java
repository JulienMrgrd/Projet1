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

	/**
	 * Constructeur de la case
	 * @param x Coordonnée en abscisse de la case
	 * @param y Coordonnée en ordonnée de la case
	 */
	public Case(int x, int y){
		this.x=x;
		this.y=y;
		canContainRobotsOrCible = true;
		murs = new ArrayList<>(2);
		mursInvisibles = new ArrayList<>(2);
		mursDeBase = new ArrayList<>(2);
	}
	
	/**
	 * Methode permettant de savoir si il y a un mur sur cette case
	 * @return vrai si il y a un mur sur cette case, faux sinon
	 */
	public boolean containsMur(){
		if( (murs!=null && murs.size()>0) 
				|| (mursInvisibles!=null && mursInvisibles.size()>0) 
				|| (mursDeBase!=null && mursDeBase.size()>0)){
			return true;
		}
		return false;
	}
	
	/**
	 * Methode permettant de savoir si il y a un mur à la position "m" sur cette case
	 * @param m ("H","B","G","D") position de la case ou l'on souhaite savoir si il y a un mur
	 * @return vrai si il y a un mur a la position m, faux sinon
	 */
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

	/**
	 * Methode permettant d'ajouter un mur à la position "posMur" sur cette case
	 * @param posMur position de la case ("H","B","G","D") ou l'on souhaite ajouter le mur
	 */
	public void addMur(Mur posMur){
		if(murs==null){
			murs = new ArrayList<>(2);
			murs.add(posMur);
		} else if(!murs.contains(posMur)) murs.add(posMur);
	}
	
	/**
	 * Methode permettant d'ajouter un mur invisible à la position "posMur" sur cette case, Mur qui ne sera pas afficher dans le client.java
	 * permettant de tester le serveur
	 * @param posMur position de la case ("H","B","G","D") ou l'on souhaite ajouter le mur
	 */
	public void addMurInvisible(Mur posMur){
		if(mursInvisibles==null){
			mursInvisibles = new ArrayList<>(2);
			mursInvisibles.add(posMur);
		} else if(!mursInvisibles.contains(posMur)) mursInvisibles.add(posMur);
	}
	
	/**
	 * Methode permettant d'ajouter un mur de base à la position "posMur" sur cette case,
	 * @param posMur position de la case ("H","B","G","D") ou l'on souhaite ajouter le mur
	 */
	public void addMurDeBase(Mur posMur){
		if(mursDeBase==null){
			mursDeBase = new ArrayList<>(2);
			mursDeBase.add(posMur);
		} else if(!mursDeBase.contains(posMur)) mursDeBase.add(posMur);
	}	
	
	/**
	 * Methode retournant l'affichage de chaque case
	 * @return l'affichage de la case
	 */
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
	
	/**
	 * methode retournant les détails de la case utiles pour le bilan
	 * @return un string de la forme "(x,y,H)(x,y,B)..."
	 */
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
	
	/**
	 * retourne l'abscisse de la case
	 * @return abscisse de la case
	 */
	public int getX() { return x; }
	
	/**
	 * retourne l'ordonnée de la case
	 * @return l'ordonnée de la case
	 */
	public int getY() { return y; }
	
	/**
	 * @return tous les murs de base d'un plateau, mur entourant le plateau et centraux
	 */
	public List<Mur> getMursDeBase(){ return mursDeBase; }
	
	/**
	 * ajout d'un robot dans cette case
	 * @param robot : Couleur du robot qu'on souhaite 
	 */
	public void addRobot(Couleur robot){ this.robot = robot; }
	
	/**
	 * suppresion du robot sur la case
	 */
	public void removeRobot(){ this.robot = null; }
	
	/**
	 * Methode permettant de connaitre la couleur du robot sur la case 
	 * @return la couleur du robot sur la case
	 */
	public Couleur getRobot(){ return robot;	}
	
	/**
	 * Methode permettant de savoir il y a un robot sur la case
	 * @return vrai si il y a un robot dans la case, faux sinon
	 */
	public boolean containsRobot(){ return robot!=null; }
	
	/**
	 * Ajout d'une cible sur la case
	 * @param cible : Couleur de la cible
	 */
	public void addCible(Couleur cible){ this.cible = cible; }
	
	/**
	 * Methode permettant de connaitre la couleur du robot sur la case 
	 * @return la couleur de la cible sur la case
	 */
	public Couleur getCible(){ return cible;	}
	
	/**
	 * Methode permettant de savoir il y a une cible sur la case
	 * @return vrai si il y a une cible dans la case, faux sinon
	 */
	public boolean containsCible(){ return cible!=null; }

	/**
	 * Methode permettant de savoir si il peux y avoir une cible ou un robot sur la case
	 * @return vrai si il y peux y avoir une cible ou un robot dans la case, faux sinon
	 */
	public boolean canContainRobotsOrCible() { return canContainRobotsOrCible; }
	
	/**
	 * Methode permettant de modifier si la case peux posserder une cible ou un robot sur la case
	 * @param canContainRobotsOrCible : vrai si la case peux contenir un robot ou une cible et faux sinonS
	 */
	public void setCanContainsRobotsOrCible(boolean canContainRobotsOrCible) { this.canContainRobotsOrCible = canContainRobotsOrCible; }
}
