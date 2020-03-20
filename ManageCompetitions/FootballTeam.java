import java.util.List;

public class FootballTeam extends Team implements Visitable,Observer{
	
	List <Team> ranking;
	Subject competition;
	
	public FootballTeam() {}
	
	public FootballTeam(Subject competition) {
		this.competition = competition;
		competition.registerObserver(this);
	}
	
	public int maxScore() {
		int maximum = 0;
		
		for(int i = 0; i < numberOfPLayers; i++ ) {
			if(maximum < players.get(i).score)
				maximum = players.get(i).score;
		}
		
		return maximum;
	}
	
	public int minScore() {
		int minimum = maxScore();
		
		for(int i = 0; i < numberOfPLayers; i++ ) {
			if(minimum > players.get(i).score)
				minimum = players.get(i).score;
		}
		
		return minimum;
	}
	
	public void accept(Visitor visitor) {
		visitor.visit(this);
	}
	
	@Override
	public void update(List <Team> ranking) {
		this.ranking = ranking;
	}
	
	public void display() {
		System.out.println("Clasamentul echipelor de fotbal " + getGender() + " este:");
		
		for(int i = 0; i < ranking.size(); i++) {
			int rank = i + 1;
			System.out.println("Echipa " + getTeamName() + " se afla pe locul " + rank );
		}
	}

}
