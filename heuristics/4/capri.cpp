/*
Input:	 call with the command-line parameters  m (cache size) and p (packing factor)
put the sequence R in ref.txt
put the data placement scheme in scheme.txt (one block per line)
Output: Writes the optimized data placement scheme to stdout
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <unordered_set>
using namespace std;

vector<string> R;
int p,m;
unordered_map<string, int> initial_block_of;
vector<unordered_set<string>> initial_blocks;


void read_R()
{
	ifstream fin("ref.txt");
	string x;
	while(fin>>x)
		R.push_back(x);
}

void read_gamma()
{
	#define block_of initial_block_of
	ifstream fin("scheme.txt");
	string ln;
	int block_index = 0;
	while(getline(fin, ln)) //each line is one block
	{
		unordered_set<string> thisblock;
		stringstream sin(ln);
		string x;
		int cnt = 1;
		while(sin>>x)
		{
			if(cnt > p)
				exit(2); //more than p elements in block
			if(block_of.find(x)!=block_of.end() and block_of[x]!=block_index)
				exit(1); //the element appeared in another block
			
			block_of[x] = block_index;
			thisblock.insert(x);
			
			cnt++;
		}
		initial_blocks.push_back(thisblock);
		block_index++;
	}
	#undef block_of
}


unordered_map<string, unordered_set<string>> successors; //in one-way locality
void create_one_way_locality_graph()
{
	for(auto B:initial_blocks) //for every block, process R once
	{
		unordered_set<int> seen_block_indices; //blocks seen since last B
		int lastBindex = -1; //last time something in B was seen
		for(int i=0;i<R.size();++i)
		{
			if(B.find(R[i])==B.end()) //not in the current block
			{
				seen_block_indices.insert(initial_block_of[R[i]]);
			}
			else //in the current block
			{
				//process the successor relation
				if(lastBindex!=-1 and seen_block_indices.size()<m) //if there were a previous access to this block, and the distance is less than m blocks
				{
					string y = R[lastBindex];
					string x = R[i];
					successors[y].insert(x);
				}
				
				//clear everything
				lastBindex = i;
				seen_block_indices.clear();
			}
		}
	}
}

unordered_map<string, unordered_set<string>> adj; //adjacency in the locality graph
void create_locality_graph()
{
	//if x is a successor of y and y is a successor of x, they are adjacent
	for(auto obj:successors)
	{
		string x = obj.first;
		for(string y:obj.second) //y is in successors[x]
			if(successors[y].find(x)!=successors[y].end())
			{
				adj[y].insert(x);
				adj[x].insert(y);
			}
	}
}

struct ssu
{
	unordered_set<string> keys;
	vector<unordered_set<string>> groups;
	unordered_map<string, int> group_of_key;
	
	void add_key(string key)
	{
		if(keys.find(key)!=keys.end()) //already exists
			return;
		keys.insert(key);
		unordered_set<string> novo;
		novo.insert(key);
		groups.push_back(novo);
		group_of_key[key] = groups.size() - 1;
	}
	
	void merge_keys(string key1, string key2)
	{
		if(group_of_key[key1]==group_of_key[key2])
			return;
		merge_groups(group_of_key[key1], group_of_key[key2]);
	}
	
	void merge_groups(int g1, int g2)
	{
		if(g1==g2)
			return;
		if(groups[g1].size()<groups[g2].size()) //g1 must be the biggest
			swap(g1, g2);
		
		//fix group_of_key
		for(auto x:groups[g2])
			group_of_key[x] = g1;
		
		//fix groups
		for(auto x:groups[g2])
			groups[g1].insert(x);
		groups[g2].clear();
	}
	
	vector<unordered_set<string>> nonempty_groups()
	{
		vector<unordered_set<string>> ans;
		for(auto G:groups)
			if(G.size()!=0)
				ans.push_back(G);
		return ans;
	}
	
};

vector<unordered_set<string>> locality_groups;
unordered_map<string, int> locality_group_of;

void create_locality_groups()
{
	ssu cc; //connected components
	for(auto x:R)
		cc.add_key(x);
	
	//process adj
	for(auto obj:adj)
	{
		string x = obj.first;
		for(string y:obj.second)
			cc.merge_keys(x, y);
	}
	
	locality_groups = cc.nonempty_groups();
	
	for(int i=0;i<locality_groups.size();++i)
		for(auto x: locality_groups[i])
			locality_group_of[x] = i;
}

unordered_map<int, unordered_map<int, int>> affinity; //affinity[i][j] is the affinitey between locality groups i and j
void compute_affinity()
{
	for(int i=0;i<R.size();++i)
	{
		auto y = R[i];
		unordered_set<int> blocks_seen;
		for(int j=i;j<R.size();++j)
		{
			auto x = R[j];
			blocks_seen.insert(initial_block_of[x]);
			if(blocks_seen.size()>m)
				break;
			affinity[locality_group_of[x]][locality_group_of[y]]++;
			affinity[locality_group_of[y]][locality_group_of[x]]++;
		}
	}
}

unordered_map<int, int> true_size_of;
unordered_map<int, int> smallest_merged; //smallest locality group that is merged with the current locality group
set<pair<int, pair<int, int>>> affinities_first; //same as affinity, except that the affinity value comes first

int get_root_of(int A) //ssu-like pass compression
{
	vector<int> pars;
	pars.push_back(A);
	while(1)
	{
		int x = pars[pars.size()-1];
		if(smallest_merged[x] == x)
			break;
		else
			pars.push_back(smallest_merged[x]);
	}
	for(int x:pars)
		smallest_merged[x] = pars[pars.size()-1];
	return smallest_merged[A];
}

void merge_if_possible(int g1, int g2) //merges the locality groups g1 and g2, if such a merger is possible (has at most p items)
{
	g1 = get_root_of(g1);
	g2 = get_root_of(g2);
	if(g1==g2) //already merged
		return;
	if(true_size_of[g1] + true_size_of[g2] > p) //too big, cannot merge
		return;
		
	//merge
	if(g1>g2)
		swap(g1, g2);
	
	smallest_merged[g2] = g1;
	true_size_of[g1]+=true_size_of[g2];
	true_size_of[g2] = 0;
	
	//fix the affinities
	//affinity[g1][g3] += affinity[g2][g3]
	for(auto obj:affinity[g2])
	{
		int g3 = obj.first;
		int affin = obj.second;
		
		affinities_first.erase(make_pair(affinity[g1][g3], make_pair(g1, g3)));
		affinity[g1][g3] += affin;
		affinities_first.insert(make_pair(affinity[g1][g3], make_pair(g1, g3)));
		
		affinities_first.erase(make_pair(affinity[g3][g1], make_pair(g3, g1)));
		affinity[g3][g1] += affin;
		affinities_first.insert(make_pair(affinity[g3][g1], make_pair(g3, g1)));
		
		affinities_first.erase(make_pair(affinity[g2][g3], make_pair(g2, g3)));
		affinities_first.erase(make_pair(affinity[g3][g2], make_pair(g3, g2)));
	}
	
	affinity.erase(g2);
	
	
}

void merge_by_affinity() //destructs affinity map above
{
	//make smallest_merged
	for(int i=0;i<locality_groups.size();++i)
	{
		smallest_merged[i] = i;
		true_size_of[i] = locality_groups[i].size();
	}
	
	//make affinities_first
	for(auto obj:affinity)
	{
		int g1 = obj.first;
		for(auto obj2:obj.second)
		{
			int g2 = obj2.first;
			int affin = obj2.second;
			if(g1<g2) //add each affinity once
				affinities_first.insert(make_pair(affin, make_pair(g1, g2)));
		}
	}
	
	//process the mergers
	while(affinities_first.size()>0)
	{
		auto highest_affin_obj = *affinities_first.rbegin(); //highest affinity
		affinities_first.erase(highest_affin_obj);
		int affin = highest_affin_obj.first;
		int g1 = highest_affin_obj.second.first;
		int g2 = highest_affin_obj.second.second;
		merge_if_possible(g1, g2);
	}
}

void print_results()
{
	//make sure all roots are in place
	for(int i=0;i<locality_groups.size();++i)
		get_root_of(i);
	
	//make the big things
	for(int i=0;i<locality_groups.size();++i)
	{
		int par = get_root_of(i);
		if(par!=i)
		{
			for(auto x:locality_groups[i])
				locality_groups[par].insert(x);
			locality_groups[i].clear();
		}
	}
	
	//print nonempty locality groups
	for(auto G:locality_groups)
		if(G.size()!=0)
		{
			for(auto x:G)
				cout<<x<<" ";
			cout<<endl;
		}
}



int main(int argc, char** argv)
{
	p = atoi(argv[2]); //read the value of p
	m = atoi(argv[1]); //read the value of m
	read_R(); //the reference sequence
	read_gamma(); //the initial data packing scheme
	
	//step 1: create the one-way locality relationships
	create_one_way_locality_graph();
	
	//step 2: create the locality graph
	create_locality_graph();
	
	//step 3: locality groups are the connected components of the locality graph
	create_locality_groups();
	
	//step 4: compute affinity between locality groups
	compute_affinity();
	
	//step 5: merge locality groups based on affinity
	merge_by_affinity();
	
	//step 6: print the merged locality groups
	print_results();
	return 0;
}