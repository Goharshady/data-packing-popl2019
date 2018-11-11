/*
Input:	 call with the command-line parameters  m (cache size) and p (packing factor)
put the sequence R in ref.txt
put the data placement scheme in scheme.txt (one block per line)
Output: Writes the number of cache misses to stdout
returns 0 if everything is fine
returns nonzero if the input is invalid
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

#define access_pair pair<int,int>
#define access_time first
#define access_block second
#define make_access_pair(x, y) make_pair((y), (x))

vector<string> R;
int p,m;
unordered_map<string, int> block_of;
int misses = 0;

void read_R()
{
	ifstream fin("ref.txt");
	string x;
	while(fin>>x)
		R.push_back(x);
}

void read_gamma()
{
	ifstream fin("scheme.txt");
	string ln;
	int block_index = 1;
	while(getline(fin, ln)) //each line is one block
	{
		stringstream sin(ln);
		string x;
		int cnt = 1;
		while(sin>>x)
		{
			if(cnt > p)
			{
				cout<<-1<<endl;
				exit(2); //more than p elements in block
			}
			if(block_of.find(x)!=block_of.end() and block_of[x]!=block_index)
			{
				cout<<-1<<endl;
				exit(1); //the element appeared in another block
			}
			
			block_of[x] = block_index;
			
			cnt++;
		}
		block_index++;
	}
}

void count_cache_misses_LRU()
{
	unordered_set<int> blocks_in_cache;
	set<access_pair> access_pairs; //each block with its last access time, sorted by the last access time
	unordered_map<int,int> last_access_time; //last access time of the block
	
	for(int i=0;i<R.size();++i)
	{
		string x = R[i];
		if(block_of.find(x)==block_of.end())
		{
			cout<<-1<<endl;
			exit(3); //an element that does not appear in any block
		}
		int block = block_of[x];
		if(blocks_in_cache.find(block)!=blocks_in_cache.end())//it is in the cache
		{
			//no miss
			//update access time
			int last = last_access_time[block];
			access_pairs.erase(make_access_pair(block, last));
			last_access_time[block] = i;
			access_pairs.insert(make_access_pair(block, i));
		}
		else
		{
			//there is a miss
			misses++;
			
			//evict by LRU
			if(blocks_in_cache.size()==m)
			{
				access_pair evict_pair = *access_pairs.begin();
				int evict = evict_pair.access_block;
				blocks_in_cache.erase(evict);
				access_pairs.erase(evict_pair);
				last_access_time.erase(evict);
			}
			//insert the block and update access time
			blocks_in_cache.insert(block);
			access_pairs.insert(make_access_pair(block, i));
			last_access_time[block] = i;
			
		}
	}
	
}

int main(int argc, char** argv)
{
	p = atoi(argv[2]); //read the value of p
	m = atoi(argv[1]); //read the value of m
	read_R(); //the reference sequence
	read_gamma(); //the data packing scheme
	count_cache_misses_LRU();
	cout<<fixed<<misses<<endl;
	return 0;
}
