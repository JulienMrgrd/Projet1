package server;

import java.util.ArrayList;
import java.util.List;

import other.Mur;

public class Case {
	public int x;
	public int y;
	public List<Mur> pos;

	public Case(int x, int y){
		this.x=x;
		this.y=y;
		pos= new ArrayList<>();
	}
	
	public void addMur(Mur posMur){
		if(!pos.contains(posMur))
			pos.add(posMur);
	}
	
	public List<Mur> getMur(){
		return pos;
	}
}
