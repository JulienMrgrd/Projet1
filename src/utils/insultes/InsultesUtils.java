package utils.insultes;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public abstract class InsultesUtils {

	private static final String path = "./src/utils/insultes/dico_insultes_fr.txt";

	public static boolean isAnInsulte(String mot) throws IOException{
		BufferedReader br = null;
		boolean trouve = false;
		try{
			br = new BufferedReader(new FileReader(path));
		} catch(FileNotFoundException exc) { throw new FileNotFoundException("File not found" );  }
		
		String line = null; 

		try {
			while ((line = br.readLine()) != null)
			{
				line = line.toLowerCase();
				mot = mot.toLowerCase();
				if ( line.equals(mot) ){
					trouve = true;
					break;
				}
			}
		} catch(IOException ioe) { throw new IOException("Erreur IO" ); }
		
		close(br);
		return trouve;
	}

	private static void close(BufferedReader br){
		if(br!=null){
			try {
				br.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public static void main(String[] args) throws IOException {
		System.out.println(InsultesUtils.isAnInsulte("puTe"));
		System.out.println(InsultesUtils.isAnInsulte("pute"));
		System.out.println(InsultesUtils.isAnInsulte("PUTE"));
		System.out.println(InsultesUtils.isAnInsulte("nique"));
		System.out.println(InsultesUtils.isAnInsulte("ta mère"));
		System.out.println(InsultesUtils.isAnInsulte("connard"));
		System.out.println(InsultesUtils.isAnInsulte("imbecile"));
		System.out.println(InsultesUtils.isAnInsulte("put"));
	}

}
