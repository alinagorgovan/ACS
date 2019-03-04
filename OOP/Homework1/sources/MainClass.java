public class MainClass {
	public static void checkScheduler(ProblemData data, HomeworkWriter hw) {
		if (data.getSchedulerType().equals("RandomScheduler")) {
			RandomScheduler s = new RandomScheduler();
			if (data.getCacheType().equals("NoCache"))
				s.scheduleNC(data, hw);
			else
				s.scheduleCache(data, hw);
		}
		if (data.getSchedulerType().equals("RoundRobinScheduler")) {
			RoundRobinScheduler s = new RoundRobinScheduler();
			if (data.getCacheType().equals("NoCache"))
				s.scheduleNC(data, hw);
			else
				s.scheduleCache(data, hw);
		}
		if (data.getSchedulerType().equals("WeightedScheduler")) {
			WeightedScheduler s = new WeightedScheduler();
			if (data.getCacheType().equals("NoCache"))
				s.scheduleNC(data, hw);
			else
				s.scheduleCache(data, hw);
		}
	}

	public static void main(String[] args) {
		HomeworkReader reader = new HomeworkReader(args[0]);
		HomeworkWriter hw = new HomeworkWriter(args[1]);
		ProblemData data = reader.readData();
		checkScheduler(data, hw);
		hw.close();
	}

}
