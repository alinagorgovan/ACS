
/**
 * This class is the abstract base class for all operators which allow a feed to
 * be tested by a type of filter.
 * 
 * @author Alina Gorgovan
 *
 */
public abstract class BaseOp {

	/**
	 * This method will return true if the feed passed the applied filter.
	 * 
	 * @param f
	 *            the filter that is applied
	 * @param feed
	 *            the feed that is checked
	 * @return true if the feed respects the filter
	 */
	public abstract boolean applyOp(FilterStructure f, FeedStructure feed);
}
