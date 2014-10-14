// main class where socket connection is establised
class SocketThrdServer {
	ServerSocket server = null;
	public ArrayList<String> movieNameListCount = new ArrayList<String>();// get
																			// the
																			// count

	// logic when to exit from srver
	public void listenSocket(int port) {
		try {
			server = new ServerSocket(port);
			System.out.println("Server running on port " + port + ","
					+ " use ctrl-C to end");
		} catch (IOException e) {
			System.out.println("Error creating socket");
			System.exit(-1);
		}
		try {
			movieNameListCount = movieCount();
		} catch (Exception e1) {
			System.out.println("Exception in loading intial count");
		}
		while (true) {
			// this to create one thread per client
			ClientWorker w;
			try {
				w = new ClientWorker(server.accept(), movieNameListCount);
				Thread t = new Thread(w);
				t.start();
			} catch (IOException e) {
				System.out.println("Accept failed");
				System.exit(-1);
			}
		}
	}

	// close connection
	protected void finalize() {
		try {
			server.close();
		} catch (IOException e) {
			System.out.println("Could not close socket");
			System.exit(-1);
		}
	}

	// main function of server,
	public static void main(String[] args) {
		if (args.length != 1) {
			System.out.println("Usage: java SocketThrdServer port");
			System.exit(1);
		}

		SocketThrdServer server = new SocketThrdServer();
		// to fetch the given port number enter
		int port = Integer.valueOf(args[0]);// get the port
		server.listenSocket(port);
	}

	// function to return movie count and movie name in arraylist
	public ArrayList<String> movieCount() throws Exception {

		// read the movie name
		BufferedReader br = new BufferedReader(new FileReader("movies.txt"));
		ArrayList<String> movieTicketCount = new ArrayList<String>();

		try {
			StringBuilder sb = new StringBuilder();
			String line = "";
			while (line != null) {
				sb.append(line);
				sb.append(System.lineSeparator());
				line = br.readLine();
				if (line != null) {
					String str = line
							.substring(0, (line.indexOf("\t") - 1) + 1);
					String str1 = line.substring(line.length() - 3);
					String tempStr = str1.replace("\t", " ").trim();
					movieTicketCount.add(str);
					movieTicketCount.add(tempStr);// store the movie name and count into an arraylist
				}
			}
		} finally {
			br.close();
		}
		return movieTicketCount;

	}

}
