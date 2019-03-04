import java.util.ArrayList;

/**
 * This class represents the hero in the maze.
 * 
 * @author Alina Gorgovan
 *
 */
public class Dubluve {
	/**
	 * The current cell where the hero is.
	 */
	private Cell currentCell;
	/**
	 * The current direction of the hero.
	 */
	private char direction = '^';

	/**
	 * Constructs an instance of this class.
	 * 
	 * @param currentCell the cell where the hero is found at the moment
	 */
	public Dubluve(Cell currentCell) {
		super();
		this.currentCell = currentCell;
	}

	/**
	 * Calculates the right neighbor of the cell with the given coordinates
	 * depending on the direction of the hero. If the cell doesn't have a right
	 * neighbor then it is created a new cell with the wanted coordinates.
	 * 
	 * @param x
	 *            the x coordinate of the cell
	 * @param y
	 *            the y coordinate of the cell
	 * @param maze
	 *            the maze where the neighbor is searched.
	 * @return the right neighbor of the cell.
	 */
	private Cell getRight(int x, int y, Cell[][] maze) {
		if (direction == '^') {
			if (y != maze[x].length - 1)
				return maze[x][y + 1];
			else
				return new EdgeCell(x, y + 1);
		} else if (direction == '>') {
			if (x != maze.length - 1)
				return maze[x + 1][y];
			else
				return new EdgeCell(x + 1, y);
		} else if (direction == '<') {
			if (x != 0)
				return maze[x - 1][y];
			else
				return new EdgeCell(x - 1, y);
		} else {
			if (y != 0)
				return maze[x][y - 1];
			else
				return new EdgeCell(x, y - 1);
		}
	}

	/**
	 * Calculates the left neighbor of the cell with the given coordinates depending
	 * on the direction of the hero. If the cell doesn't have a left neighbor then
	 * it is created a new cell with the wanted coordinates.
	 * 
	 * @param x
	 *            the x coordinate of the cell
	 * @param y
	 *            the y coordinate of the cell
	 * @param maze
	 *            the maze where the neighbor is searched.
	 * @return the left neighbor of the cell.
	 */
	private Cell getLeft(int x, int y, Cell[][] maze) {
		if (direction == '^') {
			if (y != 0)
				return maze[x][y - 1];
			else
				return new EdgeCell(x, y - 1);
		} else if (direction == '>') {
			if (x != 0)
				return maze[x - 1][y];
			else
				return new EdgeCell(x - 1, y);
		} else if (direction == '<') {

			if (x != maze.length - 1)
				return maze[x + 1][y];
			else
				return new EdgeCell(x + 1, y);
		} else {
			if (y != maze[x].length - 1)
				return maze[x][y + 1];
			else
				return new EdgeCell(x, y + 1);
		}
	}

	/**
	 * Calculates the front neighbor of the cell with the given coordinates
	 * depending on the direction of the hero. If the cell doesn't have a front
	 * neighbor then it is created a new cell with the wanted coordinates.
	 * 
	 * @param x
	 *            the x coordinate of the cell
	 * @param y
	 *            the y coordinate of the cell
	 * @param maze
	 *            the maze where the neighbor is searched.
	 * @return the front neighbor of the cell.
	 */
	private Cell getFront(int x, int y, Cell[][] maze) {
		if (direction == '^') {

			if (x != 0)
				return maze[x - 1][y];
			else
				return new EdgeCell(x - 1, y);
		} else if (direction == '>') {
			if (y != maze[x].length - 1)
				return maze[x][y + 1];
			else
				return new EdgeCell(x, y + 1);
		} else if (direction == '<') {
			if (y != 0)
				return maze[x][y - 1];
			else
				return new EdgeCell(x, y - 1);

		} else {
			if (x != maze.length - 1)
				return maze[x + 1][y];
			else
				return new EdgeCell(x + 1, y);
		}
	}

	/**
	 * Calculates the back neighbor of the cell with the given coordinates depending
	 * on the direction of the hero. If the cell doesn't have a back neighbor then
	 * it is created a new cell with the wanted coordinates.
	 * 
	 * @param x
	 *            the x coordinate of the cell
	 * @param y
	 *            the y coordinate of the cell
	 * @param maze
	 *            the maze where the neighbor is searched.
	 * @return the back neighbor of the cell.
	 */
	private Cell getBack(int x, int y, Cell[][] maze) {
		if (direction == '^') {

			if (x != maze.length - 1)
				return maze[x + 1][y];
			else
				return new EdgeCell(x + 1, y);
		} else if (direction == '>') {
			if (y != 0)
				return maze[x][y - 1];
			else
				return new EdgeCell(x, y - 1);
		} else if (direction == '<') {

			if (y != maze[x].length - 1)
				return maze[x][y + 1];
			else
				return new EdgeCell(x, y + 1);

		} else {
			if (x != 0)
				return maze[x - 1][y];
			else
				return new EdgeCell(x - 1, y);
		}
	}

	/**
	 * This method checks if the cell can be visited.
	 * 
	 * @param cell
	 * @throws CannotMoveIntoWallsException
	 * @throws HeroOutOfBoundException
	 */
	private void valid(Cell cell) throws CannotMoveIntoWallsException, HeroOutOfBoundException {
		if (cell instanceof WallCell)
			throw new CannotMoveIntoWallsException();
		if (cell instanceof EdgeCell)
			throw new HeroOutOfBoundException();
	}

	/**
	 * This function finds all the neighbors of the current cell and decides where
	 * the hero is going to make his next move. Firstly, the neighbors are
	 * calculated with the right function and then, depending on the hero's
	 * direction, the next direction for the next move is updated in every neighbor.
	 * The neighbors are added in an arraylist and sorted using the CellComparator.
	 * If the exit is found in the list then that is next move. Else, the first
	 * valid move from the arraylist is the next move. It is updated the current
	 * cell and the direction of the hero and the number of visits in the next cell.
	 * 
	 * @param maze
	 *            the maze where is made the next move
	 * @return the cell where the hero will make his next move
	 */
	public Cell move(Cell[][] maze) {
		int x = currentCell.getX();
		int y = currentCell.getY();

		Cell right = getRight(x, y, maze);
		Cell left = getLeft(x, y, maze);
		Cell front = getFront(x, y, maze);
		front.setDirection(currentCell.getDirection());
		Cell back = getBack(x, y, maze);
		switch (currentCell.getDirection()) {
		case '>':
			right.setDirection('v');
			left.setDirection('^');
			back.setDirection('<');
			break;

		case '<':
			right.setDirection('^');
			left.setDirection('v');
			back.setDirection('>');
			break;
		case '^':
			right.setDirection('>');
			left.setDirection('<');
			back.setDirection('v');
			break;
		case 'v':
			right.setDirection('<');
			left.setDirection('>');
			back.setDirection('^');
		}

		CompareCells comparator = new CompareCells();

		ArrayList<Cell> next = new ArrayList<Cell>();
		try {
			valid(right);
			next.add(right);
		} catch (HeroOutOfBoundException e) {

		} catch (CannotMoveIntoWallsException e) {

		}
		try {
			valid(front);
			next.add(front);
		} catch (HeroOutOfBoundException e) {

		} catch (CannotMoveIntoWallsException e) {

		}
		try {
			valid(left);
			next.add(left);
		} catch (HeroOutOfBoundException e) {

		} catch (CannotMoveIntoWallsException e) {

		}
		try {
			valid(back);
			next.add(back);
		} catch (HeroOutOfBoundException e) {

		} catch (CannotMoveIntoWallsException e) {

		}
		next.sort(comparator);

		boolean k = false;
		for (Cell c : next) {
			if (c instanceof ExitCell) {
				currentCell = c;
				direction = c.getDirection();
				k = true;
				break;
			}
		}
		if (k == false) {

			Cell c = next.get(0);

			c.addCount();
			currentCell = c;
			direction = c.getDirection();

		}
		next.clear();
		return currentCell;
	}

}
