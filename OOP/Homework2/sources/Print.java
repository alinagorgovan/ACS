import java.util.Locale;
import java.util.Map.Entry;

/**
 * This class contains the two methods used for printing an observer.
 * 
 * @author Alina Gorgovan
 *
 */
public class Print {
	/**
	 * This method is used for observers that have the filter different from nil.
	 * Firstly, a visitor is created. Then, for every entry in the treemap of the
	 * observer the filter tree is visited. If the visitor returns "true" it means
	 * that the feed passed the tree filter and it is printed. The number of changes
	 * and the past value are updated for the feeds that passes the filter.
	 * 
	 * @param o
	 *            the observer that is printed
	 */
	public static void print(ObserverStructure o) {

		Visitor visitor = new Visitor();
		for (Entry<String, FeedStructure> entry : o.getFeedtree().entrySet()) {
			FeedStructure f = entry.getValue();
			boolean k = false;
			if (o.getTree() instanceof OperandNode) {
				k = visitor.visit((OperandNode) o.getTree(), f);
			} else if (o.getTree() instanceof AndNode) {
				k = visitor.visit((AndNode) o.getTree(), f);
			} else if (o.getTree() instanceof OrNode) {
				k = visitor.visit((OrNode) o.getTree(), f);
			}

			if (k == true) {
				double increase = 0.00;
				if (f.getPast_value() != 0.00)
					increase = (-1 + f.getValue() / f.getPast_value()) * 100;
				System.out.println(
						"obs " + o.getId() + ": " + f.getName() + " " + String.format(Locale.US, "%.2f", f.getValue())
								+ " " + String.format(Locale.US, "%.2f", increase) + "% " + f.nr_changes);

				f.nr_changes = 0;
				f.setPast_value(f.getValue());
			}
		}
	}

	/**
	 * This method is used for observers that have the filter equal with nil. For
	 * every entry in the treemap the feed is printed. The number of changes and the
	 * past value are updated for all feeds.
	 * 
	 * @param o
	 *            the observer that is printed
	 */
	public static void printAll(ObserverStructure o) {

		for (Entry<String, FeedStructure> entry : o.getFeedtree().entrySet()) {
			FeedStructure f = entry.getValue();
			double increase = 0.00;
			if (f.getPast_value() != 0.00)
				increase = (-1 + f.getValue() / f.getPast_value()) * 100;
			System.out.println(
					"obs " + o.getId() + ": " + f.getName() + " " + String.format(Locale.US, "%.2f", f.getValue()) + " "
							+ String.format(Locale.US, "%.2f", increase) + "% " + f.nr_changes);
			f.nr_changes = 0;
			f.setPast_value(f.getValue());
		}

	}
}
