
import java.util.TreeMap;
import java.util.Map.Entry;

/**
 * This class represents the structure of an observer in the observer pattern.
 * Elements: id, filter that contains a String, the Treemap with feeds and the
 * root of the filter tree.
 * 
 * @author Alina Gorgovan
 *
 */
public class ObserverStructure {
	private int id;
	private String filter;
	private TreeMap<String, FeedStructure> feedtree = new TreeMap<String, FeedStructure>();
	private Node tree;

	/**
	 * Parameterized constructor.
	 * 
	 * @param id
	 * @param filter
	 */
	public ObserverStructure(int id, String filter) {
		super();
		this.id = id;
		this.filter = filter;
	}

	/**
	 * @return the id of the observer
	 */
	public int getId() {
		return id;
	}

	/**
	 * @param id
	 */
	public void setId(int id) {
		this.id = id;
	}

	/**
	 * @return the filter of the observer(String)
	 */
	public String getFilter() {
		return filter;
	}

	/**
	 * @param filter
	 */
	public void setFilter(String filter) {
		this.filter = filter;
	}

	/**
	 * @return the root of the filter tree
	 */
	public Node getTree() {
		return tree;
	}

	/**
	 * @param tree
	 */
	public void setTree(Node tree) {
		this.tree = tree;
	}

	/**
	 * @return the treemap of this observer
	 */
	public TreeMap<String, FeedStructure> getFeedtree() {
		return feedtree;
	}

	/**
	 * The method transforms a tremap of name and values to a treemap of name and
	 * FeedStructure.
	 * 
	 * @param feedmap
	 */
	public void setFeedtree(TreeMap<String, Double> feedmap) {
		for (Entry<String, Double> entry : feedmap.entrySet()) {
			FeedStructure f = new FeedStructure(entry.getKey(), entry.getValue());
			feedtree.put(f.getName(), f);

		}
	}

	/**
	 * If the feed is already contained in the treemap than its value is updated,
	 * else a new entry is added in the treemap. In both cases, the number of
	 * changes is incremented.
	 * 
	 * @param name
	 *            the name of the feed
	 * @param value
	 *            the value of the feed
	 */
	public void updateFeed(String name, Double value) {
		if (feedtree.containsKey(name)) {
			feedtree.get(name).setValue(value);
		} else {
			feedtree.put(name, new FeedStructure(name, value));
		}
		feedtree.get(name).nr_changes++;
	}

}
