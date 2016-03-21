package server;

public class Enchere implements Comparable<Enchere>{
	
	private Joueur joueur;
	private int nbCoups;
	
	public Enchere(Joueur joueur, int nbCoups){
		this.setJoueur(joueur);
		this.setNbCoups(nbCoups);
	}

	public Joueur getJoueur() {
		return joueur;
	}

	public void setJoueur(Joueur joueur) {
		this.joueur = joueur;
	}

	public int getNbCoups() {
		return nbCoups;
	}

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
