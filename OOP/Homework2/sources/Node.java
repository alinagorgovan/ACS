
/**
 * This class represents the structure of a node from the filter tree.
 * 
 * @author Alina Gorgovan
 *
 */
public class Node implements IVisitable {
	public Node left;
	public Node right;

	/**
	 * @param left
	 *            the left child of the node
	 * @param right
	 *            the right child of the node
	 */
	public Node(Node left, Node right) {
		super();
		this.left = left;
		this.right = right;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see IVisitable#accept(IVisitor, FeedStructure)
	 */
	@Override
	public boolean accept(IVisitor visitor, FeedStructure f) {
		return false;
	}
}
