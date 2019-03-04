
/**
 * Singleton-Factory that creates instances of operators type.
 * @author Alina Gorgovan
 *
 */
public class OperatorFactory {

	private static OperatorFactory instance = null;

	/**
	 * @return the instance of the factory
	 */
	public static OperatorFactory getInstance() {
		if (instance == null) {
			instance = new OperatorFactory();
		}
		return instance;

	}

	/**
	 * @param type the type of the operator wanted
	 * @return an instance of the operator
	 */
	public BaseOp getOperator(String type) {
		switch (type) {
		case "eq":
			return new EqOp();
		case "ne":
			return new NeOp();
		case "le":
			return new LeOp();
		case "lt":
			return new LtOp();
		case "ge":
			return new GeOp();
		case "gt":
			return new GtOp();
		}
		
		throw new IllegalArgumentException();
	}
}
