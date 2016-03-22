package plateau;

public enum Mur {
	H,
	B,
	G,
	D;

	
	public static boolean isADir(String dir){
		if(dir==null) return false;
		for(Mur d : values()){
			if(d.name().equals(dir)) return true;
		}
		return false;	
	}
	
	public static Mur getADirByName(String dir){
		if(dir==null) return null;
		for(Mur d : values()){
			if(d.name().equals(dir)) return d;
		}
		return null;	
	}
}
