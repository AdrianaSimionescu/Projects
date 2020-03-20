import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/**
 * In this class is made the reading of data from the input file, data storage and
 * the implementation of the commands: OBSERVE, TRIGGER HEAT,TEMPERATURE and LIST
 *
 */
public class DataBase{
	/**
	 * represents the number of rooms of the house
	 */
	private int numberOfRooms;
	
	/**
	 * represents the global temperature that the user wants
	 */
	private double globalTemperature;
	
	/**
	 * represents the reference time for the transmitted temperatures
	 */
	private static long timestampReference;
	
	
	/**
	 * this list keeps the evidence of the rooms
	 */
	ArrayList<Room> list = new ArrayList<Room>();
	
	/**
	 * default constructor
	 */
	public DataBase() {}
	
	/**
	 * setter for the number of rooms
	 * @param numberOfRooms represents the number of rooms
	 */
	public void setNumberOfRooms(int numberOfRooms) {
		this.numberOfRooms = numberOfRooms;
	}
	
	/**
	 * getter for the number of rooms
	 * @return the number of rooms
	 */
	public int getNumberOfRooms() {
		return numberOfRooms;
	}
	
	/**
	 * setter for the global temperature
	 * @param globalTemperature represents the global temperature
	 */
	public void setGlobalTemperature(double globalTemperature) {
		this.globalTemperature = globalTemperature;
	}
	
	/**
	 * getter for the global temperature
	 * @return the global temperature
	 */
	public double getGlobalTemperature() {
		return globalTemperature;
	}
	
	/**
	 * setter for the timestamp reference
	 * @param timestampReference represents the timestamp reference
	 */
	public void setTimestampReference(long timestampReference) {
		DataBase.timestampReference = timestampReference;
	}
	
	/**
	 * getter for the timestamp reference
	 * @return the timestamp reference
	 */
	public long getTimestampReference() {
		return timestampReference;
	}
	
	public static void main(String[] args) {
		BufferedReader reader;
		PrintWriter out = null;
		
		DataBase dataBase = new DataBase();
		
		try {
			/**
			 * buffer reader used to read from the input file
			 */
			reader = new BufferedReader(new FileReader("therm.in"));
			
			/**
			 * print writer used to store the results into the output file
			 */
			out = new PrintWriter(new File("therm.out"));
			
			/**
			 * string used to read each line
			 */
			String line = reader.readLine();
			
			/**
			 * counter for the number of input  lines
			 */
			int count = 1; 
			
			/** 
			 * array of strings used to store all the words from the current input file 
			 */
			String[] parts = null; 
			
			while(line != null) {
				
				parts = line.split(" ");
				
				/**
				 * in case of reading the first line of the input file 
				 * the fields regarding the number of rooms, the global temperature and
				 * the timestamp reference are set; the input is formed of strings so 
				 * the converstions to Integer,Double and Long are needed
				 */
				if(count == 1) {
					dataBase.setNumberOfRooms(Integer.parseInt(parts[0]));
					dataBase.setGlobalTemperature(Double.parseDouble(parts[1]));
					dataBase.setTimestampReference(Long.parseLong(parts[2]));
				}else {
					
					/**
					 * if the command is in the form: <room_name> <device_id> <area>
					 * a new object of type 'Room' is created based on the information given 
					 * and is stored in the list of rooms of this class
					 */
					if(parts[0].matches("ROOM[\\d]")) {
						String	name = parts[0];
						String	id = parts[1];
						Double temperature = Double.parseDouble(parts[2]);
						dataBase.list.add(new Room(name,id,temperature,timestampReference));
					}
					
					/**
					 * if the command is in the form: OBSERVE <device_id> <timestamp> <temperature>
					 * firstly is checked if the timestamp from the input file exceeds the timestamp reference,
					 * because the data is verified during a certain interval of time 
					 * then the temperature measured at that timestamp is added in the corresponding room
					 */
					if(parts[0].equals("OBSERVE")) {
						
						String device_id = parts[1];
					
						long timestamp = Long.parseLong(parts[2]);
						double temperature = Double.parseDouble(parts[3]);

						if(timestampReference - timestamp  > 0) {
							
							for(int i = 0; i < dataBase.list.size(); i++) {
								Room currentRoom = dataBase.list.get(i);
								String s = currentRoom.getRoomId();
			
								if(s.equals(device_id)) {
									currentRoom.addTemperatures(timestamp, temperature);
								}
							}
						}
					}
					
					/**
					 * if the command is in the form: LIST <room_name> <start_interval> <stop_interval>
					 * the room that is investigated is searched in the list of rooms and the method 
					 * from the 'Room' class is used to list into the output file the temperatures
					 * recorded in the given interval
					 */
					if(parts[0].equals("LIST")) {
						
						String room_name = parts[1];
						long startOfInterval = Long.parseLong(parts[2]);
						long endOfInterval = Long.parseLong(parts[3]);
						
						for(int i = 0; i < dataBase.list.size(); i++) {
							
							Room currentRoom = dataBase.list.get(i);
							
							if(currentRoom.getRoomName().equals(room_name)) {
								out.print(room_name + " ");
								currentRoom.listTemperatures(startOfInterval, endOfInterval,out);
								out.println();
							}
						}
					}
					
					/**
					 * if the command is in the form: TRIGGER HEAT
					 * a HashMap is used to store the the lowest temperatures recorded in the last hour of 
					 * storing temperatures by every device and every area of the correspondig room
					 * going through the list of rooms is checked if a temperature was added in the 
					 * latest hour interval,if so taking into cosideration the fact that the arrays of temperatures 
					 * are sorted, the lowest temperature is the first one in the array
					 */
					if(parts[0].equals("TRIGGER")) {
						
						HashMap<Double, Double> mapOfMinimumTemperatures = new HashMap<>();
						
						for(int i = 1; i < dataBase.list.size(); i++) {
							
							Room room = dataBase.list.get(i);
							double getMinTemperature = 0;
								
							Map < Long, ArrayList<Double> > map = room.storeTemperatures;
								
							ArrayList<Long> keys = new ArrayList<Long>(map.keySet());
							
							for(int j = 0; j <= keys.size() - 1; j++){
								ArrayList<Double> array = map.get(keys.get(j));
								
								if(array.size() != 0) {
									getMinTemperature = array.get(0);
									break;
								}
							}
							
							mapOfMinimumTemperatures.put(getMinTemperature, room.getRoomArea());
						}
						
						/**
						 * calculate the weighted mean between the lowest temperatures recorded and 
						 * the coresponding areas 
						 */
						double numerator = 0;
				        double denominator = 0;
				        
				        for( Map.Entry <Double, Double> entry : mapOfMinimumTemperatures.entrySet()) {
				            numerator += entry.getKey() * entry.getValue();
				            denominator += entry.getValue();
				        }

				        double weightedMean = 0;
				        
				        /**
				         * catch the exception in case of trying to divide the numerator by zero 
				         */
				        try{
				        	
				        	weightedMean = numerator / denominator;
				    	
				        	 if(weightedMean < dataBase.getGlobalTemperature())
					        		out.println("YES");
					        else out.println("NO");
				        	 
				        }catch(ArithmeticException ex) {
				        	System.out.println("Exception: a number cannot be divided by zero");
				        }
				        
					}
					
					/**
					 * if the command is in the form: TEMPERATURE <global_temperature>
					 * the global temperature is updated
					 */
					if(parts[0].equals("TEMPERATURE")) {
						dataBase.setGlobalTemperature(Double.parseDouble(parts[1]));
					}
				}
				
				/**
				 * move to the next line of the input file
				 */
				line = reader.readLine();
				/**
				 * increase the number of lines read from the input file
				 */
				count++;
			}
			
			/**
			 * close the buffer reader
			 */
			reader.close();
			
		/**
		* catch the exception in case if the one of files (input,output) are not found
		*/
		}catch (IOException e) {
			System.out.println("File not found!");
		}
		
		/**
		 * close the print writer
		 */
		out.close();
		
	}
}
