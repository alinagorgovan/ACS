
/**
 * LE Operator type
 * @author Alina Gorgovan
 *
 */
public class LeOp extends BaseOp{
	
	/* (non-Javadoc)
	 * @see BaseOp#applyOp(FilterStructure, FeedStructure)
	 */
	@Override
	public boolean applyOp(FilterStructure filter, FeedStructure feed) {
		if(Double.parseDouble(filter.getValue()) >= feed.getValue())
			return true;
		return false;
	}
}
