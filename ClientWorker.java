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
	public ArrayList<String> movieName = new ArrayList<String>();			// to store the
	private ArrayList<String> movieListCount = new ArrayList<String>();
	Semaphore mutex1 = new Semaphore(1);						// to prevent the movie count
	BufferedReader in = null;							// to take input from client
	PrintWriter out = null;								// to send to client

	ClientWorker(Socket client, ArrayList<String> movieListCount) {
		this.client = client;
		this.movieListCount = movieListCount;					// passes movie count
	}


	public void run() {
		String line = "";							// to get the movie and count from client and store in
		String flag = "";							// to send the status of movie as avaible or not
		String movieNameClient = "";						// to get movie name
		String ticketCountClient = "";						// to get count
		// client
		String serverClientFlag = "";						// to signal the cleint it can exit
		String cofirmationFlag = "";						// to see whether client choose

		try {
			// read response from client
			in = new BufferedReader(new InputStreamReader(client.getInputStream()));
			// give back response to client
			out = new PrintWriter(client.getOutputStream(), true);
		} catch (IOException e) {
			System.out.println("in or out failed");
		}
		String clientServerFlag = "";						// to check if the C option is printed by
		// cancel/confirmed
		try {
			// array to store all the movie names
			movieName = movieSelect();
			// send movie name to client
			ObjectOutputStream objectOutput = new ObjectOutputStream(client.getOutputStream());
			objectOutput.writeObject(movieName);
		} catch (IOException e) {
			System.out.println("Write array exception " + e);
		}
		// get value of movie name and number of tickets from cleint
		try {
			clientServerFlag = in.readLine();
		} catch (IOException e1) {
			System.out.println("read from client exception " + e1);
		}
		// to check if option c is pressed before aany option
		if(clientServerFlag != null){
		if (clientServerFlag.equalsIgnoreCase("false")) {
			out.println("server");
		} else {
			if (clientServerFlag.equalsIgnoreCase("true")) {
				try {
					// to get movie name from client, with count
					line = in.readLine();
				} catch (IOException e) {
					System.out.println("read exception in fetching movie name "+ e);
				}
				try {
					// if data is not null, than processthe count for that movie
					if (line != null) {
						String[] getCleintData = line.split("yyyxe");
						movieNameClient = getCleintData[0];				// get movie name
						ticketCountClient = getCleintData[1];				// get count
						// movie count-number of ticket
						flag = checkTicketsAvailability(movieNameClient,ticketCountClient);
						// send the status whether movie is avaialbe or not
						out.println(flag);
					}
					try {
						// to see whether client entered confirm/cancel
						cofirmationFlag = in.readLine();
						if (cofirmationFlag.equalsIgnoreCase("cancel")) {
							// if it preseed cancel
							// function to increment the decremented count again
							cancelCount(movieNameClient, ticketCountClient);
						}

					} catch (Exception e3) {
						 System.out.println("Exception in cancelaation");
					}
				} catch (Exception e) {
					System.out.println("write failed in setting flag " + e);
				}
				try {
					// to check if c option is entered from client
					clientServerFlag = in.readLine();
					if(clientServerFlag != null){
						if (clientServerFlag.equalsIgnoreCase("false")) {
							out.println("server");
						}
					}
				} catch (IOException e) {
				System.out.println("Error occured in client " + e);
				}
			}
		}
		}
		try {
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
						movieName.add(str);// the the data from text file to list
					}
				}
			} else {
				System.out.println("Please enter valid input file name. ");// error
			}
		} catch (Exception e) {
			System.out.println("File not found. ");
		} finally {
			try {
				br.close();
			} catch (IOException e) {
				System.out.println("Exception occured in movieSelect()" + e);
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
				movieCounts1 = movieCounts1 + ticketCount1;// add back the count

				// store back this updated count to arraylist
				this.movieListCount.set(count1, movieCounts1 + "");

			}

		}
	}
}

