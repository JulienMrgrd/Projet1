import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;


public class Joueur extends Thread{

	private String pseudo;
	private int score;
	private Socket client;
	private PrintWriter ecriture;
	private BufferedReader lecture;
	private Server server;

	public Joueur(Socket socket, Server server) {
		score = 0;
		client = socket;
		this.server = server;

		try {
			ecriture = new PrintWriter(socket.getOutputStream());
		} catch (IOException e) {
			System.out.println("(Joueur) : Obtiention outputStream de "+pseudo+" impossible.");
		}
		try {
			lecture = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		} catch (IOException e) {
			System.out.println("(Joueur) : Obtiention inputStream de "+pseudo+" impossible.");
		}
		
	}

	public String getPseudo() {
		return pseudo;
	}
	public void setPseudo(String pseudo) {
		this.pseudo = pseudo;
	}
	public int getScore() {
		return score;
	}
	public void setScore(int score) {
		this.score = score;
	}

	@Override
	public void run() {
		try {
			while(true){
				readFromJoueur();
			}
		} catch (IOException e) {}
	}

	/**
	 * throws IOException si le client se déconnecte
	 */
	public void sendToJoueur(String msg) throws IOException{
		if(ecriture!=null){
			ecriture.write(msg+"\n");
			ecriture.flush();
		}
	}

	private void readFromJoueur() throws IOException {
		String msg = "";
		while(msg.isEmpty() || !msg.contains("/")){
			msg = lecture.readLine();
			if(msg==null) msg = "";
		}
		
		String[] msgs = msg.split("/");
		
		if(msgs[0].startsWith(Protocole.CONNEX.title)){
			// Commencer la procédure d'ajout
			System.out.println("Nombre de connectes : "+server.getNbJoueurs());
			System.out.println("--------");
			return;
		
		} else if(msgs[0].startsWith(Protocole.BIENVENUE.title)){
			
			
		} else {
			System.out.println("Cette commande n'est pas supportee");
			System.out.println("Message pour users : \"users\"");
			System.out.println("Nombre de users : \"total\"");
			System.out.println("--------");
			return;
		}
		
	}


}
