

import java.util.ArrayList;
import java.util.Random;

public class RandomScheduler extends Scheduler {

	public RandomScheduler() {
		super();
	}

	public void scheduleNC(ProblemData p, HomeworkWriter hw) {
		int rand = 0;
		int result = 0;
		for (int i = 0; i < p.getNumbersToBeProcessed().length; i++) {
			int n = p.getNumbersToBeProcessed()[i];
			rand = getRandom(p.getProcesses().length);
			result = checkProcess(p.getProcesses()[rand], n);
			hw.println(n + " " + p.getProcesses()[rand].getType() + " " + result + " " + "Computed");

		}
	}

	public void scheduleCache(ProblemData p, HomeworkWriter hw) {
		ArrayList<LruCacheStructure> lru = new ArrayList<>(p.getCacheCapacity());
		ArrayList<LfuCacheStructure> lfu = new ArrayList<>(p.getCacheCapacity());
		int rand = 0;
		String aux;
		for (int i = 0; i < p.getNumbersToBeProcessed().length; i++) {
			int n = p.getNumbersToBeProcessed()[i];
			rand = getRandom(p.getProcesses().length);
			if(p.getCacheType().equals("LruCache"))
				aux = implementLru(lru, p.getProcesses()[rand], n, p.getCacheCapacity());
			else
				aux = implementLfu(lfu, p.getProcesses()[rand], n, p.getCacheCapacity());
			hw.println(aux);

		}
	}

	public static int getRandom(int length) {
		int rnd = new Random().nextInt(length);
		return rnd;
	}
}
