import java.util.HashSet;
import java.util.Set;
import java.util.Vector;

public class Bag {
    Set<String> vertices = new HashSet<>();
    Bag parent = null;
    Vector<Bag> children = new Vector<>();
    String bagType = "";   //IE: Introduce Edge, IV: Introduce Vertex, FV: Forget Vertex, J: Join, L: Leaf
    Edge introducedEdge = null;
    String introducedVertex = "";
    String forgottenVertex = "";

    void addVertex(String vertex){
        vertices.add(vertex);
    }

    void addChild(Bag child){
        if(!(children.contains(child))){
            children.add(child);
        }
    }
    
    void removeChild(Bag child)
    {
    	children.remove(child);
    }

    void setParent(Bag parent){
        this.parent = parent;
    }


}
