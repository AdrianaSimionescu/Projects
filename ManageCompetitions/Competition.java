import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Competition implements Subject{
	String type;
	String gender;
	List <Team> teams;
	List <Team> ranking;
	List <Team> initialListOfTeams;
	ArrayList<Observer> observers;
	
	public Competition() {
		observers = new ArrayList<Observer>();
		ranking = new ArrayList<Team>();
		initialListOfTeams = new ArrayList<Team>();
	}
	
	@Override
	public void registerObserver(Observer o) {
		observers.add(o);
	}
	
	@Override
	public void removeObserver(Observer o) {
		int i = observers.indexOf(o);
		if(i >= 0) {
			observers.remove(i);
		}
	}
	
	@Override
	public void notifyObservers() {
		for(int i = 0; i < observers.size(); i++) {
			Observer observer = (Observer)observers.get(i);
			observer.update(ranking);
		}
	}
	
	public void rankingChanged() {
		notifyObservers();
	}
	
	public void updateRanking() {
		Collections.sort(ranking);
		rankingChanged();
	}
	
	public void competition() {
		setScores();
		Collections.sort(teams);
		
		for(int i = 0; i < teams.size(); i++) {
			
			for(int j = 0; j < teams.size(); j ++) {
				if( i != j ) {
					compare(teams.get(i),teams.get(j));
					updateRanking();
				}
			}
		}
	}
	
	public void compare(Team t1, Team t2) {
		double scoreT1 = t1.getTeamTotalScore();
		double scoreT2 = t2.getTeamTotalScore();
 		if( scoreT1 > scoreT2 ) 
 			t1.setTotalScore(scoreT1 + 3);
 		else if( scoreT1 == scoreT2 ) {
 			t1.setTotalScore(scoreT1 + 1);
 			t2.setTotalScore(scoreT2 + 1);
 		}
	}
	
	public int getIndex(String teamName) {
		for(int i = 0; i < ranking.size(); i++) {
			if(ranking.get(i).teamName.equals(teamName))
				return i;
		}
		
		return -1;
	}
	
	public void setType(String type) {
		this.type = type;
	}
	
	public void setGender(String gender) {
		this.gender = gender;
	}
	
	void setTeams() {
		this.teams = new ArrayList<Team>();
	}
	
	void setScores() {
		for(int i = 0;i < teams.size(); i++){
			CompetitionVisitor competitionV = new CompetitionVisitor();
			
			if(type.equals("football")) {
				if(teams.get(i) instanceof FootballTeam) {
					competitionV.visit((FootballTeam)teams.get(i));
				}
			}else if(type.equals("handball")) {
				if(teams.get(i) instanceof HandballTeam) {
					competitionV.visit((HandballTeam)teams.get(i));
				}
			}else if(type.equals("basketball")) {
				if(teams.get(i) instanceof BasketballTeam) {
					competitionV.visit((BasketballTeam)teams.get(i));
				}
			}
		} 
	}
	
	void display(PrintWriter out) {

		for(int i = 0; i < 3; i++) {
			out.println(ranking.get(i).teamName);
		}
		
		for(int i = 0; i < initialListOfTeams.size(); i++) {
			int index = getIndex(initialListOfTeams.get(i).teamName) + 1;
		
			if(index > 0) {
				out.println("Echipa " + initialListOfTeams.get(i).teamName + " a ocupat locul " + index);
			}
		}
	}

}
