package protocole;

public class ProtocoleCreator {

	public static String create(Protocole prot, String... args){
		if(prot != null){
			String message = prot + "/";
			if(args != null && args.length>0){
				for(String arg : args){
					message += arg + "/";
				}
			}
			return message;
		}
		return "";
	}
}
