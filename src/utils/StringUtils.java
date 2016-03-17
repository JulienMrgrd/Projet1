package utils;

public class StringUtils {
	
	private StringUtils(){}

	public static String deleteCommaIfExists(String str){
		if(str != null && str.endsWith(",")) return str.substring(0, str.length()-1);
		else return str;
	}
}
