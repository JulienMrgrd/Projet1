package server;

public class Enchere implements Comparable<Enchere>{
	
	private Joueur joueur;
	private int nbCoups;
	
	public Enchere(Joueur joueur, int nbCoups){
		this.setJoueur(joueur);
		this.setNbCoups(nbCoups);
	}

	/**
	 * Recuperation du joueur
	 * @return joueur
	 */
	public Joueur getJoueur() {
		return joueur;
	}

	/**
	 * Modification du nombre de Coups
	 * @param joueur nouveau joueur
	 */
	public void setJoueur(Joueur joueur) {
		this.joueur = joueur;
	}

	/**
	 * Recuperation du nombre de coups
	 * @return le nombre de coups
	 */
	public int getNbCoups() {
		return nbCoups;
	}

	/**
	 * Modification du nombre de Coups
	 * @param nbCoups nouveau nbCoups
	 */
	public void setNbCoups(int nbCoups) {
		this.nbCoups = nbCoups;
	}

	@Override
	public int compareTo(Enchere o) {
		if(this.nbCoups>o.getNbCoups()) return 1;
		else if (this.nbCoups==o.getNbCoups()) return 0;
		return -1;
	}
}
