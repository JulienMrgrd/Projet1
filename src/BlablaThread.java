
import java.net.*;
import java.io.*;

class BlablaThread implements Runnable
{
  private Thread _t; // contiendra le thread du client
  private Socket _s; // recevra le socket liant au client
  private PrintWriter _out; // pour gestion du flux de sortie
  private BufferedReader _in; // pour gestion du flux d'entrée
  private BlablaServ _blablaServ; // pour utilisation des méthodes de la classe principale
  private int _numClient=0; // contiendra le numéro de client géré par ce thread

  BlablaThread(Socket s, BlablaServ blablaServ) // le param s est donnée dans BlablaServ par ss.accept()
  {
    _blablaServ=blablaServ; // passage de local en global (pour gestion dans les autres méthodes)
    _s=s; // passage de local en global
    try
    {
      _out = new PrintWriter(_s.getOutputStream());
      _in = new BufferedReader(new InputStreamReader(_s.getInputStream()));
      _numClient = blablaServ.addClient(_out);
    }
    catch (IOException e){ }

    _t = new Thread(this); // instanciation du thread
    _t.start(); // demarrage du thread, la fonction run() est ici lancée
  }

  public void run()
  {
    String message = ""; // déclaration de la variable qui recevra les messages du client
    System.out.println("Un nouveau client s'est connecte, no "+_numClient);
    try
    {
      char charCur[] = new char[1]; // déclaration d'un tableau de char d'1 élement, _in.read() y stockera le char lu
      while(_in.read(charCur, 0, 1)!=-1) // attente en boucle des messages provenant du client (bloquant sur _in.read())
      {
        if (charCur[0] != '\u0000' && charCur[0] != '\n' && charCur[0] != '\r')
                message += charCur[0]; // ... si non, on concatène le caractère dans le message
        else if(!message.equalsIgnoreCase("")) // juste une vérification de principe
        {
          if(charCur[0]=='\u0000') // le dernier caractère était '\u0000' (char de terminaison nulle)
            _blablaServ.sendAll(message,""+charCur[0]);
          else _blablaServ.sendAll(message,""); // sinon on envoi le message à tous
          message = ""; // on vide la chaine de message pour qu'elle soit réutilisée
        }
      }
    }
    catch (Exception e){ }
    finally // finally se produira le plus souvent lors de la deconnexion du client
    {
      try
      {
        System.out.println("Le client no "+_numClient+" s'est deconnecte");
        _blablaServ.delClient(_numClient); // on supprime le client de la liste
        _s.close(); // fermeture du socket si il ne l'a pas déjà été (à cause de l'exception levée plus haut)
      }
      catch (IOException e){ }
    }
  }
}