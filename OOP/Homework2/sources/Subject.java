/**
 * The interface shows how the subject of the observer pattern works.
 * 
 * @author Alina Gorgovan
 *
 */
public interface Subject {
	/**
	 * Firstly, the observer is searched in the list. Then, for the found observer
	 * it is called the right print method depending on its filter.
	 * 
	 * @param id
	 *            the id of the observer that is printed
	 */
	public void printObserver(int id);

	/**
	 * If the observer has the filter nil then it is just added to the list and its
	 * treemap is filled. Else, the observer's filter tree is also created.
	 * 
	 * @param observer
	 */
	public void registerObserver(ObserverStructure observer);

	/**
	 * After the observer with the right id is found it is removed from the
	 * arraylist.
	 * 
	 * @param id
	 */
	public void removeObserver(int id);

	/**
	 * For all the observers in the list the update method is called with the
	 * parameters name and value.
	 * 
	 * @param name
	 *            name of the feed that has changed
	 * @param value
	 *            value of the feed
	 */
	public void notifyObservers(String name, Double value);

}
