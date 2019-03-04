
/**
 * This class shows how the Visitor Pattern is implemented and types of node it
 * can visit.
 * 
 * @author Alina Gorgovan
 */
public interface IVisitor {
	/**
	 * @param node
	 *            Operand Node type and contains the filter
	 * @param f
	 *            The feed that is filtered
	 * @return true if the feed passes through the filter
	 */
	boolean visit(OperandNode node, FeedStructure f);

	/**
	 * @param node
	 *            And Node type
	 * @param f
	 *            The feed that is filtered
	 * @return true if both children are true
	 */
	boolean visit(AndNode node, FeedStructure f);

	/**
	 * @param node
	 *            Or Node type
	 * @param f
	 *            The feed that is filtered
	 * @return true if at least one child is true
	 */
	boolean visit(OrNode node, FeedStructure f);
}
