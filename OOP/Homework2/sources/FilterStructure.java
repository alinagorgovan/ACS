
/**
 * The class represents the structure of an filter which has 3 elements: the
 * type of the operator, where it is applied(name or value), and the value.
 * 
 * @author Alina Gorgovan
 *
 */
public class FilterStructure {
	private String type;
	private String elem;
	private String value;

	/**
	 * @param type
	 * @param elem
	 * @param value
	 */
	public FilterStructure(String type, String elem, String value) {
		super();
		this.type = type;
		this.elem = elem;
		this.value = value;
	}

	/**
	 * @return the type of the operator
	 */
	public String getType() {
		return type;
	}

	/**
	 * @param operand
	 */
	public void setType(String operand) {
		this.type = operand;
	}

	/**
	 * @return the part where the filter is applied
	 */
	public String getElem() {
		return elem;
	}

	/**
	 * @param elem
	 */
	public void setElem(String elem) {
		this.elem = elem;
	}

	/**
	 * @return the value of the filter
	 */
	public String getValue() {
		return value;
	}

	/**
	 * @param value
	 */
	public void setValue(String value) {
		this.value = value;
	}

}
