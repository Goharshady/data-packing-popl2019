import java.util.Comparator;
//used for the priority queue when creating the access hypergraph, orders the vertices according to their last seen time
public class indexComparator implements Comparator<String>
{
    @Override
    public int compare(String x, String y)
    {
        if(!Main.lastIndexOf.containsKey(x) || !Main.lastIndexOf.containsKey(y))
            return 0;
        if(Main.lastIndexOf.get(x) < Main.lastIndexOf.get(y))
            return -1;
        else if(Main.lastIndexOf.get(x) == Main.lastIndexOf.get(y))
            return 0;
        return 1;
    }
}
