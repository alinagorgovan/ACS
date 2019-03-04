
public class LfuCacheStructure extends Cache{
	public int freq = 0;

	public LfuCacheStructure() {
		super();
	}
	

	public LfuCacheStructure(String processName, int in, int out) {
		super(processName, in, out);
	}


	public int getFreq() {
		return freq;
	}

	public void setFreq(int freq) {
		this.freq = freq;
	}


	@Override
	public String toString() {
		return "LfuCacheStructure [freq=" + freq + ", processName=" + processName + ", in=" + in + ", out=" + out + "]";
	}
	
}
