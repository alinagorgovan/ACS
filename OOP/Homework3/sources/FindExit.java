import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;

/**
 * In this class are implemented the methods for solving the maze.
 * 
 * @author Alina Gorgovan
 *
 */
public class FindExit {

	/**
	 * This method finds the next move using the Dubluve class and adds every step
	 * from the path in a list. After the path is find the list is printed.
	 * 
	 * @param maze
	 *            The maze where the hero needs to find the exit.
	 * @param start
	 *            The starting position of the hero.
	 * @param writer
	 *            the writer that prints in the output file.
	 */
	public static void task1(Cell[][] maze, Cell start, PrintWriter writer) {

		Cell currentPosition = start;
		Dubluve dubluve = new Dubluve(start);
		ArrayList<Cell> way = new ArrayList<Cell>();
		do {
			way.add(currentPosition);
			currentPosition = dubluve.move(maze);
		} while (!(currentPosition instanceof ExitCell));
		way.add(currentPosition);
		writer.println(way.size());
		for (Cell c : way) {
			writer.println(c.getX() + " " + c.getY());
		}

	}

	/**
	 * This method checks if a Cell can be visited.
	 * 
	 * @param x
	 *            the x coordinate
	 * @param y
	 *            the y coordinate
	 * @param maze
	 *            the maze where the cell is found
	 * @return true if it can be visited and false otherwise.
	 */
	private static boolean isFree(int x, int y, Cell[][] maze) {
		if ((x >= 0 && x < maze.length) && (y >= 0 && y < maze[x].length)
				&& (maze[x][y] instanceof RoadCell || maze[x][y] instanceof ExitCell)) {
			return true;
		}
		return false;
	}

	/**
	 * 
	 * The method implements the Lee's algorithm for finding the shortest road
	 * between two points in the map. Firstly the cost of every cell that can be
	 * visited is calculated and the cell is marked as visited. This is made going
	 * from a cell to all its valid neighbors in the specified order: E N V S. For
	 * every neighbor the parent is set as the current cell. If the exit is found
	 * the loop stops. So, this way the path is obtained by going up from parent to
	 * parent starting with the exit cell.
	 * 
	 * @param maze
	 *            the map of the maze where the hero needs to find the shortest path
	 * @param start
	 *            the start position of the hero
	 * @param exit
	 *            the destination
	 * @param writer
	 *            the writer that prints in the output file.
	 */
	public static void task2(Cell[][] maze, Cell start, Cell exit, PrintWriter writer) {
		Queue<Cell> q = new LinkedList<Cell>();

		start.setVisited(true);

		start.setCost(0);
		q.add(start);

		int[] row = { 0, -1, 0, 1 };
		int[] col = { 1, 0, -1, 0 };

		while (!q.isEmpty()) {
			Cell current = q.remove();

			if (current == exit) {
				break;
			}
			for (int i = 0; i < 4; i++) {
				int x = current.getX() + row[i];
				int y = current.getY() + col[i];
				if (isFree(x, y, maze) && maze[x][y].isVisited() == false) {
					maze[x][y].setVisited(true);
					maze[x][y].setCost(current.getCost() + 1);
					maze[x][y].setParent(current);
					q.add(maze[x][y]);

				}
			}

		}
		ArrayList<Cell> path = new ArrayList<Cell>();
		Cell step = exit;
		while (step != start) {
			path.add(0, step);
			step = step.getParent();
		}
		path.add(0, start);
		writer.println(path.size());
		for (Cell c : path) {
			writer.println(c.getX() + " " + c.getY());
		}

	}
}
