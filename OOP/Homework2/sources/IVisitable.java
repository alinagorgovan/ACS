
/**
 * This interface represents the elements(nodes) from the tree.
 * 
 * @author Alina Gorgovan
 *
 */
public interface IVisitable {
	/**
	 * @param v
	 *            the visitor
	 * @param f
	 *            the feed that is filtered
	 * @return true if the feed is accepted by the filter
	 */
	boolean accept(IVisitor v, FeedStructure f);
}
