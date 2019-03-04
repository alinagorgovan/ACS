
/**
 * NE Operator type
 * 
 * @author Alina Gorgovan
 *
 */
public class NeOp extends BaseOp {

	/*
	 * (non-Javadoc)
	 * 
	 * @see BaseOp#applyOp(FilterStructure, FeedStructure)
	 */
	@Override
	public boolean applyOp(FilterStructure f, FeedStructure feed) {
		return !(f.getValue().equals(Double.toString(feed.getValue())));
	}

	/**
	 * This method is called if the filter is applied on name.
	 * 
	 * @param f
	 * @param feed
	 * @return true if the names are not equal
	 */
	public boolean applyOpName(FilterStructure f, FeedStructure feed) {
		return (!f.getValue().equals(feed.getName()));
	}

}
