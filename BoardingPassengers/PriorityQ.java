package Tema1POO;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class PriorityQ {
	//public static int INF = 100000;
	static int heapSize;
	static Passenger queue[];
	static Passenger copyQueue[];
	static PrintWriter out;
	public PriorityQ() {}
	static int count = 1;
	
	public PriorityQ(int parameters) {
		queue = new Passenger[2 * parameters + 1];
		copyQueue = new Passenger[ 2 * parameters + 1];
	}

	public void extractMax() {
		    queue[1] = queue[heapSize--];
		    maxHeapify(1);
	}
	
	//function to get right child of a node of a tree
	public static int getRightChild( int index ) {
		return (2 * index) + 1;
	}
	
	//function to get the parent of a node of a tree
	public static int getParent( int index ) {
		return index / 2;
	}

	//function to get left child of a node of a tree
	public static int getLeftChild( int index ) {
		return (2 * index);
	 }
	
	 // Returns true of given node is leaf 
    public static boolean isLeaf(int index) 
    { 
        if(index > (heapSize / 2) && index <= heapSize) { 
            return true; 
        } 
        
        return false; 
    }
  
    public void swap(int parentIndex, int childIndex) 
    { 
        Passenger tmp; 
        tmp = queue[parentIndex]; 
        queue[parentIndex] = queue[childIndex]; 
        queue[childIndex] = tmp; 
    } 

    public void maxHeapify(int index) 
    { 
        if(isLeaf(index)) {
        	return;
        } 
  
        if( queue[index].points < queue[getLeftChild(index)].points ||  queue[index].points < queue[getRightChild(index)].points ) { 
        	
            if( queue[getLeftChild(index)].points >= queue[getRightChild(index)].points) { 
                swap(index, getLeftChild(index)); 
                maxHeapify(getLeftChild(index)); 
            } 
            else { 
                swap(index, getRightChild(index)); 
                maxHeapify(getRightChild(index)); 
            } 
        } 
    } 
  
    // Inserts a new element to max heap 
    public void insert(Passenger passenger, int key) 
    { 
    	heapSize++;
        queue[heapSize] = passenger; 
  
        // Traverse up and fix violated property 
        int current = heapSize; 
        while( current > 1 && queue[current].points > queue[getParent(current)].points ){ 
            swap(current, getParent(current)); 
            current = getParent(current); 
        } 
    } 
    
    public void listPreOrder(int index ) {
  
    	if( index > heapSize ) return;
    	
    	if( index > 1 && index <= heapSize ) {
    		if( count + 1 == heapSize ) {
    			out.print( queue[index].id );
    		}
    		else{
    			count++;
    			out.print( queue[index].id + " ");
    		}
    	
    	}
    	
    	listPreOrder(2 * index);
    	listPreOrder(2 * index + 1);
    }
    
	 public void list( ) {
		count = 1;
		out.print( queue[1].id + " ");
		listPreOrder(1);
		out.println();
	 }
		    
	 public void closefile() {
		out.close();
	 }
		    
	 public void openfile() {
		try {
			 	out = new PrintWriter("queue.out");
			} catch (FileNotFoundException e) {
				System.out.println("File not found!");
			}
	 	}

}
