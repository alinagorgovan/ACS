
/**
 * GT Operator type
 * @author Alina Gorgovan
 *
 */
public class GtOp extends BaseOp {

	/* (non-Javadoc)
	 * @see BaseOp#applyOp(FilterStructure, FeedStructure)
	 */
	@Override
	public boolean applyOp(FilterStructure filter, FeedStructure feed) {
		return (Double.parseDouble(filter.getValue()) < feed.getValue());
	}
}