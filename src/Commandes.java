import java.io.*;

class Commandes implements Runnable
{
  BlablaServ _blablaServ; // pour utilisation des méthodes de la classe principale
  BufferedReader _in; // pour gestion du flux d'entrée (celui de la console)
  String _strCommande=""; // contiendra la commande tapée
  Thread _t; // contiendra le thread

  public Commandes(BlablaServ blablaServ) {
    _blablaServ=blablaServ; // passage de local en global
    // le flux d'entrée de la console sera géré plus pratiquement dans un BufferedReader
    _in = new BufferedReader(new InputStreamReader(System.in));
    _t = new Thread(this); // instanciation du thread
    _t.start(); // demarrage du thread, la fonction run() est ici lancée
  }

  public void run() {
    try
    {
      while ((_strCommande=_in.readLine())!=null)
      {
        if (_strCommande.equalsIgnoreCase("quit")) // commande "quit" detectée ...
          System.exit(0); // ... on ferme alors le serveur
        else if(_strCommande.equalsIgnoreCase("total")) // commande "total" detectée ...
        {
          System.out.println("Nombre de connectes : "+_blablaServ.getNbClients());
          System.out.println("--------");
        }
        else
        {
          System.out.println("Cette commande n'est pas supportee");
          System.out.println("Quitter : \"quit\"");
          System.out.println("Nombre de connectes : \"total\"");
          System.out.println("--------");
        }
        System.out.flush(); // on affiche tout ce qui est en attente dans le flux
      }
    }
    catch (IOException e) {}
  }
}