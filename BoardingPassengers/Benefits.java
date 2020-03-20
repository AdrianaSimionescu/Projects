package Tema1POO;
public class Benefits {
	private boolean priorityBoarding, specialNeeds;
	
	public Benefits() {}
	
	public Benefits(boolean prioorityBoarding, boolean specialNeeds) {
		this.priorityBoarding = prioorityBoarding;
		this.specialNeeds = specialNeeds;
	}
	
	public void getBenefits() {
		if(priorityBoarding == true && specialNeeds == true)
			System.out.println("The passenger has priority boarding and special needs!");
		else if(specialNeeds == true)
			System.out.println("The passenger has special needs!");
		else if( priorityBoarding == true )
			System.out.println("The passenger has priority boarding!");
		else System.out.println("The passenger has no special needs neither priority boarding!");
	}
	
	public boolean isPriorityBoarding() {
		return priorityBoarding;
	}
	
	public boolean areSpecialNeeds() {
		return specialNeeds;
	}
	
	public int pointsPriorityBoarding() {
		if( priorityBoarding == true ) return 30;
		else return 0;
	}
	
	public int pointsSpecialNeeds() {
		if( specialNeeds == true ) return 100;
		else return 0;
	}
	
}
