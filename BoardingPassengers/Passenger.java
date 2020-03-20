package Tema1POO;
public class Passenger extends Person{
	
		public String id;
		public Ticket typeOfTicket;
		int points;
		
		public Passenger() {}
		
		public Passenger( String id,  Ticket typeOfTicket, int age, String name, boolean priorityBoarding, boolean specialNeeds) {
			super(name,age,typeOfTicket,id,priorityBoarding,specialNeeds);
			this.id = id;
			this.typeOfTicket = typeOfTicket;
		}
		
		
		public void display() {
			System.out.println("Pasagerul: " + getName() +" din "+ getId() + " cu varsta "+ getAge() + " : ");
			typeOfTicket.getTypeOfTicket();
			benefits.getBenefits();
		}
		
		public int getAllPoints() {
			return pointsAge() + typeOfTicket.pointsTicket() + benefits.pointsPriorityBoarding() + benefits.pointsSpecialNeeds();
		}
		
		public int getPoints() {
			return typeOfTicket.pointsTicket() + benefits.pointsPriorityBoarding();
		}

		public void setPoints(int points) {
			this.points = points; 
		}
		
}
