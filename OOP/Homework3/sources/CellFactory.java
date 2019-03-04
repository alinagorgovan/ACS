
/**
 * This class represents a Factory Pattern and generates all types of Cells.
 * 
 * @author Alina Gorgovan
 *
 */
public class CellFactory {

	/**
	 * Depending on the type of the cell this method returns an instance of a cell
	 * of that type.
	 * 
	 * @param type
	 *            the type of cell returned.
	 * @param x
	 *            the x coordinate of the returned cell
	 * @param y
	 *            the y coordinate of the returned cell.
	 * @return a new instance of the type of cell given.
	 */
	public Cell getCell(char type, int x, int y) {
		switch (type) {
		case '#':
			return new WallCell(x, y);
		case '.':
			return new RoadCell(x, y);
		case 'I':
			return new RoadCell(x, y);
		case 'O':
			return new ExitCell(x, y);
		}
		return null;
	}
}
