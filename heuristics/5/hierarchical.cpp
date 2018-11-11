/*
An implementation of "A hierarchical model of data locality" by Zhang, Ding, Zhong and Wu
*/
/*
Input:	 call with the command-line parameters  m (cache size) and p (packing factor)
put the sequence R in ref.txt
Output: Writes the data placement scheme to stdout, one line per block
Note: we use logarithmic changes on the sampling window size and we set theta to 1/2, as suggested in the paper by Zhang et al, we also use a complete sampling, i.e. a sampling rate of 1
Note 2: We do not ignore any vertices
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <unordered_set>
#include <deque>
#include <unordered_map>
using namespace std;

#define min(x,y) (((x)<(y))?(x):(y))
#define affinity_group unordered_set<string>

vector<string> R;
unordered_set<string> data_elements;
int p,m;

void read_R()
{
	ifstream fin("ref.txt");
	string x;
	while(fin>>x)
	{
		R.push_back(x);
		data_elements.insert(x);
	}
}

vector<affinity_group> groups;
vector<int> parent;
vector<vector<int>> children;


//used in the function below
unordered_map<string, int> S;
map<pair<string,string> , int > P;
unordered_map<string, unordered_set<string>> adj; //adjacency list
unordered_map<string, int> count_in_window;
unordered_set<string> seen;

void add_the_group(affinity_group G)
{
	string elem = *G.begin();
	int par = -1;
	for(int i=groups.size()-1;i>=0;--i)
		if(groups[i].find(elem)!=groups[i].end()) //found the parent
		{
			par = i;
			break;
		}
		
	groups.push_back(G);
	parent.push_back(par);
	vector<int> empty;
	children.push_back(empty);
	children[par].push_back(groups.size()-1);
	
}

void dfs(string root, affinity_group &G)
{
	if(seen.find(root)!=seen.end())
		return;
	G.insert(root);
	seen.insert(root);
	for(auto v:adj[root])
		dfs(v, G);
}

void add_connected_components()
{
	//erase seen
	seen.clear();
	
	affinity_group G;
	for(string x:data_elements)
	{
		if(seen.find(x)!=seen.end())
			continue;
		G.clear();
		dfs(x, G);
		add_the_group(G);
	}
}

void add_groups(int window_size, double theta)
{
	//clear up
	S.clear();
	P.clear();
	adj.clear();
	count_in_window.clear();
	//end of clearing
	
	int j = 0; //j is the end of the window 
	for(int i=0;i<R.size();++i) //i is the beginning of the window -- as is customary in C++, the window is in range [i, j)
	{
		while(count_in_window.size() < window_size and j < R.size())
		{
			string add = R[j];
			count_in_window[add]++;
			j++;
		}
		
		//the window [i, j) is ready for inspection
		//process S[x]'s
		for(auto obj:count_in_window)
		{
			string x = obj.first;
			S[x] += 1;
		}
		
		//process P[x,y]'s
		for(auto obj1:count_in_window)
			for(auto obj2:count_in_window)
			{
				string x = obj1.first;
				string y = obj2.first;
				if(x>=y)
					continue; //do not do it twice
				P[make_pair(x,y)]+=1;
				P[make_pair(y,x)]+=1;
			}
		
		//going to the next window
		string rem = R[i];
		count_in_window[rem] --;
		if(count_in_window[rem]<=0)
			count_in_window.erase(rem);
	}
	
	//compute confidence and the graph
	for(auto x:data_elements)
		for(auto y:data_elements)
			if(x>y and ((long double)P[make_pair(x,y)])/min(S[x], S[y]) > theta)
			{
				adj[x].insert(y);
				adj[y].insert(x);
			}
			
	//add connected components as groups
	add_connected_components();
}

void dfs_dendrogram_fill(int root_G_index, vector<string> &order)
{
	if(children[root_G_index].size()==0) //a leaf
	{
		for(auto x:groups[root_G_index])
			order.push_back(x);
		return;
	}
	else
	{
		for(auto y:children[root_G_index])
			dfs_dendrogram_fill(y, order);
	}
}

int main(int argc, char** argv)
{
	m = atoi(argv[1]);
	p = atoi(argv[2]);
	read_R();
	groups.push_back(data_elements);
	parent.push_back(0);
	vector<int> empty;
	children.push_back(empty);
	
	int window_size = data_elements.size();
	
	while(window_size>0) //logarithmic decrease of the window size
	{
		add_groups(window_size, 0.5); //using theta = 1/2
		window_size /= 2; 
	}
	
	//print the result
	vector<string> order;
	dfs_dendrogram_fill(0, order);
	
	int printed = 0;
	for(auto x:order)
	{
		cout<<x<<" ";
		printed++;
		if(printed==p)
		{
			cout<<endl;
			printed = 0;
		}
	}
	cout<<endl;
	
	return 0;
}