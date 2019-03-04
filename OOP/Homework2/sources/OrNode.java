
/**
 * The class represents the Or type of node from the filter tree.
 * 
 * @author Alina Gorgovan
 *
 */
public class OrNode extends Node implements IVisitable {

	/**
	 * @param left the left child of the node
	 * @param right the right child of the node
	 */
	public OrNode(Node left, Node right) {
		super(left, right);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Node#accept(IVisitor, FeedStructure)
	 */
	@Override
	public boolean accept(IVisitor visitor, FeedStructure f) {
		return visitor.visit(this, f);
	}
}
