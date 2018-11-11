/*
An implementation of the k-distance algorithm for Data Packing. The k-distance algorithm is presented in "Array regrouping and Structure splitting using whole-program reference affinity" by Zhong et al
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
#include <unordered_set>
#include <deque>
#include <unordered_map>
using namespace std;

#define min(x,y) (((x)<(y))?(x):(y))
#define affinity_group unordered_set<string>

inline double absS(double x)
{
	return x>0?x:(-x);
}

vector<string> R;
unordered_set<string> data_elements;
int p,m;


int LOG(int x)
{
	int ans = 1;
	int MAX = 2;
	while(MAX<x)
	{
		MAX *= 2;
		ans++;
	}
	return ans;
}

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

unordered_map<string, set<int>> access_indices;
unordered_map<string, unordered_map<int, int>> reuse_signature_number;
unordered_map<string, unordered_map<int, int>> reuse_signature_total;
unordered_map<string, unordered_map<int, double>> reuse_signature_average;

void compute_reuse_signatures()
{
	//clear
	access_indices.clear();
	reuse_signature_number.clear();
	reuse_signature_average.clear();
	reuse_signature_total.clear();
	
	//fill access_indices
	for(int i=0;i<R.size();++i)
		access_indices[R[i]].insert(i);
	
	//compute reuse signatures
	for(int i=0;i<R.size();++i)
	{
		string x = R[i];
		access_indices[x].erase(i);
		if(access_indices[x].size() == 0)
			continue;
		int j = *access_indices[x].begin();
		int dist = j - i;
		reuse_signature_number[x][LOG(dist)]++;
		//reuse_signature_total[x][LOG(dist)]+=dist;
	}
	
	//compute Avg's
	for(auto obj:reuse_signature_number)
	{
		string x = obj.first;
		for(auto obj2:reuse_signature_number[x])
		{
			int i = obj2.first;
			reuse_signature_average[x][i] = ((double)reuse_signature_total[x][i]) / reuse_signature_number[x][i];
		}
	}
}

double manhattan_distance(unordered_map<int, double> &a, unordered_map<int, double> &b)
{
	unordered_set<int> keys;
	for(auto obj:a)
		keys.insert(obj.first);
	for(auto obj:b)
		keys.insert(obj.first);
	double ans = 0;
	for(auto key:keys)
		ans += absS(a[key]-b[key]);
	return ans;
}


unordered_map<string, unordered_set<string>> adj; //adjacency list
unordered_set<string> dfsseen;

void dfs(string element, affinity_group &G)
{
	if(dfsseen.find(element) != dfsseen.end())
		return ;
	G.insert(element);
	dfsseen.insert(element);
	for(auto v:adj[element])
		dfs(v, G);
}

void add_connected_comps() //adds connected components of adj as groups
{
	dfsseen.clear();
	affinity_group G;
	for(string element:data_elements)
		if(dfsseen.find(element) == dfsseen.end())
		{
			G.clear();
			dfs(element, G);
			add_the_group(G);
		}
}

void add_groups(int k)
{
	//clear
	adj.clear();

	
	int N = R.size();
	int B = LOG(N);
	
	//add edges
	for(auto x:data_elements)
		for(auto y:data_elements)
			if(x!=y and reuse_signature_average.find(x)!=reuse_signature_average.end() and reuse_signature_average.find(y)!=reuse_signature_average.end())
				if(manhattan_distance(reuse_signature_average[x], reuse_signature_average[y]) < k * B)
				{
					adj[x].insert(y);
					adj[y].insert(x);
				}
	
	//add connected components as groups
	add_connected_comps();
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
	
	compute_reuse_signatures();
	
	int k = data_elements.size() * m * p * 2;
	
	while(k>0) //logarithmic decrease of k
	{
		add_groups(k); 
		k /= 2; 
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
