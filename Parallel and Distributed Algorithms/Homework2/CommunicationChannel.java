import java.util.concurrent.*;
import java.util.ArrayList;
import java.util.Arrays;

/**
 * Class that implements the channel used by wizards and miners to communicate.
 */
public class CommunicationChannel {
	// A buffer where wizards put messages and miners read from
	public static ConcurrentHashMap<Integer, ArrayList<Message>> wizards;
	// A buffer where miners put messages and wizards read
	public static LinkedBlockingQueue<Message> miners;

	/**
	 * Creates a {@code CommunicationChannel} object.
	 */
	public CommunicationChannel() {
		wizards = new ConcurrentHashMap<Integer, ArrayList<Message>>();
		miners = new LinkedBlockingQueue<Message>();
	}

	/**
	 * Puts a message on the miner channel (i.e., where miners write to and wizards
	 * read from).
	 * 
	 * @param message message to be put on the channel
	 */
	public void putMessageMinerChannel(Message message) {
		// Only call add method which is already synchronized
		try {
			miners.put(message);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Gets a message from the miner channel (i.e., where miners write to and
	 * wizards read from).
	 * 
	 * @return message from the miner channel
	 */
	public Message getMessageMinerChannel() {
		// Call take method which is already synchronized
		try {
			return miners.take();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * Puts a message on the wizard channel (i.e., where wizards write to and miners
	 * read from).
	 * 
	 * @param message message to be put on the channel
	 */
	synchronized public void putMessageWizardChannel(Message message) {
		// Get the id of the current wizard thread
		int currentId = (int) Thread.currentThread().getId();

		boolean exists = false;
		// Iterate through miners hashmap and try to add the new message
		// to the right wizard list
		for (int threadId : wizards.keySet()) {
			if (threadId == currentId) {
				exists = true;
				// If the wizard thread was found in the map add the new message
				// if it is not a END message because END messages don't help
				// the miners with any info
				if (!message.getData().equals("END")) {
					wizards.get(currentId).add(message);
				}
				// If EXIT message put twice
				if (message.getData().equals("EXIT")) {
					wizards.get(currentId).add(message);
				}
			}
		}
		// If the thread id was not found in the map keys then
		// create a new entry
		if (!exists) {
			ArrayList<Message> m = new ArrayList<Message>(Arrays.asList(message));
			wizards.put(currentId, m);
		}
	}

	/** 
	 * Gets a message from the wizard channel (i.e., where wizards write to and
	 * miners read from).
	 * 
	 * @return message from the miner channel
	 */
	synchronized public Message getMessageWizardChannel() {
		// Iterate through wizards buffers and search for 
		// an array that has 2 or more elements
		// so that it has at least one parent and one adjacent room
		for (ArrayList<Message> a : wizards.values()) {
			// lock the array so that wizards cannot write while miners read
			synchronized (a) {
				if (a.size() >= 2) {
					Message current = a.remove(0);
					Message next = a.remove(0);
					// set the parent room and return
					next.setParentRoom(current.getCurrentRoom());
					return next;
				}
			}
		}
		// If no messages were found return null
		return null;
	}
}
