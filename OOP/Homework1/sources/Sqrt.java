

public class Sqrt extends ProcessTypes {

	public Sqrt() {
		super();
	}

	public int process(int n) {
		double a = Math.sqrt(Math.abs(n));
		double parteFract = a % 1;
		double parteIntreaga = a - parteFract;
		return (int) parteIntreaga;
	}

}
