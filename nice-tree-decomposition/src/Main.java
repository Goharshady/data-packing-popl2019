import nl.uu.cs.treewidth.input.InputException;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Scanner;
import java.util.Set;
import java.util.Vector;

public class Main {

    static Vector<String> R = new Vector<>(); //the reference sequence R
    static int N; //the length of R
    static Set<String> V = new HashSet<>(); //the set of vertices of the graph (corresponding to data elements)
    static Vector<Edge> E = new Vector<>();

    //creates all hyperedges of a given order q, based on R, and adds them to E
    static Map<String, Integer> lastIndexOf = new HashMap<>(); //last index in R where a vertex was seen
    static Comparator<String> comparator = new indexComparator(); //does comparison based on lastIndexOf
    static PriorityQueue<String> lastSeenVertices = new PriorityQueue<String>(1, comparator); //a priority queue to find the latest elements seen in R
    static void compileHyperedges(int q)
    {
        for(int i=0;i<N;++i)
        {
            //add the new vertex and update its index
            String Ri = R.elementAt(i); //R[i]

            if(!lastSeenVertices.contains(Ri) && lastSeenVertices.size()>=q){
                lastSeenVertices.poll();
            }

            //add the edge corresponding to R[i]
            PriorityQueue<String> verticesInEi = new PriorityQueue<>(lastSeenVertices);
            Edge Ei = new Edge(); //The edge E[i] corresponding to R[i]
            while(!verticesInEi.isEmpty())
                Ei.addVertex(verticesInEi.poll());

            Ei.addVertex(Ri);

            //add E[i] to our edges
            E.add(Ei);


            lastSeenVertices.remove(Ri);
            lastIndexOf.put(Ri, i);
            lastSeenVertices.add(Ri);

            //make sure there are at most q vertices in the queue
            while(lastSeenVertices.size()>q)
            {
                lastSeenVertices.poll();
            }
        }
    }

    static void compileWeightedEdges() //creates the edges of the access graph
    {
        Map<String, Integer> cnt = new HashMap<>();
        for(int i=1;i<R.size();++i) //find all edges
            if(!R.elementAt(i).equals(R.elementAt(i-1))) //if the two elements are different
            {
                String u = R.elementAt(i);
                String v = R.elementAt(i-1);
                if(u.compareTo(v)>0) //swap them if not in order
                {
                    String temp = v;
                    v = u;
                    u = temp;
                }
                int c = 0;
                String key = u+","+v;
                if(cnt.containsKey(key))
                    c = cnt.get(key);
                c++; //increase the number of times this pair was seen
                cnt.put(key, c);
            }

        //convert cnt to edges and add them to E
        for(String key:cnt.keySet())
        {
            List<String> vertices = Arrays.asList(key.split(","));
            String u = vertices.get(0);
            String v = vertices.get(1);
            Edge e = new Edge();
            e.addVertex(u);
            e.addVertex(v);
            e.setWeight(cnt.get(key));
            E.addElement(e);
        }
    }

    static void compileVertices() //fills the list V of vertices using the data in R
    {
        for(String x:R)
            V.add(x);
    }

    //reads the sequence R of accesses from ref.txt
    static void readR() throws FileNotFoundException
    {
        File refs = new File("ref.txt");
        Scanner cin = new Scanner(refs);
        while(cin.hasNext())
            R.add(cin.next());
        N = R.size();
        cin.close();
    }

    public static void main(String[] args) throws Exception {
        int m = Integer.parseInt(args[0]);
        int p = Integer.parseInt(args[1]);

        readR();

        //find the vertices of the (hyper)graph
        compileVertices();

        //form the edges of the (hyper)graph
        if(m==1)
            compileWeightedEdges();
        else
        {
            int qstar = (m-1) * p + 2; //order of the hypergraph
            compileHyperedges(qstar);
        }

        System.out.println("Formed the following access (hyper)graph:");

        System.out.println("V = ");
        System.out.println(V);

        System.out.println("E = ");
        System.out.println(E);

        //We first convert the graph to the DGF format which can be used by libTW
        TWTools.printInDGF(V, E);

        // Makes a tree decomposition based on the dgf file
        Bag root = TWTools.makeTreeDecomposition();
        //System.out.println("The Tree Decomposition is : ");
        //TWTools.printTreeDecomposition(root);

        // makes the nice tree decomposition
        root = TWTools.makeNiceTreeDecomposition(root, E);
        //System.out.println("The Nice Tree Decomposition is : ");
        //TWTools.printTreeDecomposition(root);
        
        //check the nice tree decomposition 
        TWTools.checkNiceTreeDecomposition(root, V, E);
        System.out.println("Checking Done.");

        // Writes the Nice Tree Decomposition to file
        PrintWriter writer = new PrintWriter("NiceTreeDecomposition.txt");
        TWTools.printTreeDecompositionToFile(root, writer);
        writer.close();
    }

}


