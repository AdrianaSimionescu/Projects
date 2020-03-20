import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class CommandCenter {
	
	private static CommandCenter uniqueInstance;
	boolean lastDisplay;
	List <Team> teams;
	int numberOfTeams;
	
	private CommandCenter() {}
	
	public static CommandCenter getInstance() {
		if(uniqueInstance == null) {
			uniqueInstance = new CommandCenter();
		}
		
		return uniqueInstance;
	}
	
	void setDisplayed() {
		lastDisplay = true;
	}
	
	public static void main(String[] args) throws IOException {
		
		BufferedReader reader = null;
		/**
		 * string used to read each line
		 */
		String line = null;
		
		/** 
		 * array of strings used to store all the words from the current input file 
		 */
		String[] parts = null; 

		try { 
			reader = new BufferedReader(new FileReader(args[1]));
			line = reader.readLine();
		} catch (IOException e) {
				System.out.println("Input file not found!");
		}
		
		PrintWriter out = null;
		
		try {
			out = new PrintWriter(new File(args[3]));
		} catch (FileNotFoundException e) {
			System.out.println("Output file not found!");
		}
		
		if(args[0].equals("inscriere")) {
			
			getInstance().addTeam(reader, line, parts, 1,null,out);
			
		}else if( args[0].equals("competitie")) {
			
			getInstance().setTeams();
			
			Competition competition = new Competition();
			getInstance().addTeam(reader, line, parts, 2, competition, out);
			
			reader = null;
			line = null;
			parts = null; 

			try { 
				reader = new BufferedReader(new FileReader(args[2]));
				line = reader.readLine();
			} catch (IOException e) {
					System.out.println("Input file for competition not found!");
			}
			
			int countLines = 0;
			
			while(line != null) {
				parts = line.split(", ");
				
				countLines++;
				
				if(countLines == 1) {
					competition.setType(parts[0]);
					competition.setGender(parts[1]);
					competition.setTeams();
				}else {
					for(int i = 0; i < getInstance().numberOfTeams; i++) {
						if(parts[0].equals(getInstance().teams.get(i).teamName)) {
							if(competition.type.equals(getInstance().teams.get(i).type) && competition.gender.equals(getInstance().teams.get(i).gender)) {
								competition.initialListOfTeams.add(getInstance().teams.get(i));
								competition.teams.add(getInstance().teams.get(i));
								competition.ranking.add(getInstance().teams.get(i));
							}
						}
					}
				}
			
				line = reader.readLine();
			}
			
			competition.competition();
			competition.display( out);
		}
		
		out.close();
	}
	
	public void addTeam(BufferedReader reader,String line, String[] parts,int currentCommand,Subject competition, PrintWriter out) throws IOException {
		
		while(line != null) {
			parts = line.split(", ");
			
			TeamFactory factory = new TeamFactory();
			Team team = null;
			
			if(currentCommand == 1)
				team = factory.createTeam(parts[0]);
			else if(currentCommand == 2)
				team = factory.createTeam(parts[0],competition);
			
			getInstance().setTeam(team, parts[0], parts[1], parts[2], Integer.parseInt(parts[3]));
			
			for(int i = 0; i < Integer.parseInt(parts[3]); i++) {
				line = reader.readLine();
				String[] part = line.split(", ");
				
				team.players.add(new Player(part[0], Integer.parseInt(part[1])));
			}
			
			if(currentCommand == 1)
				team.display(out);
			else if(currentCommand == 2) {
				teams.add(team);
				numberOfTeams++;
			}
			
			line = reader.readLine();
		}	
	}
	
	public void setTeam(Team team,String type, String teamName, String gender, int numberOfPlayers ) {
		team.setTeamType(type);
		team.setTeamName(teamName);
		team.setGender(gender);
		team.setNumberOfPlayers(numberOfPlayers);
		team.setPlayers();
	}
	
	void setTeams() {
		this.teams = new ArrayList<Team>();
	}
}
