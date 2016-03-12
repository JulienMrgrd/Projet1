package utils;

public class Tuple<X, Y> { 
	public X left; 
	public Y right; 
	
	public Tuple(){}
	
	public Tuple(X x, Y y) { 
		this.left = x; 
		this.right = y; 
	} 
} 