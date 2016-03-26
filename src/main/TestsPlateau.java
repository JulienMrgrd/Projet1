package main;

import java.util.Scanner;

import plateau.Plateau;
import utils.PlateauUtils;
import utils.ResolutionUtils;

public class TestsPlateau {

	public static void main(String[] args ){
		Plateau plateau = new Plateau();
		Scanner sc = new Scanner(System.in);
		String str = "";
		while(!str.equals("stop")){
			System.out.println("\nNouveau Plateau !\n");
			plateau.init();
			System.out.println("\nPlateau avant ...");
			PlateauUtils.display(plateau.getPlat());
			System.out.println("\n"+plateau.plateau());
			System.out.println(plateau.enigme());
			System.out.print("Votre solution : ");
			str = sc.nextLine();
			if(str.equals("stop")) break;
			boolean solution = ResolutionUtils.isGoodSolution(str,plateau);
			System.out.println("Votre solution est : "+solution+"\n");
			
			System.out.println("\nPlateau après ...");
			PlateauUtils.display(plateau.getPlat());
			
			while(!solution){
				System.out.print("\nRedonnez une solution : ");
				str = sc.nextLine();
				solution = ResolutionUtils.isGoodSolution(str,plateau);
				System.out.println("Votre solution est : "+solution+"\n\n");
				
				System.out.println("\nPlateau après ...");
				PlateauUtils.display(plateau.getPlat());
			}
		}
		sc.close();
	}


}
