public class TeamFactory {
	
	public Team createTeam(String type) {
		
		Team team = null;
		
		if(type.equals("football")) {
			team = new FootballTeam();
		}else if (type.equals("basketball")) {
			team = new BasketballTeam();
		}else if (type.equals("handball")) {
			team = new HandballTeam();
		}
		
		return team;
		}
	
	public Team createTeam(String type,Subject competition) {
		
		Team team = null;
		
		if(type.equals("football")) {
			team = new FootballTeam();
		}else if (type.equals("basketball")) {
			team = new BasketballTeam();
		}else if (type.equals("handball")) {
			team = new HandballTeam();
		}
		
		return team;
		}
}