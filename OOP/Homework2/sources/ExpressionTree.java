import java.util.ArrayList;
import java.util.Stack;

/**
 * The class contains one method that generates a tree from the postfix
 * expression of the filter.
 * 
 * @author Alina Gorgovan
 *
 */
public class ExpressionTree {

	/**
	 * This method creates a node for every element in the arraylist and uses a
	 * stack to create the structure of the tree.
	 * 
	 * @param s
	 *            the list of nodes
	 * @return the root of the tree
	 */
	public static Node constructTree(ArrayList<String> s) {
		Stack<Node> stack = new Stack<>();
		AndNode and;
		OrNode or;
		OperandNode op;
		Node n1, n2;

		for (int i = 0; i < s.size(); i++) {
			String current = s.get(i);
			if (!current.equals("||") && !current.equals("&&")) {
				op = new OperandNode(current);
				stack.push(op);
			} else if (current.equals("||")) {
				n1 = stack.pop();
				n2 = stack.pop();

				or = new OrNode(n2, n1);
				stack.push(or);

			} else if (current.equals("&&")) {
				n1 = stack.pop();
				n2 = stack.pop();

				and = new AndNode(n2, n1);
				stack.push(and);
			}
		}
		n1 = stack.peek();
		stack.pop();

		return n1;
	}

}
