
/**
 * This class represents the And type of node that can be found in the filter
 * tree and implements the IVisitable interface.
 * 
 * @author Alina Gorgovan
 *
 */
public class AndNode extends Node implements IVisitable {

	/**
	 * Parameterized constructor
	 * @param left the left child of the node
	 * @param right the right child of the node
	 */
	public AndNode(Node left, Node right) {
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
