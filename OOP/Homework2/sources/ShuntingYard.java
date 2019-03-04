import java.util.ArrayList;
import java.util.Stack;

/**
 * @author Alina Gorgovan
 *
 */
public class ShuntingYard {
	
	/**
	 * @param filter
	 * @return
	 */
	public static ArrayList<String> parseFilter(String filter) {
		
		
		String[] p;
		ArrayList<String> pf = new ArrayList<String>();
		if(filter.equals("nil")) {
			pf.add(filter);
			return pf;
		}
		filter = filter.replaceAll("\\(", "\\( ");
		filter = filter.replaceAll("\\)", " \\)");
		filter = filter.replaceAll(" +", " ");
		p = filter.split("\\s");
		for (int i = 0; i < p.length; i++) {
			if (p[i].equals("(") || p[i].equals(")") || p[i].equals("&&") || p[i].equals("||")) {
				pf.add(p[i]);
			} else {
				pf.add(p[i] + " " + p[i + 1] + " " + p[i + 2]);
				i += 2;
			}
		}
		return pf;

	}

	/**
	 * @param a
	 * @return
	 */
	public static ArrayList<String> postfix(ArrayList<String> a) {
		Stack<String> stack = new Stack<String>();
		ArrayList<String> queue = new ArrayList<String>();

		for (int i = 0; i < a.size(); i++) {
			String current = a.get(i);
			if (current.equals("(")) {
				stack.push("(");
			} else if (current.equals(")")) {
				while(!stack.peek().equals("(")) {
					queue.add(stack.pop());
				}
				stack.pop();
			} else if (current.equals("||") || current.equals("&&")) {
				if(stack.empty())
					stack.push(current);
				else {
					while(!stack.peek().equals("(")) {
						queue.add(stack.pop());
					}
					stack.push(current);
				}
				
			} else {
				queue.add(current);
			}
		}
		
		while(!stack.empty()) {
			queue.add(stack.pop());
		}
		
		return queue;
	}
}
