
/**
 * The class implements the IVisitor interface and overrides its methods.
 * 
 * @author Alina Gorgovan
 */
public class Visitor implements IVisitor {
	/*
	 * (non-Javadoc)
	 * 
	 * @see IVisitor#visit(OrNode, FeedStructure)
	 */
	@Override
	public boolean visit(OrNode node, FeedStructure f) {
		return (node.left.accept(this, f) || node.right.accept(this, f));
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see IVisitor#visit(AndNode, FeedStructure)
	 */
	@Override
	public boolean visit(AndNode node, FeedStructure f) {
		return (node.left.accept(this, f) && node.right.accept(this, f));
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see IVisitor#visit(OperandNode, FeedStructure)
	 */
	@Override
	public boolean visit(OperandNode node, FeedStructure f) {
		return node.getValue(f);
	}

}