import java.io.PrintWriter;

public class Player {
	String name;
	int score;
	
	public Player(String name, int score) {
		this.name = name;
		this.score = score;
	}
	
	void displayPlayer(PrintWriter out) {
		out.print("{name: " + name + ", score: " + score + "}");
	}
}
