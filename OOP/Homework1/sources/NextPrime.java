public class NextPrime extends ProcessTypes{
	
	public NextPrime() {
		super();
	}

	public int process(int n) {
		if (n < 0) {
			return 2;
		}
		n++;
		for (int i = 2; i < n; i++) {
			if (n % i == 0) {
				n++;
				i = 2;
			} else
				continue;
		}
		return n;
	}
}
