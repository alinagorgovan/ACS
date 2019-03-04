

public class Factorial {

	public Factorial() {
		super();
	}

	public int process(int n) {
		if (n < 0)
			return 0;
		if (n == 0)
			return 1;
		int p = 1;
		for (int i = 1; i <= n; i++) {
			p = p * i;
			p = p % 9973;
		}
		return p;
	}
}
