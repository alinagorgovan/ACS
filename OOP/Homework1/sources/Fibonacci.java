public class Fibonacci extends ProcessTypes {

	public Fibonacci() {
		super();
	}

	public int process(int n) {
		if (n < 0) {
			return -1;
		}
		int aux = 0;
		int n0 = 0;
		int n1 = 1;
		if (n == 0 | n == 1)
			return n;
		else {
			for (int i = 2; i <= n; i++) {
				aux = (n0 + n1) % 9973;
				n0 = n1;
				n1 = aux;
			}
		}
		return aux % 9973;
	}

}
