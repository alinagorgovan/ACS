
public class Cache {
	protected String processName;
	protected int in;
	protected int out;
	
	
	public Cache() {
		super();
	}

	public Cache(String processName, int in, int out) {

		this.processName = processName;
		this.in = in;
		this.out = out;
	}
	public String getProcessName() {
		return processName;
	}
	public void setProcessName(String processName) {
		this.processName = processName;
	}
	public int getIn() {
		return in;
	}
	public void setIn(int in) {
		this.in = in;
	}
	public int getOut() {
		return out;
	}
	public void setOut(int out) {
		this.out = out;
	}
	
	
	
}
