import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public abstract class Team implements Comparable<Team>{
	String type;
	String teamName;
	String gender;
	int numberOfPLayers;
	List <Player> players;
	double totalScore;
	
	void setTeamType(String type) {
		this.type = type;
	}
	
	String getTeamType() {
		return type;
	}
	
	void setTeamName(String teamName) {
		this.teamName = teamName;
	}
	
	String getTeamName() {
		return teamName;
	}
	
	void setGender(String gender) {
		this.gender = gender;
	}
	
	String getGender() {
		return gender;
	}
	
	void setNumberOfPlayers(int numberOfPlayers) {
		this.numberOfPLayers = numberOfPlayers;
	}
	
	int getNumberOfPlayers() {
		return numberOfPLayers;
	}
	
	double getTeamTotalScore() {
		return totalScore;
	}
	
	void setTotalScore(double totalScore) {
		this.totalScore = totalScore;
	}

	void setPlayers() {
		if(numberOfPLayers != 0) {
			this.players = new ArrayList<Player>(numberOfPLayers);
		}
	}
	
	void display(PrintWriter out) {
		
		out.print("{teamName: " + teamName + ", gender: " + gender + ", numberOfPlayers: " + numberOfPLayers + ", ");
		out.print("players: [");
		
		for(int i = 0; i < numberOfPLayers; i++ ) {
			players.get(i).displayPlayer(out);
			if( i != numberOfPLayers  - 1) {
				out.print(", ");
			}
		}
		
		out.print("]}");
		out.println();
	}
	
	int getSumOfScores() {
		int sum = 0;
		for(int i = 0; i < numberOfPLayers; i++ ) {
			sum += players.get(i).score;
		}
		
		return sum;
	}
	
	@Override
	public int compareTo(Team team) {
		if(  getTeamTotalScore() < team.getTeamTotalScore() ) {
			return 1;
		}
		else if( getTeamTotalScore() > team.getTeamTotalScore() )
			return -1;
		else 
			return 0;
	}
	
}
