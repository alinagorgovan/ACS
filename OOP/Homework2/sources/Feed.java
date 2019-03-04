import java.util.ArrayList;
import java.util.TreeMap;

/**
 * This is the class where is implemented the biggest part of the observer
 * pattern.
 * 
 * @author Alina Gorgovan
 *
 */
public class Feed implements Subject {
	ArrayList<ObserverStructure> observers = new ArrayList<>();
	private TreeMap<String, Double> feedtree = new TreeMap<String, Double>();

	/**
	 * This method adds the new feed in the treemap and then notifies the observers
	 * calling the notifyObservers method.
	 * 
	 * @param name
	 *            feed name
	 * @param value
	 *            feed value
	 */
	public void addFeed(String name, double value) {
		feedtree.put(name, value);
		notifyObservers(name, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Subject#printObserver(int)
	 */
	@Override
	public void printObserver(int id) {
		for (int i = 0; i < observers.size(); i++) {
			if (observers.get(i).getId() == id) {
				ObserverStructure obs = observers.get(i);
				if (obs.getFilter().equals("nil"))
					Print.printAll(obs);

				else
					Print.print(obs);
			}
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Subject#registerObserver(ObserverStructure)
	 */
	@Override
	public void registerObserver(ObserverStructure observer) {
		if (!observer.getFilter().equals("nil")) {
			observer.setTree(
					ExpressionTree.constructTree(ShuntingYard.postfix(ShuntingYard.parseFilter(observer.getFilter()))));

		}
		observer.setFeedtree(feedtree);
		observers.add(observer);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Subject#removeObserver(int)
	 */
	@Override
	public void removeObserver(int id) {
		for (int i = 0; i < observers.size(); i++) {
			if (observers.get(i).getId() == id)
				observers.remove(i);
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Subject#notifyObservers(java.lang.String, java.lang.Double)
	 */
	@Override
	public void notifyObservers(String name, Double value) {
		for (ObserverStructure observer : observers) {
			observer.updateFeed(name, value);
		}
	}
}
