package Tema1POO;
public class Family extends Passenger {
	
	public int numberOfFamilyMembers;
	public int familyPoints = 0;
	public Person familyMembers[];
	
	public Family() {}
	
	public Family(int numberOfFamilyMembers, Person[] persons, String id, Ticket typeOfTicket,int age, String name, boolean priorityBoarding, boolean specialNeeds,int numberOfPassengers ){
		super(id,typeOfTicket,age,name,priorityBoarding,specialNeeds);
		familyMembers = new Person[numberOfFamilyMembers];
		this.numberOfFamilyMembers = numberOfFamilyMembers;
		createFamily(persons,numberOfPassengers);
	}
	
	public void createFamily(Person persons[], int numberOfPassengers) {
		int count = 0;
		
		for(int i = 0; i < numberOfPassengers; i++) {
			if( persons[i].id.equals(getId()) == true) {
				familyMembers[count] = persons[i];
				familyPoints += persons[i].getPointsPerson();
				count++;
			}
		}
		
	}
	
	public int getAllFamilyPoints() {
		 return  familyPoints + 10 + numberOfFamilyMembers * getPoints();
	}
}
