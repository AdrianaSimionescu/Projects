package Tema1POO;
public class Single extends Passenger {

	public int points;
	public Person person;
	
	public Single() {}
	
	public Single( Person person, String id, Ticket typeOfTicket, int age, String name, boolean priorityBoarding, boolean specialNeeds ) {
		super(id,typeOfTicket,age,name,priorityBoarding,specialNeeds);
		this.person = person;
	}
	
}
