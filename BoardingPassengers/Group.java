package Tema1POO;
public class Group extends Passenger{
	
	public int numberOfGroupMembers;
	public int groupPoints;
	public static int keepPoints;
	public Person groupMembers[];
	
	
	public Group() {}
	
	public Group(int numberOfGroupMembers, Person persons[],String id, Ticket typeOfTicket, int age, String name, boolean priorityBoarding, boolean specialNeeds, int numberOfPassengers) {
		super(id,typeOfTicket,age,name,priorityBoarding,specialNeeds);
		groupMembers = new Person[numberOfGroupMembers];
		this.numberOfGroupMembers = numberOfGroupMembers;
		createGroup(persons,numberOfPassengers);		
	}
	
	public void createGroup(Person persons[], int numberOfPassengers) {
		int count = 0;
		for(int i = 0; i < numberOfPassengers; i++) {
			if( persons[i].id.equals(getId()) == true) {
				groupMembers[count] = persons[i];
				groupPoints += persons[i].getPointsPerson();
				count++;
			}
		}
		
	}
	
	public int getGroupPoints() {
		return groupPoints + 5 + getPoints() * numberOfGroupMembers;
	}
	
	
}
