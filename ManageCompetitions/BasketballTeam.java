import java.util.List;

public class BasketballTeam extends Team implements Visitable,Observer{
	
	List <Team> ranking;
	Subject competition;
	
	public BasketballTeam() {}
	
	public BasketballTeam(Subject competition) {
		this.competition = competition;
		competition.registerObserver(this);
	}
	
	public void accept(Visitor visitor) {
		visitor.visit(this);
	}

	@Override
	public void update(List <Team> ranking) {
		this.ranking = ranking;
	}
	
	public void display() {
		System.out.println("Clasamentul echipelor de baschet " + getGender() + " este:");
		
		for(int i = 0; i < ranking.size(); i++) {
			int rank = i + 1;
			System.out.println("Echipa " + getTeamName() + " se afla pe locul " + rank );
		}
	}
}
