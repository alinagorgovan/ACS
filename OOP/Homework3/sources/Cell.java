
/**
 * This class represents a cell in the maze.
 * 
 * @author Alina Gorgovan
 *
 */
public abstract class Cell {
	/**
	 * The x-coordinate of the cell in the maze.
	 */
	private int x;
	/**
	 * The y-coordinate of the cell in the maze.
	 */
	private int y;
	/**
	 * The number of visits in the current cell.
	 */
	private int visitCount = 0;
	/**
	 * The direction of the hero in the current cell.
	 */
	private char direction;
	/**
	 * The parent of the cell in the shortest path tree.
	 */
	private Cell parent;
	/**
	 * The distance between start position and current cell in the maze.
	 */
	private int cost;
	/**
	 * True if the cell is visited and false otherwise.
	 */
	private boolean visited = false;

	/**
	 * Constructs a cell with the given coordinates.
	 * 
	 * @param x
	 *            the x coordinate of the new cell
	 * @param y
	 *            the y coordinate of the new cell
	 */
	public Cell(int x, int y) {
		super();
		this.x = x;
		this.y = y;
	}

	/**
	 * @return the parent of the current Cell.
	 */
	public Cell getParent() {
		return parent;
	}

	/**
	 * Sets the parent of cell.
	 * 
	 * @param parent
	 *            the parent of this cell
	 */
	public void setParent(Cell parent) {
		this.parent = parent;
	}

	/**
	 * @return the x coordinate of the cell.
	 */
	public int getX() {
		return x;
	}

	/**
	 * @return the y coordinate of the cell.
	 */
	public int getY() {
		return y;
	}

	/**
	 * @return the direction of the hero while he is in the current cell.
	 */
	public char getDirection() {
		return direction;
	}

	/**
	 * Updates the direction of the hero in the cell.
	 * 
	 * @param direction the value of the direction
	 */
	public void setDirection(char direction) {
		this.direction = direction;
	}

	/**
	 * Increments the number of visits in the cell.
	 */
	public void addCount() {
		this.visitCount++;
	}

	/**
	 * @return visitCount-the number of visits in the cell.
	 */
	public int getVisitCount() {
		return visitCount;
	}

	/**
	 * @return true if the cell has been visited and false otherwise.
	 */
	public boolean isVisited() {
		return visited;
	}

	/**
	 * The visited field is updated.
	 * 
	 * @param visited
	 *            true if the cell was visited and false otherwise.
	 */
	public void setVisited(boolean visited) {
		this.visited = visited;
	}

	/**
	 * @return The cost of the cell.
	 */
	public int getCost() {
		return cost;
	}

	/**
	 * Updates the cost of the cell.
	 * 
	 * @param cost
	 *            the calculated cost for this cell.
	 */
	public void setCost(int cost) {
		this.cost = cost;
	}

}
