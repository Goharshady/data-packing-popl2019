import nl.uu.cs.treewidth.algorithm.GreedyFillIn;
import nl.uu.cs.treewidth.algorithm.MaximumMinimumDegreePlusLeastC;
import nl.uu.cs.treewidth.algorithm.PermutationToTreeDecomposition;
import nl.uu.cs.treewidth.algorithm.QuickBB;
import nl.uu.cs.treewidth.input.DgfReader;
import nl.uu.cs.treewidth.input.GraphInput;
import nl.uu.cs.treewidth.input.InputException;
import nl.uu.cs.treewidth.ngraph.NGraph;
import nl.uu.cs.treewidth.ngraph.NTDBag;
import nl.uu.cs.treewidth.ngraph.NVertex;
import nl.uu.cs.treewidth.ngraph.NVertexOrder;

import java.io.*;
import java.util.*;

//Contains all the functionality for creating tree decompositions and nice tree decompositions, relies on libTW
public class TWTools {

    //converts all hyperedges to cliques, effectively outputting the primal graph
    static Vector<Edge> convertEdgesToClique(Vector<Edge> E)
    {
        Vector<Edge> ans = new Vector<>();
        Set<String> edges = new HashSet<String>();

        for(Edge e: E)
            for(String u:e.end_vertices)
                for(String v:e.end_vertices)
                    if(u.compareTo(v) < 0)
                        edges.add(u+","+v);

        for(String s:edges)
        {
            Edge e = new Edge();
            List<String> vertices = Arrays.asList(s.split(","));
            e.addVertex(vertices.get(0));
            e.addVertex(vertices.get(1));
            ans.add(e);
        }

        return ans;
    }

    //writes the graph in DGF format in a temporary file
    public static void printInDGF(Set<String> V, Vector<Edge> E) throws IOException{
        BufferedWriter writer = null;
        //create a temporary file
        File logFile = new File("temporaryGraph.dgf");

        writer = new BufferedWriter(new FileWriter(logFile));
        Vector<Edge> edges = convertEdgesToClique(E); //replace every hyperedge in E with a clique over its vertices

        //write every edge
        for(Edge e : edges){
            String v1 = e.end_vertices.elementAt(0);
            String v2 = e.end_vertices.elementAt(1);
            String s = "e " + v1 + " " + v2 + "\n";
            writer.write(s); //print this edge in DGF
        }
        writer.close();
    }

    // Makes a tree decomposition based on the dgf file
    static Bag makeTreeDecomposition() throws InputException {

        GraphInput input = new DgfReader("temporaryGraph.dgf");
        NGraph<GraphInput.InputData> g = null;
        g = input.get();


        MaximumMinimumDegreePlusLeastC<GraphInput.InputData> lbAlgo = new MaximumMinimumDegreePlusLeastC<GraphInput.InputData>();
        lbAlgo.setInput(g);
        lbAlgo.run();
        int lowerbound = lbAlgo.getLowerBound();

        GreedyFillIn<GraphInput.InputData> ubAlgo = new GreedyFillIn<GraphInput.InputData>();
        ubAlgo.setInput(g);
        ubAlgo.run();
        int upperbound = ubAlgo.getUpperBound();

        // the tree width
        System.out.println("the tree width <= " + upperbound);


        NVertexOrder<GraphInput.InputData> permutation = null;

        if (lowerbound == upperbound) {
            permutation = ubAlgo.getPermutation();
        } else {
            QuickBB<GraphInput.InputData> qbbAlgo = new QuickBB<GraphInput.InputData>();
            qbbAlgo.setInput(g);
            qbbAlgo.run();
            permutation = qbbAlgo.getPermutation();
        }

        PermutationToTreeDecomposition<GraphInput.InputData> convertor = new PermutationToTreeDecomposition<GraphInput.InputData>(permutation);
        convertor.setInput(g);
        convertor.run();
        NGraph<NTDBag<GraphInput.InputData>> decomposition = convertor.getDecomposition();    // the tree decomposition

        // converts from NGraph to Bag
        Bag root = convertDecomposition(decomposition);
        return root;

    }

    // converts the tree decomposition in libTW format to tree decomposition in our format
    static Bag convertDecomposition(NGraph<NTDBag<GraphInput.InputData>> decomposition){


        Iterator<NVertex<NTDBag<GraphInput.InputData>>> tdBags = decomposition.getVertices();  // bags in the libTW tree decomposition
        Set<Bag> allBags = new HashSet<>();   // bags in our format
        HashMap<NVertex<NTDBag<GraphInput.InputData>>,Bag> bagMap = new HashMap<>();  //maps bags in the libTW tree decomposition to the corresponding bag in our format
        HashMap<Bag,HashSet<Bag>> bagNeighbors = new HashMap<>();

        // converting bags
        while(tdBags.hasNext()){
            NVertex<NTDBag<GraphInput.InputData>> tdBag = tdBags.next();
            Set<NVertex<GraphInput.InputData>> tdBagVertices = tdBag.data.vertices;
            Bag bag = new Bag();
            for(NVertex<GraphInput.InputData> tdBagVertex : tdBagVertices){
                bag.addVertex(tdBagVertex.data.name);
            }
            allBags.add(bag);
            bagMap.put(tdBag,bag);
        }

        // converting tree decomposition edges
        tdBags = decomposition.getVertices();
        while(tdBags.hasNext()){
            NVertex<NTDBag<GraphInput.InputData>> tdBag = tdBags.next();
            Iterator<NVertex<NTDBag<GraphInput.InputData>>> neighbors = tdBag.getNeighbors();
            Bag bag = bagMap.get(tdBag);
            HashSet<Bag> thisBagNeighbors = new HashSet<>();
            while(neighbors.hasNext()){
                NVertex<NTDBag<GraphInput.InputData>> neighbor = neighbors.next();
                Bag neighborBag = bagMap.get(neighbor);
                thisBagNeighbors.add(neighborBag);
            }
            bagNeighbors.put(bag,thisBagNeighbors);
        }


        Bag root = allBags.iterator().next();

        // performing DFS to find the parents and children
        HashSet<Bag> visitedBags = new HashSet<>();
        DFS(root,bagNeighbors,visitedBags);

        return root;
    }

    static void DFS(Bag bag, HashMap<Bag,HashSet<Bag>> bagNeighbors, HashSet<Bag> visitedBags){
        visitedBags.add(bag);
        for(Bag neighbor:bagNeighbors.get(bag)){
            if(!(visitedBags.contains(neighbor))){
                bag.addChild(neighbor);
                neighbor.setParent(bag);
                DFS(neighbor, bagNeighbors, visitedBags);
            }
        }
    }

    // prints the tree decomposition rooted at bag
    static void printTreeDecomposition(Bag bag){

        System.out.println("*****************************");
        System.out.println(bag);
        System.out.println("Vertices = " + bag.vertices);
        System.out.println("Parent = " + bag.parent);
        System.out.println("Children = " + bag.children);
        String bagType = bag.bagType;
        System.out.println("Bag type = " + bagType);

        if(bagType.equals("IE")){
            System.out.println("The bag is introducing the edge : " + bag.introducedEdge);
        }

        System.out.println("*****************************");

        for(Bag childBag:bag.children){
            printTreeDecomposition(childBag);
        }
    }

    //makes a nice tree decomposition and returns its root
    static Bag makeNiceTreeDecomposition(Bag root, Vector<Edge> E){
        fixVertices(root); // makes sure that all the leaves are empty bags and each bag differs with its parent in at most one vertex
        fixNumberOfChildren(root); //makes sure each bag has at most two children and if it has exactly two children, its vertices must be equal to them
        
        //in order to add the introduce-edge bags, we need to locate the bags where the
        //edge can be added. To do this, we order the bags in a pre-order manner
        //Given a vertex v, let B[v] be the root bag of v, i.e. the root of the subtree that
        //contains v. All vertices of every edge appear somewhere in the same bag, so if 
        //u and v are in the same edge, then B[u] is an ancestor or descendant of B[v]
        //we find the indices of all such B[u]'s and take the maximum, we then insert the edge there
                
        Map<Bag, Integer> preorder = new HashMap<>();
        Map<String, Bag> rootBag = new HashMap<>();
        preorderTraverse(root, preorder, rootBag); //fills the preorder and rootBag
        
        root = addIntroduceEdgeBags(E, preorder, rootBag, root); //adds the introduce edges
        
        fillBagTypes(root); //fills the bagType field in all the bags
        
        root = removeRedundancy(root, root); //removes the redundant bags
        
        return root;
    }
    
    //removes all the redundant bags in the subtree of "bag", removes the root in the new tree (possibly above bag)
    static Bag removeRedundancy(Bag bag, Bag root)
    {
    	//recursive call
    	for(Bag child:bag.children)
    		root = removeRedundancy(child, root);
    	
    	//see if this bag should be removed
    	if(bag.bagType.equals("redundant"))
    	{
    		Bag parent = bag.parent;
    		Bag child = bag.children.firstElement();
    		child.setParent(parent);
    		if(parent!=null)
    		{
    			parent.removeChild(bag);
    			parent.addChild(child);
    		}
    		
    		//if this was the root, the child is now the root
    		if(parent==null) //this bag was root
    			return child;
    	}
    	return root;
    }
    
  //fills the bagType field in all the bags in the subtree of "bag"
    static void fillBagTypes(Bag bag)
    {
    	if(bag.bagType.equals("IE"))
    	{
    		//it is already known, don't do anything
    	}
    	else
    	{
    		//find the bag type
    		if(bag.children.size() == 0) //this is a leaf bag
    			bag.bagType = "L";
    		else if(bag.children.size() == 2) //this is a join bag
    			bag.bagType = "J";
    		else if(bag.children.size() == 1) //it is introduce vertex, forget vertex or redundant
    		{
    			for(Bag child:bag.children) //there is only one child
    			{
    				Set<String> addedVertices = setMinus(bag.vertices, child.vertices);
    				if(addedVertices.size() == 1) //this is an introduce vertex bag
    				{
    					bag.bagType = "IV";
    					for(String vertex:addedVertices) //there is only one such vertex
    						bag.introducedVertex = vertex;
    				}
    				else
    				{
    					Set<String> removedVertices = setMinus(child.vertices, bag.vertices);
    					if(removedVertices.size() == 1) //this is a forget vertex bag
    					{
    						bag.bagType = "FV";
    						for(String vertex:removedVertices) //there is only one such vertex
    							bag.forgottenVertex = vertex;
    					}
    					else
    						bag.bagType = "redundant";
    				}
    			}
    		}
    	}
    	
    	//call it recursively on the children
    	for(Bag child:bag.children)
    		fillBagTypes(child);
    }
    
    //adds introduce-edge bags for every edge, returns the new root
    static Bag addIntroduceEdgeBags(Vector<Edge> E, Map<Bag, Integer> preorder, Map<String, Bag> rootBag, Bag root)
    {
    	//add every edge right above its vertices' highest-numbered root bag
    	for(Edge e:E)
    	{
    		Bag placeToInsert = null;
    		for(String v:e.end_vertices)
    		{
    			Bag rootOfV = rootBag.get(v);
    			if(placeToInsert==null || preorder.get(placeToInsert) < preorder.get(rootOfV))
    				placeToInsert = rootOfV;
    		}
    		
    		
    		//add the introduce edge bag right above place to insert
    		//create a new bag
    		Bag newBag = new Bag();
    		newBag.vertices = new HashSet<>(placeToInsert.vertices);
    		newBag.bagType = "IE";
    		newBag.introducedEdge = e;
    		
    		//connect it in between placeToInsert and its parent
    		newBag.setParent(placeToInsert.parent);
    		newBag.addChild(placeToInsert);
    		if(placeToInsert.parent!=null)
    		{
    			placeToInsert.parent.removeChild(placeToInsert);
    			placeToInsert.parent.addChild(newBag);
    		}
    		placeToInsert.parent = newBag;
    		if(placeToInsert == root)
    			root = newBag;
    		
    		
    	}//end of for edge e
    	
    	return root;
    }
    
    //traverses the tree rooted at "bag" in preorder, numbers every bag, finds the root bag of every vertex
    static void preorderTraverse(Bag bag, Map<Bag, Integer> preorder, Map<String, Bag> rootBag)
    {
    	//number the current bag
    	int index = preorder.size() + 1;
    	preorder.put(bag, index);
    	
    	//for every vertex in this bag, check if this is the root bag
    	for(String vertex:bag.vertices)
    		if(rootBag.containsKey(vertex) == false) //has not been seen before
    		{
    			rootBag.put(vertex, bag);    			
    		}
    	
    	//recursively call the same function on the children
    	for(Bag child:bag.children)
    		preorderTraverse(child, preorder, rootBag);
    }
    
  //makes sure each bag below the given bag has at most two children, and if it has exactly two children, then they have the same vertices as this bag
    static void fixNumberOfChildren(Bag bag)
    {	
    	//fix this one
    	if(bag.children.size()>2)
    	{
    		//create two copies
    		Bag copy1 = new Bag();
    		Bag copy2 = new Bag();
    		copy1.vertices = new HashSet<>(bag.vertices);
    		copy2.vertices = new HashSet<>(bag.vertices);
    		
    		//set the parent of the copies to bag
    		copy1.setParent(bag);
    		copy2.setParent(bag);
    		
    		//distribute the children between the two copies
    		for(Bag child:bag.children)
    			if(copy1.children.size()<2) //add two of them to copy1
    			{
    				copy1.addChild(child);
    				child.setParent(copy1);
    			}
    			else //add the rest to copy2
    			{
    				copy2.addChild(child);
    				child.setParent(copy2);
    			}
    		
    		//make sure the two copies are the only children
    		bag.children.clear();
    		bag.addChild(copy1);
    		bag.addChild(copy2);
    		
    	} //end of if (more than two children)
    	
    	//if there are two children, they must have the exact same vertex set as "bag"
    	if(bag.children.size() == 2)
    	{
    		HashSet<Bag> childrenCopy = new HashSet<Bag>(bag.children);
    		for(Bag child:childrenCopy)
    			if(child.vertices.size() != bag.vertices.size())
    			{
    				Bag intermediate = new Bag();
    				intermediate.vertices = new HashSet<>(bag.vertices);
    				intermediate.setParent(bag);
    				intermediate.addChild(child);
    				bag.removeChild(child);
    				bag.addChild(intermediate);
    				child.setParent(intermediate);
    			}
    	}
    	
    	//recursively fix the children
    	for(Bag child:bag.children)
    		fixNumberOfChildren(child);
    	
    }

    // makes sure that all the leaves are empty bags and each bag differs with its parent in at most one vertex
    static void fixVertices(Bag bag){
        if(bag.children.size()==0 && bag.vertices.size()>0){
            Bag leaf = new Bag();
            bag.addChild(leaf);
            leaf.setParent(bag);
        }
        HashSet<Bag> childrenCopy = new HashSet<>(bag.children);
        for(Bag child:childrenCopy){
            fixVertices(child);
            addIntermediateBags(bag,child);  // adds the intermediate bags between bag and child
        }
    }

    static Set<String> setMinus(Set<String> first, Set<String> second){
        Set<String> result = new HashSet<>();
        for(String s:first){
            if(!(second.contains(s))){
                result.add(s);
            }
        }
        return result;

    }

    static void addIntermediateBags(Bag parent, Bag child){
        Set<String> parentVertices = parent.vertices;
        Set<String> childVertices = child.vertices;
        Set<String> toBeAdded = setMinus(childVertices,parentVertices);
        Set<String> toBeRemoved = setMinus(parentVertices, childVertices);

        // parent and child differ in at most one vertex
        if(toBeAdded.size() + toBeRemoved.size()<=1){
            return;
        }

        Vector<Bag> intermediateBags = new Vector<>();
        intermediateBags.add(parent);  // The sequence of bags starts with parent and ends with child
        parent.children.remove(child);  // The child is no longer the child of its parent
        Set<String> currentVertices = new HashSet<>(parentVertices);  // The set of vertices in the current bag
        // In order to preserve the tree width, we first remove vertices
        for(String vertex:toBeRemoved){
            currentVertices.remove(vertex);
            Bag newBag = new Bag();
            newBag.vertices = new HashSet<>(currentVertices);
            intermediateBags.add(newBag);
        }

        for(String vertex:toBeAdded){
            currentVertices.add(vertex);
            Bag newBag = new Bag();
            newBag.vertices = new HashSet<>(currentVertices);
            intermediateBags.add(newBag);
        }

        intermediateBags.removeElementAt(intermediateBags.size()-1); // the last element is equal to child
        intermediateBags.add(child);

        //fixing parents and children
        for(int i=1; i<intermediateBags.size(); i++){
            Bag x = intermediateBags.get(i);
            Bag y = intermediateBags.get(i-1);
            x.setParent(y);
            y.addChild(x);
        }

    }
    
    
    //checks whether the nice tree decomposition is valid
    static void checkNiceTreeDecomposition(Bag root, Set<String> V, Vector<Edge> E) throws Exception
    {
    	//First, do a preorder traverse
    	Map<Bag, Integer> preorder = new HashMap<>();
    	Map<String, Bag> rootBag = new HashMap<>();
    	preorderTraverse(root, preorder, rootBag);
    	    	
    	//check that all vertices are present
    	for(String v:V)
    		if(!rootBag.containsKey(v))
    			throw new Exception("The vertex "+v+" does not appear in the Tree Decomposition");
    	
    	//check for extra vertices
    	if(rootBag.size()!=V.size())
    		throw new Exception("Invalid Number of Vertices in the Tree Decomposition, there are some extra vertices!");
    	
    	//check that all edges are introduced exactly once
    	Map<Edge, Integer> numberOfIntroductions = new HashMap<>();
    	countIntroductions(root, numberOfIntroductions);
    	for(Edge e:E)
    		if(!numberOfIntroductions.containsKey(e))
    			throw new Exception("The edge "+e+" is not introduced.");
    		else if(numberOfIntroductions.get(e)!=1)
    			throw new Exception("The edge "+e+" is introduced "+numberOfIntroductions.get(e)+" times.");
    	if(numberOfIntroductions.size()!=E.size())
    		throw new Exception("Extra edges are introduced!");
    	
    	
    	//check that all bags are of the right type and satisfy the conditions of their type
    	recursiveSanityCheck(root);

    }

    //does a sanity check on all the bags in the subtree of "bag", checks that they are of a valid type
    //and that their values matches their type
    //it also checks validity of parent-child relations 
    static void recursiveSanityCheck(Bag bag) throws Exception
    {
    	//recurse on children
    	for(Bag child:bag.children)
    		recursiveSanityCheck(child);
    	
    	//sanity check on this bag
    	if(bag.bagType.equals("IE")) //introduce edge
    	{
    		if(bag.introducedEdge == null)
    			throw new Exception("An introduce edge node, introducing null");
    		if(bag.children.size()!=1)
    			throw new Exception("An introduce edge node with "+bag.children.size()+" children");
    		Bag child = bag.children.firstElement();
    		if(child.vertices.size()!=bag.vertices.size())
    			throw new Exception("An introduce edge node with a different number of vertices than its child");
    		for(String vertex:bag.vertices)
    			if(!child.vertices.contains(vertex))
    				throw new Exception("An introduce edge node with a different set of vertices than its child");
    		if(bag.introducedVertex.length()!=0)
    			throw new Exception("An introduce edge node, introducing a vertex");
    		if(bag.forgottenVertex.length()!=0)
    			throw new Exception("An introduce edge node, forgetting a vertex");
    	}
    	else if(bag.bagType.equals("IV")) //introduce vertex
    	{
    		if(bag.introducedVertex.length()==0)
    			throw new Exception("An introduce vertex node, introducing nothing!");
    		if(bag.children.size()!=1)
    			throw new Exception("An introduce vertex node with "+bag.children.size()+" children");
    		Bag child = bag.children.firstElement();
    		if(bag.vertices.size() != child.vertices.size() + 1)
    			throw new Exception("An introduce vertex node with mismatching number of vertices (wrt its child)");
    		if(!bag.vertices.contains(bag.introducedVertex))
    			throw new Exception("An introduce vertex node, not containing the introduced vertex");
    		for(String vertex:child.vertices)
    			if(!bag.vertices.contains(vertex))
    				throw new Exception("An introduce vertex node, not containing one of the vertices in its child");
    		if(bag.introducedEdge!=null)
    			throw new Exception("An introduce vertex node introducing an edge");
    		if(bag.forgottenVertex.length()>0)
    			throw new Exception("An introduce vertex node, forgetting a vertex");
    	}
    	else if(bag.bagType.equals("FV")) //forget vertex
    	{
    		if(bag.forgottenVertex.length()==0)
    			throw new Exception("A forget vertex node, forgetting nothing!");
    		if(bag.children.size()!=1)
    			throw new Exception("A forget vertex node with "+bag.children.size()+" children");
    		Bag child = bag.children.firstElement();
    		if(child.vertices.size()!=bag.vertices.size()+1)
    			throw new Exception("A forget vertex node with mismatching sizes");
    		if(!child.vertices.contains(bag.forgottenVertex))
    			throw new Exception("A forget vertex node, forgetting a non-existent vertex (wrt its child)");
    		for(String vertex:bag.vertices)
    			if(!child.vertices.contains(vertex))
    				throw new Exception("A forget vertex node that has an extra vertex compared to its child");
    		if(bag.introducedEdge!=null)
    			throw new Exception("A forget vertex node introducing an edge");
    		if(bag.introducedVertex.length()>0)
    			throw new Exception("A forget vertex node introducing a vertex");
    	}
    	else if(bag.bagType.equals("J")) //join
    	{
    		if(bag.children.size() != 2)
    			throw new Exception("A join node with "+bag.children.size()+" children");
    		Bag child1 = bag.children.elementAt(0);
    		Bag child2 = bag.children.elementAt(1);
    		if(bag.vertices.size() != child1.vertices.size())
    			throw new Exception("A join node " + bag + " that has a different number of vertices compared to its first child");
    		if(bag.vertices.size() != child2.vertices.size())
    			throw new Exception("A join node that has a different number of vertices compared to its second child");
    		for(String vertex:bag.vertices)
    			if(!child1.vertices.contains(vertex))
    				throw new Exception("There is a vertex in this join node that is not in its first child");
    			else if(!child2.vertices.contains(vertex))
    				throw new Exception("There is a vertex in this join node that is not in its second child");
    		if(bag.introducedEdge!=null)
    			throw new Exception("A join node introducing an edge");
    		if(bag.introducedVertex.length()>0)
    			throw new Exception("A join node introducing a vertex");
    		if(bag.forgottenVertex.length()>0)
    			throw new Exception("A join node forgetting a vertex");
    	}
    	else if(bag.bagType.equals("L")) //leaf
    	{
    		if(bag.vertices.size()!=0)
    			throw new Exception("Nonempty leaf");
    		if(bag.children.size()!=0)
    			throw new Exception("A leaf with children!");
    		if(bag.introducedEdge!=null)
    			throw new Exception("A leaf introducing an edge");
    		if(bag.introducedVertex.length()>0)
    			throw new Exception("A leaf introducing a vertex");
    		if(bag.forgottenVertex.length()>0)
    			throw new Exception("A leaf forgetting a vertex");
    	}
    	else
    		throw new Exception("Invalid bag type");
    	
    	//check the parent-children relations
    	if(bag.parent!=null && !bag.parent.children.contains(bag))
    		throw new Exception("A bag that is not included in its parent's children list");
    	for(Bag child:bag.children)
    		if(child.parent!=bag)
    			throw new Exception("A child that has a different parent!");
    }
    
    //recursively counts the number of times each edge is introduced in the subtree of "bag"
    static void countIntroductions(Bag bag, Map<Edge, Integer> numberOfIntroductions) throws Exception
    {
    	//check if this bag is introducing an edge
    	if(bag.bagType.equals("IE"))
    	{
    		Edge e = bag.introducedEdge;
    		if(e==null)
    			throw new Exception("An introduce edge bag is introducing null!");
    		if(numberOfIntroductions.containsKey(e))
    			numberOfIntroductions.put(e, numberOfIntroductions.get(e)+1);
    		else
    			numberOfIntroductions.put(e, 1);
    	}
    	
    	//recurse on children
    	for(Bag child:bag.children)
    		countIntroductions(child, numberOfIntroductions);
    }

    static HashMap<Bag,Integer> bagNumberMap = new HashMap<>();
    static void printTreeDecompositionToFile(Bag bag , PrintWriter writer) throws FileNotFoundException, UnsupportedEncodingException {

        if(!bagNumberMap.containsKey(bag)){
            bagNumberMap.put(bag,bagNumberMap.size());
        }
        writer.println("--Bag Number = \n" + bagNumberMap.get(bag));
        writer.println("--Vertices = \n" + bag.vertices);
        writer.println("--Parent = \n" + bagNumberMap.get(bag.parent));
        String bagType = bag.bagType;
        writer.println("--Bag type = \n" + bagType);

        if(bagType.equals("IE")){
            writer.println("--The bag is introducing the edge : \n" + bag.introducedEdge);
        }

        if(bagType.equals("IV")){
            writer.println("--The bag is introducing the vertex : \n" + bag.introducedVertex);
        }

        if(bagType.equals("FV")){
            writer.println("--The bag is forgetting the vertex : \n" + bag.forgottenVertex);
        }


        for(Bag childBag:bag.children){
            printTreeDecompositionToFile(childBag, writer);
        }
    }

}
