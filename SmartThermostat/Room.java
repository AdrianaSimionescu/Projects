import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;

/**
 * This class represents the rooms of the house with all their information and recorded temperatures
 *
 */
public class Room {
	/**
	 * new decimal format used to print the results of type double with a presition of two decimals
	 */
	private static DecimalFormat df2 = new DecimalFormat("#.00");
	
	/**
	 * represents the room name
	 */
	private String room_name;
	
	/**
	 * represents the id of the room
	 */
	private String room_id;
	
	/**
	 * represents the area of the room
	 */
	private double area;	
	
	/**
	 * represents the timestamp reference found in the 'DataBase' class
	 */
	private long timestampReference;
	
	/**
	 * map used to store the recorded temperatures of the room on buckets of 1 hour 
	 * during 24 hours before the timestamp reference;
	 * the keys of the map represent the lower ends of each interval; 
	 * each value of the map corresponding to a key represents 
	 * the arraylist of temperatures added in the interval [lower end + 3600s,lower end] 
	 */
	Map < Long, ArrayList<Double> > storeTemperatures;
	
	/**
	 * default constructor
	 */
	public Room() {}
	
	/**
	 * This constructor creates a room based on the specified information given about it
	 * @param room_name the name of the room
	 * @param room_id the id of the room
	 * @param area the are of the room
	 * @param timestampReference the timestamp reference
	 */
	public Room(String room_name, String room_id, double area, long timestampReference ) {
		setRoomName(room_name);
		setRoomId(room_id);
		setRoomArea(area);
		setTimestampReference(timestampReference);
		
		storeTemperatures = new LinkedHashMap < Long, ArrayList<Double> >();
		
		/**
		 * the intervals (the map's keys) are created : [timeRef - k*3600s, timeRef - (k+1)*3600s] with k from 0 to 23
		 * keeping just the lower ends of them
		 */
		long hoursBack = 1;
		
		while(hoursBack <= 24) {
			long endIntervalValue = timestampReference - hoursBack*3600;
			storeTemperatures.put(endIntervalValue,new ArrayList<Double>());
			hoursBack++;
		}
	}
	
	/**
	 * This method adds a recorded temperature in the corresponding interval of time;
	 * The correct interval for the given timestamp is searched going through every interval;
	 * If the correct interval is found then the temperature is added to the array and the array is being sorted;
	 * In case if the timestamp is at the limit of 2 intervals then the temperature is added in both of the intervals
	 * @param timestamp timestamp corresponding to a given temperature
	 * @param temperature temperature recorded in the given interval
	 */
	public void addTemperatures(long timestamp, double temperature) {
		int hoursBack = 1;
		
		while(hoursBack <= 24) {
			
			long endIntervalValue = timestampReference - hoursBack*3600;
			long startIntervalValue = endIntervalValue + 3600;
			
			if( timestamp > endIntervalValue && timestamp < startIntervalValue ) {
				
				ArrayList<Double> list = storeTemperatures.get(endIntervalValue);
				list.add(temperature);
				Collections.sort(list);
				
				break;
			}else if(timestamp == endIntervalValue) {
				
				ArrayList<Double> list = storeTemperatures.get(endIntervalValue);
				list.add(temperature);
				
				list = storeTemperatures.get(endIntervalValue - 3600);
				list.add(temperature);
				Collections.sort(list);
				
				break;
			}else if(timestamp == startIntervalValue) {
				
				ArrayList<Double> list = storeTemperatures.get(endIntervalValue);
				list.add(temperature);
				
				list = storeTemperatures.get(startIntervalValue);
				list.add(temperature);
				Collections.sort(list);
				
				break;
			}
			hoursBack++;
		}
	}
	
	/**
	 * This method writes the temperatures recorded in a given interval into the output file;
	 * Every arraylist of temperatures recorded between start and end is converted to a set
	 * to clear all the duplicate temperatures 
	 * @param start the start of the interval
	 * @param end the end of the interval
	 * @param out the print writer where the result are stored
	 */
	public void listTemperatures(long start, long end, PrintWriter out) {
		
		while( start < end ) {
			end -= 3600;
			
			ArrayList<Double> list = storeTemperatures.get(end);
			Set <Double> set = new HashSet<>(list);
			list.clear();
			list.addAll(set);
			Collections.sort(list);
			
		    for(int i = 0; i < list.size(); i++) {
		    	if( i == list.size() - 1 && start == end) {
		    		out.print( df2.format(list.get(i)));
		    	}else {
		    		out.print( df2.format(list.get(i)) + " ");
		    	}
		    }
		
		}
	}
	
	/**
	 * getter for the id of the room
	 * @return the id of the room
	 */
	public String getRoomId() {
		return this.room_id;
	}
	
	/**
	 * getter for the id of the room
	 * @param room_id the id of the room
	 */
	public void setRoomId(String room_id) {
		this.room_id = room_id;
	}
	
	/**
	 * getter for the room name
	 * @return the room name
	 */
	public String getRoomName() {
		return this.room_name;
	}
	
	/**
	 * setter for the room name
	 * @param room_name the room name
	 */
	public void setRoomName(String room_name) {
		this.room_name = room_name;
	}
	
	/**
	 * getter for the area of the room
	 * @return the area of the room
	 */
	public double getRoomArea() {
		return this.area;
	}
	
	/**
	 * setter for the area of the room
	 * @param area the area of the room
	 */
	public void setRoomArea(double area) {
		this.area = area;
	}
	
	/**
	 * setter for the timestamp reference
	 * @param timestampReference timestamp reference
	 */
	public void setTimestampReference(long timestampReference) {
		this.timestampReference = timestampReference;
	}
	
}
