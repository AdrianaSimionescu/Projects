import java.util.ArrayList;
import java.util.List;

public class HandballTeam extends Team implements Visitable,Observer{
	
	List <Team> ranking;
	Subject competition;
	
	public HandballTeam() {}
	
	public HandballTeam(Subject competition) {
		this.competition = competition;
		this.ranking = new ArrayList<Team>();
		competition.registerObserver(this);
	}
	
	public void accept(Visitor visitor) {
		visitor.visit(this);
	}
	
	int getMultiplyOfScores() {
		int multiply = 1;
		for(int i = 0; i < numberOfPLayers; i++ ) {
			multiply *= players.get(i).score;
		}
		
		return multiply;
	}

	@Override
	public void update(List <Team> ranking) {
		this.ranking = ranking;
	}
	
	public void display() {
		System.out.println("Clasamentul echipelor de handbal " + getGender() + " este:");
		
		for(int i = 0; i < ranking.size(); i++) {
			int rank = i + 1;
			System.out.println("Echipa " + getTeamName() + " se afla pe locul " + rank );
		}
	}

}
