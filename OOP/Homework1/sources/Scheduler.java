
import java.util.ArrayList;

public abstract class Scheduler {
	protected abstract void scheduleNC(ProblemData p, HomeworkWriter hw);

	protected abstract void scheduleCache(ProblemData p, HomeworkWriter hw);

	protected int checkProcess(ProcessStructure currentProcess, int n) {
		int result = 0;

		if (currentProcess.getType().equals("CheckPrime")) {
			CheckPrime a = new CheckPrime();
			result = a.process(n);
		}
		if (currentProcess.getType().equals("NextPrime")) {
			NextPrime a = new NextPrime();
			result = a.process(n);
		}
		if (currentProcess.getType().equals("Fibonacci")) {
			Fibonacci a = new Fibonacci();
			result = a.process(n);
		}
		if (currentProcess.getType().equals("Sqrt")) {
			Sqrt a = new Sqrt();
			result = a.process(n);
		}
		if (currentProcess.getType().equals("Square")) {
			Square a = new Square();
			result = a.process(n);
		}
		if (currentProcess.getType().equals("Cube")) {
			Cube a = new Cube();
			result = a.process(n);
		}
		if (currentProcess.getType().equals("Factorial")) {
			Factorial a = new Factorial();
			result = a.process(n);
		}
		return result;
	}

	protected ArrayList<LruCacheStructure> addLRU(ArrayList<LruCacheStructure> lru, int capacity, int in, int out,
			String processName) {
		LruCacheStructure l = new LruCacheStructure(processName, in, out);
		if (lru.size() < capacity) {
			lru.add(l);
		} else {
			lru.remove(0);
			lru.add(l);
		}
		return lru;

	}

	protected ArrayList<LruCacheStructure> updateLRU(ArrayList<LruCacheStructure> lru, int k) {
		for (int l = k; l < lru.size() - 1; l++) {
			lru.set(l, lru.get(l + 1));
		}
		lru.set(lru.size() - 1, lru.get(k));

		return lru;
	}

	protected String implementLru(ArrayList<LruCacheStructure> lru, ProcessStructure pr, int n, int capacity) {
		int result = 0;
		boolean k = false;
		String aux = "Computed";
		String str;
		for (int j = 0; j < lru.size(); j++) {
			k = false;
			aux = "Computed";
			if (lru.get(j).getProcessName().equals(pr.getType()) && lru.get(j).getIn() == n) {
				result = lru.get(j).getOut();
				lru = updateLRU(lru, j);
				aux = "FromCache";
				k = true;
				break;
			}

		}
		if (k == false) {
			result = checkProcess(pr, n);
			addLRU(lru, capacity, n, result, pr.getType());
		}

		str = n + " " + pr.getType() + " " + result + " " + aux;
		return str;
	}

	protected int minim(ArrayList<LfuCacheStructure> lfu) {
		// returneaza indexul celei mai putin folosite intrare din lfu
		int min = 0;
		for (int i = 0; i < lfu.size(); i++) {
			if (min > lfu.get(i).freq) {
				min = i;
			}
		}
		return min;
	}

	protected ArrayList<LfuCacheStructure> addLFU(ArrayList<LfuCacheStructure> lfu, int capacity, int in, int out,
			String processName) {
		LfuCacheStructure l = new LfuCacheStructure(processName, in, out);
		l.freq = 1;
		if (lfu.size() < capacity) {
			lfu.add(l);
		} else {
			lfu.remove(minim(lfu));
			lfu.add(l);
		}
		return lfu;
	}

	protected String implementLfu(ArrayList<LfuCacheStructure> lfu, ProcessStructure pr, int n, int capacity) {
		int result = 0;
		boolean k = false;
		String aux = "Computed";
		String str;
		for (int j = 0; j < lfu.size(); j++) {
			k = false;
			if (lfu.get(j).getProcessName().equals(pr.getType()) && lfu.get(j).getIn() == n) {
				result = lfu.get(j).getOut();
				lfu.get(j).freq += 1;
				aux = "FromCache";
				k = true;
				break;
			}
		}
		if (k == false) {
			result = checkProcess(pr, n);
			addLFU(lfu, capacity, n, result, pr.getType());
		}

		str = n + " " + pr.getType() + " " + result + " " + aux;
		return str;
	}

}
