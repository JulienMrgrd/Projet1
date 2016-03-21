package server;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

import other.Protocole;
import other.ProtocoleCreator;


public class Joueur extends Thread{

	private String pseudo;
	private int score;
	private PrintWriter ecriture;
	private BufferedReader lecture;
	private Server server;
	
	private boolean hasQuit; // To stop listening and stop the thread

	public Joueur(Socket socket, Server server) {
		score = 0;
		this.server = server;
		hasQuit = false;

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
			while(hasQuit==false){
				readFromJoueur();
			}
		} catch (IOException e) {
			System.out.println("(Joueur run) Le Joueur "+ pseudo +" est parti");
			server.removeJoueur(this);
		} catch (Exception e) {
			System.out.println("(Joueur run) Exception : "+e.getMessage());
		}
	}

	/** throws IOException si le client se déconnecte
	 */
	public void sendToJoueur(String msg) throws IOException{
		if(ecriture!=null){
			ecriture.println(msg);
			ecriture.flush();
			if(ecriture.checkError()){
				throw new IOException();
			}
		}
	}
	
	/**
	 * 
	 */
	private void readFromJoueur() throws IOException {
		String msg = "";
		while(msg.isEmpty() || !msg.contains("/")){
			msg = lecture.readLine();
			if(msg==null) msg = "";
		}
		
		System.out.println("(SERVER) ReadFromJoueur reçoit : "+msg);
		
		String[] msgs = msg.split("/");
		String cmd = msgs[0];
		
		if(cmd.startsWith(Protocole.CONNEX.name())){ // CONNEX/user/
			
			String username = msgs[1];
			this.setPseudo(username);
			if(!server.addJoueur(this)){
				this.sendToJoueur(ProtocoleCreator.create(Protocole.USERNAME_ALREADY_USED, username));
			}
			return;
		
		} else if(cmd.startsWith(Protocole.SORT.name())){ // SORT/user/
			
			String username = msgs[1];
			if( !username.equals(pseudo) ){
				this.sendToJoueur(ProtocoleCreator.create(Protocole.BAD_PARAMETERS));
			
			} else if(server.removeJoueur(this)){
				this.sendToJoueur(ProtocoleCreator.create(Protocole.BYE));
				hasQuit = true;
			}
			return;
			
		} else if(cmd.startsWith(Protocole.TROUVE.name())){ // TROUVE/user/coups/
			Session session = server.getSession();
			if(session.hasStarted() && session.isPlaying(this) && session.isInReflexion()){
				boolean itIsTheFirst = false;
				int nbCoups = -1;
				try{
					nbCoups = Integer.parseInt(msgs[2]); // msgs[2] = nbCoups
				} catch (NumberFormatException exc){
					sendToJoueur(ProtocoleCreator.create(Protocole.BAD_PARAMETERS));
				}
				synchronized (session) {
					if(session.getVainqueurReflexion()==null && nbCoups!=-1){
						session.setVainqueurReflexion(this);
						session.setNbCoupsVainqueurReflexion(nbCoups);
						itIsTheFirst = true;
						session.notify(); // session is waiting for a solution
					}
				}
				if(itIsTheFirst){
					sendToJoueur(ProtocoleCreator.create(Protocole.TUASTROUVE));
					String ilatrouve = ProtocoleCreator.create(Protocole.ILATROUVE, pseudo, Integer.toString(nbCoups));
					server.sendToThemButThis(ilatrouve, session.getAllPlaying(), this);
					
				}
			}

		} else if(cmd.startsWith(Protocole.ENCHERE.name())){ // ENCHERE/user/coups/
			Session session = server.getSession();
			if(session.hasStarted() && session.isPlaying(this) && session.isInEnchere()){
				int nbCoups = -1;
				try{
					nbCoups = Integer.parseInt(msgs[2]); // msgs[2] = nbCoups
				} catch (NumberFormatException exc){}
				if(nbCoups>0){
					session.addEncheres(new Enchere(this, nbCoups));
				} else {
					sendToJoueur(ProtocoleCreator.create(Protocole.BAD_PARAMETERS));
				}
			}

		} else if(cmd.startsWith(Protocole.SOLUTION.name())){ // SORT/user/


		} else {
			this.sendToJoueur(Protocole.UNKNOWN_CMD.name());
			return;
		}

	}
	
	public boolean estEnVie(){
		try{
			sendToJoueur("");
			return true;
		} catch (IOException e){
			return false;
		}
	}
	
	public boolean hasQuit(){
		return hasQuit;
	}
	
	public String toString(){
		return pseudo;
	}


}
