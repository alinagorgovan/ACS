
/**
 * The class represents the structure of an element from the feed.
 * 
 * @author Alina Gorgovan
 *
 */
public class FeedStructure {
	private String name;
	private double value;
	public int nr_changes = 0;
	private double past_value = 0.00;

	/**
	 * No parameter constructor
	 */
	public FeedStructure() {
		super();
	}

	/**
	 * Parameterized constructor.
	 * @param name
	 * @param value
	 */
	public FeedStructure(String name, double value) {
		super();
		this.name = name;
		this.value = value;
	}

	/**
	 * @return the past value of the feed
	 */
	public double getPast_value() {
		return past_value;
	}

	/**
	 * @param past_value
	 */
	public void setPast_value(double past_value) {
		this.past_value = past_value;
	}

	/**
	 * @return the name of the feed
	 */
	public String getName() {
		return name;
	}

	/**
	 * @param name
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * @return the value of the feed
	 */
	public double getValue() {
		return value;
	}

	/**
	 * @param value
	 */
	public void setValue(double value) {
		this.value = value;
	}

}
