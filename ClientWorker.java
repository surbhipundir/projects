/** @author surbhi singh , University of Texas at Dallas,
 * @description class for server. Communicate with client and send all raw data to client, for GUI
 * **/
import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.Random;
import java.util.concurrent.Semaphore;

//class to do all the communication with client
class ClientWorker implements Runnable {
	private Socket client;// to establish connection
	public ArrayList<String> movieName = new ArrayList<String>();// to store the
																	// name of
																	// movies
	private ArrayList<String> movieListCount = new ArrayList<String>();// to
																		// store
																		// the
																		// movie
																		// count

	Semaphore mutex1 = new Semaphore(1);// to prevent the movie count

	ClientWorker(Socket client, ArrayList<String> movieListCount) {
		this.client = client;
		this.movieListCount = movieListCount;// passes movie count

	}

	BufferedReader in = null;// to take input from client
	PrintWriter out = null;// to send to client

	public void run() {
		String line = "";// to get the movie and count from client and store in
							// this variable
		String flag = "";// to send the status of movie as avaible or not
		String movieNameClient = "";// to get movie name
		String ticketCountClient = "";// to get count

		try {
			// read response from client
			in = new BufferedReader(new InputStreamReader(
					client.getInputStream()));
			// give back response to client
			out = new PrintWriter(client.getOutputStream(), true);
		} catch (IOException e) {
			System.out.println("in or out failed");
		}
		String clientServerFlag = "";// to check if the C option is printed by
										// client
		String serverClientFlag = "";// to signal the cleint it can exit
		String cofirmationFlag = "";// to see whether client choose
									// cancel/confirmed
		try {
			// array to store all the movie names
			movieName = movieSelect();
			// send movie name to client
			ObjectOutputStream objectOutput = new ObjectOutputStream(
					client.getOutputStream());
			objectOutput.writeObject(movieName);
		} catch (IOException e) {
			System.out.println("Write array exception " + e);
		}
		// get value of movie name and number of tickets from cleint
		try {
			clientServerFlag = in.readLine();
			// System.out.println("firsr flag"+clientServerFlag);
		} catch (IOException e1) {
			System.out.println("read from client exception " + e1);
		}
		// to check if option c is pressed before aany option
		if(clientServerFlag != null){
		if (clientServerFlag.equalsIgnoreCase("false")) {
			out.println("server");
		} else {
			//System.out.println("clientServerFlag "+clientServerFlag);
			if (clientServerFlag.equalsIgnoreCase("true")) {
				try {
					// to get movie name from client, with count
					line = in.readLine();
					// System.out.println("movie at server "+line);
				} catch (IOException e) {
					System.out.println("read exception in fetching movie name "
							+ e);
				}
				try {
					// if data is not null, than processthe count for that movie
					if (line != null) {
						// mutex.acquire();
						String[] getCleintData = line.split("yyyxe");

						movieNameClient = getCleintData[0];// get movie name
						ticketCountClient = getCleintData[1];// get count
						flag = checkTicketsAvailability(movieNameClient,
								ticketCountClient);// movie count-number of
													// ticket
						// System.out.println("value of flag "+flag);
						out.println(flag);// send the status whether movie is
											// avaialbe or not

						// mutex.release();
					}
					try {
						// flagnew= 0;
						cofirmationFlag = in.readLine();// to see whether client
														// entered
														// confirm/cancel
						// System.out.println("cofirmationFlag "+cofirmationFlag);
						if (cofirmationFlag.equalsIgnoreCase("cancel")) {// if
																			// it
																			// preseed
																			// cancel
							cancelCount(movieNameClient, ticketCountClient);// function
																			// to
																			// increment
																			// the
																			// decremented
																			// count
																			// again
							// flagnew=1;

						}

					} catch (Exception e3) {
						// System.out.println("Exception in cancelaation");
					}
				} catch (Exception e) {
					System.out.println("write failed in setting flag " + e);
				}
				// mutex1.release();
				try {
					// to check if c option is entered from client
					clientServerFlag = in.readLine();
					if(clientServerFlag != null){
					if (clientServerFlag.equalsIgnoreCase("false")) {
						out.println("server");
					}
					}
					// System.out.println("second flag"+clientServerFlag);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					// e.printStackTrace();
				}
			}
		}
		}
		try {

			/*
			 * in.close(); out.close();
			 */
			client.close();

		} catch (IOException e) {
			System.out.println("Close failed");
			System.exit(-1);
		}
	}

	// extra functions starts from here
	// read from input file and return the name of movie
	public ArrayList<String> movieSelect() {
		BufferedReader br = null;
		// int randomNumber;
		ArrayList<String> movieName = new ArrayList<String>();// to get the
																// movie name
		String fileName = "";
		try {
			// logic to read the text file
			fileName = "movies.txt";
			if (fileName.equals("movies.txt")) {
				br = new BufferedReader(new FileReader("movies.txt"));
				StringBuilder sb = new StringBuilder();
				String line = "";
				while (line != null) {
					sb.append(line);
					sb.append(System.lineSeparator());
					line = br.readLine();
					if (line != null) {
						String str = line.substring(0,
								(line.indexOf("\t") - 1) + 1);
						movieName.add(str);// the the data from text file to
											// list
					}
				}
			} else {
				System.out.println("Please enter valid input file name. ");// error
																			// when
																			// the
																			// file
																			// is
																			// not
																			// present
			}
		} catch (Exception e) {
			System.out.println("File not found. ");
		} finally {
			try {
				br.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				// e.printStackTrace();
			}
		}
		return movieName;
	}

	// function to check if that movie selected by customer is sold out or not
	public String checkTicketsAvailability(String movieName,
			String ticketCountClient) throws Exception {
		int ticketCount = Integer.parseInt(ticketCountClient);
		String flag = "";

		// check if the movie name and the count associated with it is not zero
		int movieCounts;
		// if movie matches the choice
		int count = 0;
		// logic to get the data and store the updated count corresponding to
		// movie name
		for (String name : this.movieListCount) {

			count++;
			if (name.equalsIgnoreCase(movieName)) {

				movieCounts = Integer.parseInt(this.movieListCount.get(count));// get
																				// the
																				// available
																				// count

				mutex1.acquire();
				movieCounts = movieCounts - ticketCount;

				if (movieCounts >= 0) {
					flag = "t";// if ticket available

					// store back this updated count to arraylist
					this.movieListCount.set(count, movieCounts + "");

				} else {
					flag = "f";// if ticket not available

				}
				mutex1.release();
			}

		}
		
		  /*for(int i=0;i<this.movieListCount.size();i++){
			  System.out.println("this.movieListCount     "+  this.movieListCount.get(i));
		  }*/
		return flag;
	}

	// function to increment the count when the cancel is pressed
	public void cancelCount(String movieName, String ticketCountClient) {
		int ticketCount1 = Integer.parseInt(ticketCountClient);

		// check if the movie name and the count associated with it is not zero
		int movieCounts1;
		// if movie matches the choice
		int count1 = 0;
		for (String name : this.movieListCount) {

			count1++;
			if (name.equalsIgnoreCase(movieName)) {

				movieCounts1 = Integer
						.parseInt(this.movieListCount.get(count1));// get the
																	// avaiable
																	// count
				// mutex1.wait();
				movieCounts1 = movieCounts1 + ticketCount1;// add back the count

				// store back this updated count to arraylist
				this.movieListCount.set(count1, movieCounts1 + "");

			}

		}
		/*
		 * for(int i=0;i<this.movieListCount.size();i++)
		 * System.out.println("this.movieListCount     "
		 * +this.movieListCount.get(i));
		 */
		// return flag;
	}
}

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
				/*
				 * for(int i=0;i<movieNameListCount.size();i++)
				 * System.out.println
				 * ("movieNameListCount....... "+movieNameListCount.get(i));
				 */
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
					// System.out.println("value inside movie count() "+movieTicketCount.add(tempStr));
					movieTicketCount.add(tempStr);// store the movie name and
													// count into an arraylist
				}
			}
		} finally {
			br.close();
		}
		return movieTicketCount;

	}

}
