/*
An implementation of the CPACK algorithm as explained in "CApRI: Cache-conscious data reordering for irregular codes" by Ding and Kandemir
The algorithm was originally presented in "Improving Cache Performance in Dynamic Applications through Data and Computation Reorganization at Runtime" by Ding and Kennedy
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

int main(int argc, char** argv)
{
	m = atoi(argv[1]);
	p = atoi(argv[2]);
	read_R();
	unordered_set<string> put;
	set<string> current_block;
	for(auto r:R)
	{
		if(put.find(r)!=put.end()) //already put
			continue;
		if(current_block.size()==p) //print it out
		{
			for(auto d:current_block)
				cout<<d<<" ";
			cout<<endl;
			current_block.clear(); //erase all
		}
		
		//put the new element
		current_block.insert(r);
		put.insert(r);
		
	}
	for(auto d:current_block)
				cout<<d<<" ";
			cout<<endl;
	return 0;
}