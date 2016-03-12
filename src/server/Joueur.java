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
			server.sendAllButThis(ProtocoleCreator.create(Protocole.SORTI,pseudo), this);
		} catch (Exception e) {
			System.out.println("(Joueur run) Exception : "+e.getMessage());
		}
	}

	/**
	 * throws IOException si le client se déconnecte
	 */
	public void sendToJoueur(String msg) throws IOException{
		if(ecriture!=null){
			ecriture.println(msg);
			ecriture.flush();
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
		
		if(cmd.startsWith(Protocole.CONNEX.title)){ // CONNEX/user/
			
			String username = msgs[1];
			this.setPseudo(username);
			if(server.addJoueur(this)==false){
				this.sendToJoueur(ProtocoleCreator.create(Protocole.USERNAME_ALREADY_USED, username));
			} else {
				server.sendAllButThis(ProtocoleCreator.create(Protocole.BIENVENUE,username), this);
				this.sendToJoueur(ProtocoleCreator.create(Protocole.CONNECTE,username));
			}
			
			return;
		
		} else if(cmd.startsWith(Protocole.SORT.title)){ // SORT/user/
			
			String username = msgs[1];
			if( !username.equals(pseudo) ){
				this.sendToJoueur(ProtocoleCreator.create(Protocole.BAD_PARAMETERS));
			
			} else if(server.removeJoueur(this)){
				server.sendAllButThis(ProtocoleCreator.create(Protocole.SORTI,username), this);
				this.sendToJoueur(ProtocoleCreator.create(Protocole.BYE));
				hasQuit = true;
			}
			
			return;
			
		} else if(cmd.startsWith(Protocole.TROUVE.title)){ // SORT/user/


		} else if(cmd.startsWith(Protocole.ENCHERE.title)){ // SORT/user/


		} else if(cmd.startsWith(Protocole.SOLUTION.title)){ // SORT/user/


		} else {
			this.sendToJoueur(Protocole.UNKNOWN_CMD.title);
			return;
		}

	}
	
	public boolean hasQuit(){
		return hasQuit;
	}
	
	public String toString(){
		return pseudo;
	}


}
