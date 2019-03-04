import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Set;

/**
 * Class for a miner.
 */
public class Miner extends Thread {
	private Integer hashCount;
	private Set<Integer> solved;
	private CommunicationChannel channel;
	private Message roomInfo;

	/**
	 * Creates a {@code Miner} object.
	 * 
	 * @param hashCount number of times that a miner repeats the hash operation when
	 *                  solving a puzzle.
	 * @param solved    set containing the IDs of the solved rooms
	 * @param channel   communication channel between the miners and the wizards
	 */
	public Miner(Integer hashCount, Set<Integer> solved, CommunicationChannel channel) {
		this.hashCount = hashCount;
		this.solved = solved;
		this.channel = channel;
	}

	@Override
	public void run() {
		while (true) {
			// Try to get a message from wizards channel
			roomInfo = channel.getMessageWizardChannel();
			// If no message was found, try again
			if (roomInfo == null) {
				continue;
			}
			// If an EXIT message was received stop the execution
			if (roomInfo.getData().equals("EXIT")) {
				break;
			}
			// Check if the room was solved before
			if (solved.contains(roomInfo.getCurrentRoom())) {
				continue;
			}
			// Hash the string for hashCount times
			String hash = encryptMultipleTimes(roomInfo.getData(), hashCount);
			// Set the new hashed data to the message
			roomInfo.setData(hash);
			// Add the room to the solved set and put it on the miners channel
			solved.add(roomInfo.getCurrentRoom());
			channel.putMessageMinerChannel(roomInfo);
		}
	}

	private static String encryptMultipleTimes(String input, Integer count) {
		String hashed = input;
		for (int i = 0; i < count; ++i) {
			hashed = encryptThisString(hashed);
		}

		return hashed;
	}

	private static String encryptThisString(String input) {
		try {
			MessageDigest md = MessageDigest.getInstance("SHA-256");
			byte[] messageDigest = md.digest(input.getBytes(StandardCharsets.UTF_8));

			// convert to string
			StringBuffer hexString = new StringBuffer();
			for (int i = 0; i < messageDigest.length; i++) {
				String hex = Integer.toHexString(0xff & messageDigest[i]);
				if (hex.length() == 1)
					hexString.append('0');
				hexString.append(hex);
			}
			return hexString.toString();

		} catch (NoSuchAlgorithmException e) {
			throw new RuntimeException(e);
		}
	}
}
