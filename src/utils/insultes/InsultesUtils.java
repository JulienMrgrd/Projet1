package utils.insultes;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class InsultesUtils {

	String path = "./src/utils/insultes/dico_insultes_fr.txt";
	BufferedReader br = null;

	public InsultesUtils() throws IOException {
		try{
			br = new BufferedReader(new FileReader(path));
		} catch(FileNotFoundException exc) { throw new FileNotFoundException("File not found" );  }
	}

	public boolean isAnInsulte(String mot) throws IOException{
		if(br==null) throw new FileNotFoundException("File not found" );

		String line = null; 

		try {
			while ((line = br.readLine()) != null)
			{
				if ( line.equalsIgnoreCase(mot) ){
					return true;
				}
			}
		} catch(IOException ioe) { throw new IOException("Erreur IO" ); }
		return false;
	}

	public void close(){
		if(br!=null){
			try {
				br.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public static void main(String[] args) throws IOException {
		InsultesUtils ins_utils = new InsultesUtils();
		System.out.println(ins_utils.isAnInsulte("pute"));
		System.out.println(ins_utils.isAnInsulte("nique"));
		System.out.println(ins_utils.isAnInsulte("con"));
		System.out.println(ins_utils.isAnInsulte("connard"));
		System.out.println(ins_utils.isAnInsulte("imbecile"));
		System.out.println(ins_utils.isAnInsulte("put"));
	}

}
