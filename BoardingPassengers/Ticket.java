package Tema1POO;
public class Ticket {
	private String typeOfTicket;
	
	public Ticket() {}
	
	public Ticket(String typeOfTicket) {
		this.typeOfTicket = typeOfTicket;
	}
	
	public int pointsTicket() {
		if( typeOfTicket.equals("b")) return 35;
		else if( typeOfTicket.equals("p")) return 20;
		else if( typeOfTicket.equals("e")) return 0;
		else return -1;
	}
	
	public void getTypeOfTicket() {
		if(typeOfTicket.equals("b"))
			System.out.println("Business ticket");
		else if(typeOfTicket.equals("p"))
			System.out.println("Premium ticket");
		else System.out.println("Economic ticket");

	}
}
