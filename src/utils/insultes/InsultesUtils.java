package utils.insultes;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class InsultesUtils {

	public static boolean isAnInsulte(String mot){
		String line = null; 
		String path = "./src/utils/insultes/dico_insultes.txt";
		BufferedReader br = null;
		try {
			br = new BufferedReader(new FileReader(path));
			while ((line = br.readLine()) != null)
			{
				if ( line.equals(mot) ){
					return true;
				}
			}
			br.close();
		}
		catch(FileNotFoundException exc) { System.out.println("File not found" );  }
		catch(IOException ioe) { System.out.println("Erreur IO" ); }
		finally{
			if(br!=null)
				try {
					br.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
		}
		return false;
	}
	public static void main(String[] args) {
		System.out.println(InsultesUtils.isAnInsulte("pute"));
		System.out.println(InsultesUtils.isAnInsulte("nique"));
		System.out.println(InsultesUtils.isAnInsulte("con"));
		System.out.println(InsultesUtils.isAnInsulte("connard"));
		System.out.println(InsultesUtils.isAnInsulte("imbecile"));
		System.out.println(InsultesUtils.isAnInsulte("put"));
		
	}

}
