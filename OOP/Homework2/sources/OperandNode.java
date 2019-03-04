
/**
 * The class represents the Operand Node from the filter tree.
 * 
 * @author Alina Gorgovan
 *
 */
class OperandNode extends Node implements IVisitable {

	private FilterStructure filter;

	/**
	 * Constructor that generates the FilterStructure filter from a String
	 * parameter.
	 * 
	 * @param f
	 *            String that contains the filter
	 */
	public OperandNode(String f) {
		super(null, null);
		String[] aux = f.split("[ ]");
		filter = new FilterStructure(aux[0], aux[1], aux[2]);

	}

	/**
	 * @return value of filter
	 */
	public FilterStructure getFilter() {
		return filter;
	}

	/**
	 * @param filter
	 */
	public void setFilter(FilterStructure filter) {
		this.filter = filter;
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

	/**
	 * This method uses the OperatorFactory to create an instance of the operator
	 * from the filter. Then, it calls the right method to apply he filter on the
	 * feed.
	 * 
	 * @param feed
	 *            the feed that is checked
	 * @return true if the feed passes the filter
	 */
	public boolean getValue(FeedStructure feed) {

		BaseOp op = null;
		boolean k = false;
		OperatorFactory opFactory = OperatorFactory.getInstance();
		op = opFactory.getOperator(filter.getType());

		if (filter.getElem().equals("name") && (filter.getType().equals("eq"))) {
			k = ((EqOp) op).applyOpName(filter, feed);
		} else if (filter.getElem().equals("name") && (filter.getType().equals("ne"))) {
			k = ((NeOp) op).applyOpName(filter, feed);
		} else {
			k = op.applyOp(filter, feed);
		}
		return k;
	}

}