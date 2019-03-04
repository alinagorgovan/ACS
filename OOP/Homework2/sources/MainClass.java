
import java.util.Scanner;

/**
 * @author Alina Gorgovan
 *
 */
public class MainClass {
	/**
	 * @param args
	 */
	public static void main(String[] args) {

		Scanner s = new Scanner(System.in);
		String aux = new String();
		String[] p;
		ObserverStructure obs = null;
		Feed feed = new Feed();
		do {
			aux = s.nextLine();
			p = aux.split(" ", 2);

			if (p[0].equals("create_obs")) {

				p = p[1].split(" ", 2);
				obs = new ObserverStructure(Integer.parseInt(p[0]), p[1]);
				feed.registerObserver(obs);

			} else if (p[0].equals("feed")) {
				p = p[1].split(" ");
				feed.addFeed(p[0], Double.parseDouble(p[1]));
			} else if (p[0].equals("delete_obs")) {
				feed.removeObserver(Integer.parseInt(p[1]));

			} else if (p[0].equals("print")) {
				int id = Integer.parseInt(p[1]);
				feed.printObserver(id);

			} else if (aux.equals("end")) {
				s.close();
				break;
			}

		} while (s.hasNextLine());
		s.close();
	}
}
