package Tema1POO;
public class Person implements Comparable{
	
		public String id;
		public Ticket typeOfTicket;
		public String name;
		public int age;
		public Benefits benefits;
		
		public Person() {}
		
		public Person(String name, int age,Ticket typeOfTicket,String id, boolean priorityBoarding, boolean specialNeeds) {
			this.name = name;
			this.age = age;
			this.id = id;
			this.typeOfTicket = typeOfTicket;
			this.benefits =  new Benefits(priorityBoarding,specialNeeds);
		}
		
		public int getAge() {
			return age;
		}
		
		public String getName() {
			return name;
		}
		
		public String getId() {
			return id;
		}
		
		public void display() {
			System.out.println("Pasagerul: " + getName() +" din "+ getId() + " cu varsta "+ getAge() + " : ");
			typeOfTicket.getTypeOfTicket();
			benefits.getBenefits();
		}
		
		public int pointsAge() {
			if( age < 2 ) return 20;
			else if( age >= 2 && age < 5 ) return 10; 
			else if( age >= 5 && age < 10 ) return 5;
			else if( age >= 10 && age < 60 ) return 0;
			else return 15;
		}
		
		public int getPointsPerson() {
			return pointsAge() + benefits.pointsSpecialNeeds();
		}
		
		
		public int compareTo(Object o) {
			int dif = getId().compareTo(((Person)o).getId());
			if( dif > 0 )
				return 1;
			else if( dif < 0 )
				return -1;
			else 
				return 0;
		}
		
}
