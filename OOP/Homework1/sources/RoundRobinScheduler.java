
import java.util.ArrayList;

public class RoundRobinScheduler extends Scheduler {

	public void scheduleNC(ProblemData p, HomeworkWriter hw) {
		int result = 0;
		int n;
		int j = 0;

		for (int i = 0; i < p.getNumbersToBeProcessed().length; i++) {
			n = p.getNumbersToBeProcessed()[i];
			j = j % p.getProcesses().length;
			result = checkProcess(p.getProcesses()[j], n);
			hw.println(n + " " + p.getProcesses()[j].getType() + " " + result + " " + "Computed");
			j++;
		}

	}

	public void scheduleCache(ProblemData p, HomeworkWriter hw) {
		int n;
		int j = 0;
		ArrayList<LruCacheStructure> lru = new ArrayList<>(p.getCacheCapacity());
		ArrayList<LfuCacheStructure> lfu = new ArrayList<>(p.getCacheCapacity());
		String aux;

		for (int i = 0; i < p.getNumbersToBeProcessed().length; i++) {
			n = p.getNumbersToBeProcessed()[i];
			j = j % p.getProcesses().length;
			if (p.getCacheType().equals("LruCache"))
				aux = implementLru(lru, p.getProcesses()[j], n, p.getCacheCapacity());
			else
				aux = implementLfu(lfu, p.getProcesses()[j], n, p.getCacheCapacity());
			hw.println(aux);
			j++;
		}
	}

}
