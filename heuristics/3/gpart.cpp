/*
An implementation of the GPART algorithm as explained in "CApRI: Cache-conscious data reordering for irregular codes" by Ding and Kandemir
The algorithm was originally presented in "Exploiting Locality for irregular scintific codes" by Han and Tseng
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


map<string, unordered_set<string>> adj; //adjacency list of the graph
#define degree(x) (adj[(x)].size())

struct SSU
{
	unordered_set<string> keys;
	vector<unordered_set<string>> blocks;
	map<string, int> block_of_key;
	
	void add_key(string x)
	{
		if(keys.find(x)!=keys.end()) //already exists
			return;
		keys.insert(x);
		unordered_set<string> novo;
		novo.insert(x);
		blocks.push_back(novo);
		block_of_key[x] = blocks.size()-1;
	}
	
	int block_size(int index)
	{
		return blocks[index].size();
	}
	
	int block_size(string key)
	{
		return block_size(block_of_key[key]);
	}
	
	void merge_blocks(int b1, int b2)
	{
		if(b1==b2) return;
		if(block_size(b1) < block_size(b2)) //b1 is the bigger one
			swap(b1, b2);
		
		//block of key
		for(auto key:blocks[b2])
			block_of_key[key] = b1;
		
		//blocks
		blocks[b1].insert(blocks[b2].begin(), blocks[b2].end());
		blocks[b2].clear();
	}
	
	void merge_keys(string key1, string key2)
	{
		merge_blocks(block_of_key[key1], block_of_key[key2]);
	}
	
	void print()
	{
		for(auto b:blocks)
			if(b.size()>0)
			{
				for(auto x:b)
					cout<<x<<" ";
				cout<<endl;
			}
	}
	
};

SSU ssu;
vector<string> nodes_by_degree;

int main(int argc, char** argv)
{
	m = atoi(argv[1]);
	p = atoi(argv[2]);
	read_R();
	
	//make the access graph
	for(int i=1;i<R.size();++i)
	{
		string c = R[i];
		string p = R[i-1];
		if(c!=p)
		{
			adj[c].insert(p);
			adj[p].insert(c);
		}
	}
	
	for(string x:R)
		ssu.add_key(x);
	
	for(auto node:ssu.keys)
		nodes_by_degree.push_back(node);
	sort(nodes_by_degree.begin(), nodes_by_degree.end(), [](string a, string b){return degree(a)>degree(b);});
	
	bool merged = true;
	while(merged)
	{
		merged = false;
		for(auto node:nodes_by_degree) //in the decreasing order of degree
			for(auto node2:adj[node])
				if(ssu.block_of_key[node]!=ssu.block_of_key[node2] and ssu.block_size(node)!=0 and ssu.block_size(node2)!=0 and ssu.block_size(node) + ssu.block_size(node2) <= p) //if one can merge
				{
					merged = true;
					ssu.merge_keys(node2, node);
					break;
				}
	}
	
	ssu.print();
	return 0;
}