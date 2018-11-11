/*
An implementation of the CCDP algorithm as introduced in "Cache-Conscious Data Placement" by Calder, Krintz, John and Austin
The implementation only includes parts of CCDP that are needed for data packing
*/
/*
Input:	 call with the command-line parameters  m (cache size) and p (packing factor)
put the sequence R in ref.txt
Output: Writes the data placement scheme to stdout, one line per block
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <set>
#include <deque>
using namespace std;



vector<string> R;
int p,m;

void read_R()
{
	ifstream fin("ref.txt");
	string x;
	while(fin>>x)
		R.push_back(x);
}

struct graph
{
	set<string> vertices;
	set<string> popular_vertices;
	map<string, set<string>> adj; //adjacency list
	map<pair<string, string>, int> edge_w; //weight of edges
	map<string, int> vertex_w; //weight of vertices
	int totalVertexWeight = 0;
	void add_vertex(string v)
	{
		vertices.insert(v);
	}
	void add_edge(string u, string v)
	{
		adj[u].insert(v);
		adj[v].insert(u);
	}
	void increment_weight(string u, string v)
	{
		if(u>v)
			swap(u, v);
		pair<string, string> e = make_pair(u, v);
		edge_w[e]++;
		vertex_w[u]++;
		vertex_w[v]++;
		totalVertexWeight += 2;
	}
	void divide_based_on_popularity()
	{
		vector<pair<int,string>> weight_vertex;
		for(auto v:vertices)
			weight_vertex.push_back(make_pair(vertex_w[v], v));
		sort(weight_vertex.begin(), weight_vertex.end());
		int totalPopularWeight = 0;
		for(int i=weight_vertex.size()-1;i>=0;--i)
		{
			string v = weight_vertex[i].second;
			int w = weight_vertex[i].first;
			popular_vertices.insert(v);
			totalPopularWeight += w;
			if(totalPopularWeight > 99 * totalVertexWeight / 100) // 99% of the weight should belong to popular vertices
				break;
		}
	}
};

graph TRGplace;

void make_TRGplace()
{
	for(int i=0;i<R.size();++i)
		TRGplace.add_vertex(R[i]);
	int Qthreshold = 2 * m; //twice the size of the data cache
	deque<string> Q; //the queue
	for(int Ri=0;Ri<R.size();++Ri)
	{
		string obj = R[Ri];
		
		//find the index of obj
		int objIndex = -1;
		for(int i=0;i<Q.size();++i)
			if(Q[i]==obj)
			{
				objIndex = i;
				break;
			}
			
		if(objIndex!=-1) //obj is found in the Q
		{
			for(int i=0;i<objIndex;++i) //increment the weight of (obj, X)
			{
				string objx = Q[i];
				TRGplace.add_edge(obj, objx);
				TRGplace.increment_weight(obj, objx);
			}
			Q.erase(Q.begin()+objIndex); //erase obj (to be added in the front of Q later)
		}
		
		//insert obj at the front of Q
		Q.push_front(obj);
		
		//remove extra elements from the back of Q
		while(Q.size()>=Qthreshold)
			Q.pop_back();
	}
}

struct compound_node_structure
{
	map<string, int> block_of;
	vector<set<string>> blocks;
	
	void add_key(string key)
	{
		if(block_of.find(key)==block_of.end()) //if the key is not already added
		{
			set<string> novo; //put it in its own block
			novo.insert(key);
			blocks.push_back(novo);
			block_of[key] = blocks.size() - 1;
		}
	}
	
	void merge_keys(string key1, string key2)
	{
		if(blocksize_of(key1)<blocksize_of(key2)) //key1 has the bigger block
			swap(key1, key2);
		int b1 = block_of[key1];
		int b2 = block_of[key2];
		//merge in block_of
		for(string x:blocks[b2])
			block_of[x] = b1;
		//merge in blocks
		blocks[b1].insert(blocks[b2].begin(), blocks[b2].end());
		blocks[b2].clear();
	}
	
	int blocksize_of(string key)
	{
		return blocks[block_of[key]].size();
	}
	
	/////////////weighted part
	map<int, map<int,int>> weightOutgoing;
	map<pair<int, int> , int> blockEdgeWeight; 
	set<pair<int, pair<int,int>>> weightsFirst; //the same as blockEdgeWeight but with weights coming first
	void addWeight(int b1, int b2, int w)
	{
		//cerr<<"added weight: ("<<b1<<","<<b2<<") = "<<w<<endl;
		if(b1>b2)
			swap(b1, b2);
		blockEdgeWeight[make_pair(b1, b2)] = w;
		weightsFirst.insert(make_pair(w, make_pair(b1, b2)));
		weightOutgoing[b1][b2] = w;
		weightOutgoing[b2][b1] = w;
	}
	void removeWeight(int b1, int b2)
	{
		if(b1>b2)
			swap(b1, b2);
		int w = blockEdgeWeight[make_pair(b1, b2)];
		blockEdgeWeight.erase(make_pair(b1, b2));
		weightsFirst.erase(make_pair(w, make_pair(b1, b2)));
		weightOutgoing[b1].erase(b2);
		weightOutgoing[b2].erase(b1);
	}
	int getWeight(int b1, int b2)
	{
		return weightOutgoing[b1][b2];
	}
	
	void merge_two_blocks(int b1, int b2)
	{
		if(blocks[b1].size()==0 or blocks[b2].size()==0)
			return;
		if(blocks[b1].size()<blocks[b2].size())
			swap(b1, b2);
		//b1 is the bigger block
		
		//merge the blocks themselves
		merge_keys(*blocks[b1].begin(), *blocks[b2].begin());
		
		//correct the weights
		auto L = weightOutgoing[b2];
		for(auto xw: L)
		{
			auto x = xw.first;
			auto w = xw.second;
			if(x==b1 or x==b2)
				continue;
			removeWeight(b2, x);
			auto w1 = getWeight(b1, x);
			removeWeight(b1, x);
			addWeight(b1, x, w+w1);
		}
		
	}
	
	void getWeightsFromTRGselect()
	{
		for(auto u:TRGplace.popular_vertices)
			for(auto v:TRGplace.popular_vertices)
			{
				if(u>v) continue;
				int w = TRGplace.edge_w[make_pair(u, v)];
				addWeight(block_of[u], block_of[v],w);
			}
	}
	
	void mergeBlocksByWeight()
	{
		bool mergeFailed = false;
		while(!mergeFailed)
		{
			mergeFailed = true;
			for(auto iter = weightsFirst.rbegin();iter!=weightsFirst.rend();++iter)
			{
				auto temp = *iter; 
				auto b1 = temp.second.first;
				auto b2 = temp.second.second;
				if(b1==b2) continue;
				if(blocks[b1].size()>0 and blocks[b2].size()>0 and blocks[b1].size()+blocks[b2].size()<=p)
				{
					//cerr<<blocks.size()<<" blocks"<<endl;
					//cerr<<"merging "<<b1<<" "<<b2<<endl;
					merge_two_blocks(b1, b2);
					//cerr<<"merged"<<endl;
					//print_CNS();
					//cerr<<"--------"<<endl;
					mergeFailed = false;
					break;
				}
			}				
		}
	}
	
	void mergeBlocks()
	{
		bool merged = true;
		while(merged)
		{
			merged = false;
			for(int i=0;i<blocks.size();++i)
				if(blocks[i].size()==0)
					continue;
				else
				{
					for(int j=i+1;j<blocks.size();++j)
						if(blocks[j].size()>0 and blocks[i].size()+blocks[j].size()<=p)
						{
							merge_two_blocks(i, j);
							merged = true;
						}
				}
		}
	}
	
	void print_CNS()
{
	for(auto B:blocks)
		if(B.size()!=0)
		{
			for(auto x:B)
				cout<<x<<" ";
			cout<<endl;
		}
}
	
};

compound_node_structure CNS;

void make_CNS()
{
	for(auto v:TRGplace.popular_vertices)
		CNS.add_key(v);
}

void add_unpopular_to_CNS()
{
	for(auto v:TRGplace.vertices)
		if(TRGplace.popular_vertices.find(v)==TRGplace.popular_vertices.end()) //v is unpopular
			CNS.add_key(v);
}



int main(int argc, char** argv)
{
	m = atoi(argv[1]);
	p = atoi(argv[2]);
	read_R();
	//create the temporal relationship graph (TRGplace)
	make_TRGplace();
	//divide the vertices into popular and unpopular
	TRGplace.divide_based_on_popularity();
	//make a compound node for each popular vertex
	make_CNS();
	//In Data Packing, TRGselect is the same as TRGplace[popular_vertices], i.e. the induced graph of TRGplace on popular vertices
	//compute the weights of TRGselect in CNS
	CNS.getWeightsFromTRGselect();
	//merge blocks in order of weight
	CNS.mergeBlocksByWeight();
	//add unpopular vertices
	add_unpopular_to_CNS();
	//merge blocks in an arbitrary order
	CNS.mergeBlocks();
	//print the blocks
	CNS.print_CNS();
	return 0;
}