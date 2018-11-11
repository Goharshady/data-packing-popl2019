import java.util.Vector;

//models an edge or a hyperedge, possibly weighted
public class Edge {
    public Vector<String> end_vertices = new Vector<>();
    int weight = 1;

    void addVertex(String v)
    {
        end_vertices.add(v);
    }

    void increaseWeight()
    {
        weight++;
    }

    void setWeight(int w)
    {
        weight = w;
    }

    boolean containsVertex(String v)
    {
        return end_vertices.contains(v);
    }

    public String toString()
    {
        String ans = "<";
        for(String v:end_vertices)
            ans+=v+",";
        ans+="weight="+Integer.toString(weight);
        ans+=">";
        return ans;
    }
}
