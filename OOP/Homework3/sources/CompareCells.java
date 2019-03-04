import java.util.Comparator;

/**
 * This class is a comparator for sorting the cells by the number of visits.
 * 
 * @author Alina Gorgovan
 *
 */
public class CompareCells implements Comparator<Cell> {

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.util.Comparator#compare(java.lang.Object, java.lang.Object)
	 */
	@Override
	public int compare(Cell o1, Cell o2) {
		return o1.getVisitCount() - o2.getVisitCount();
	}

	/**
	 * @param o1 first object to be compared
	 * @param o2 second object to be compared
	 * @return true if the two cells have the same number of visits.
	 */
	public boolean equals(Cell o1, Cell o2) {
		return o1.getVisitCount() == o2.getVisitCount();
	}

}
