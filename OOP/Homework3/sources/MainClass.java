import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;

public class MainClass {
	public static void main(String[] args) throws IOException {

		BufferedReader buffer = new BufferedReader(new FileReader(args[1]));
		PrintWriter writer = new PrintWriter(args[2], "UTF-8");
		
		Maze maze = new Maze();
		maze.buildMaze(buffer);
		maze.solveMaze(args[0], writer);

		buffer.close();
		writer.close();
	}
}
