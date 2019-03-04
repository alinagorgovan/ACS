
import java.util.ArrayList;

public class WeightedScheduler extends Scheduler {

	protected static int gcd2(int a, int b) {
		if (b == 0) {
			return a;
		}
		return gcd2(b, a % b);
	}

	protected static int gcdOfArray(ProcessStructure[] p) {
		int result = p[0].getWeight();
		for (int i = 1; i < p.length; i++) {
			result = gcd2(result, p[i].getWeight());
		}
		return result;
	}

	public void scheduleNC(ProblemData p, HomeworkWriter hw) {
		int sumWeights = 0;
		int result = 0;
		int j = 0;
		int gcd = gcdOfArray(p.getProcesses());
		int[] nr = new int[p.getProcesses().length];

		for (int i = 0; i < p.getProcesses().length; i++) {
			sumWeights += p.getProcesses()[i].getWeight();
		}

		int t = sumWeights / gcd;
		for (int i = 0; i < p.getNumbersToBeProcessed().length; i++) {
			int n = p.getNumbersToBeProcessed()[i];

			for (int i1 = 0; i1 < p.getProcesses().length; i1++) {
				if (nr[i1] < p.getProcesses()[i1].getWeight() / gcd) {
					result = checkProcess(p.getProcesses()[i1], n);
					hw.println(n + " " + p.getProcesses()[i1].getType() + " " + result + " " + "Computed");
					nr[i1]++;
					j++;
					break;
				}
				if (j == t) {
					j = 0;
					for (int i2 = 0; i2 < nr.length; i2++)
						nr[i2] = 0;
				}
			}

		}
	}

	public void scheduleCache(ProblemData p, HomeworkWriter hw) {
		int sumWeights = 0;
		int j = 0;
		int gcd = gcdOfArray(p.getProcesses());
		int[] nr = new int[p.getProcesses().length];

		for (int i = 0; i < p.getProcesses().length; i++) {
			sumWeights += p.getProcesses()[i].getWeight();
		}
		ArrayList<LruCacheStructure> lru = new ArrayList<>(p.getCacheCapacity());
		ArrayList<LfuCacheStructure> lfu = new ArrayList<>(p.getCacheCapacity());
		String aux;
		int t = sumWeights / gcd;
		for (int i = 0; i < p.getNumbersToBeProcessed().length; i++) {
			int n = p.getNumbersToBeProcessed()[i];
			for (int i1 = 0; i1 < p.getProcesses().length; i1++) {
				if (nr[i1] < p.getProcesses()[i1].getWeight() / gcd) {
					if (p.getCacheType().equals("LruCache"))
						aux = implementLru(lru, p.getProcesses()[i1], n, p.getCacheCapacity());
					else
						aux = implementLfu(lfu, p.getProcesses()[i1], n, p.getCacheCapacity());
					hw.println(aux);
					nr[i1]++;
					j++;
					break;
				}
				if (j == t ) {
					j = 0;
					for (int i2 = 0; i2 < nr.length; i2++)
						nr[i2] = 0;
				}
			}
		}
	}
}
