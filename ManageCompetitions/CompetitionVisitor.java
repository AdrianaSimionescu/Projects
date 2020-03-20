public class CompetitionVisitor implements Visitor{
	
	double totalScore;
	
	public void visit(FootballTeam football) {
		totalScore = football.getSumOfScores();
				
		if(football.gender.equals("masculin")) {
			totalScore += football.maxScore();
		}else if (football.gender.equals("feminin")) {
			totalScore += football.minScore();
		}
		
		football.setTotalScore(totalScore);
	}
	
	public void visit(HandballTeam handball) {
		if(handball.gender.equals("masculin")) {
			totalScore = handball.getSumOfScores();
		}else if (handball.gender.equals("feminin")) {
			totalScore = handball.getMultiplyOfScores();
		}
		
		handball.setTotalScore(totalScore);
	}

	public void visit(BasketballTeam basketball) {
		totalScore = (double)basketball.getSumOfScores()/(double)basketball.numberOfPLayers;
		
		basketball.setTotalScore(totalScore);
	}

}
