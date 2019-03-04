import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * This class represents the maze where the hero needs to find the exit.
 * 
 * @author Alina Gorgovan
 *
 */
public class Maze {

	/**
	 * The maze is represented as a Cell type 2D array.
	 */
	private Cell[][] maze;
	/**
	 * The cell where the hero enters the maze.
	 */
	private Cell start;
	/**
	 * The destination cell where the hero has to arrive after finding the path.
	 */
	private Cell exit;
	/**
	 * The height of the maze.
	 */
	private int height;
	/**
	 * The width of the maze.
	 */
	private int width;

	/**
	 * This method builds the maze from the input file. Firstly, the maze is
	 * initialized with the read height and width. Using the CellFactory, for every
	 * position in the maze is generated a instance of the type of cell it
	 * represents. The start and exit cells are also set in this function when they
	 * are found.
	 * 
	 * @param buffer
	 *            the buffer that reads from the input file.
	 * @return the maze that is build from the input file.
	 * @throws IOException
	 *             Exceptions regarding input file.
	 */
	public Cell[][] buildMaze(BufferedReader buffer) throws IOException {
		String aux = buffer.readLine();
		String[] tok = aux.split(" ");
		height = Integer.parseInt(tok[0]);
		width = Integer.parseInt(tok[1]);
		maze = new Cell[height][width];
		CellFactory factory = new CellFactory();
		for (int i = 0; i < height; i++) {
			aux = buffer.readLine();
			for (int j = 0; j < width; j++) {
				maze[i][j] = factory.getCell(aux.charAt(j), i, j);
				if (aux.charAt(j) == 'I')
					start = maze[i][j];
				if (aux.charAt(j) == 'O')
					exit = maze[i][j];

			}
		}
		start.setDirection('^');
		start.addCount();

		return maze;
	}

	/**
	 * This method invokes the right method for solving the maze depending on the
	 * task number.
	 * 
	 * @param task
	 *            can be "1" or "2".
	 * @param writer
	 *            the PrintWriter instance that is used for displaying the output in
	 *            the output file.
	 */
	public void solveMaze(String task, PrintWriter writer) {
		if (task.equals("1"))
			FindExit.task1(maze, start, writer);
		else
			FindExit.task2(maze, start, exit, writer);
	}

}
