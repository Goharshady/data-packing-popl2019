//Creates a sorted array of length n and performs n random binary searches
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <map>
using namespace std;

vector<void *> R;
map<void *, int> index;

void add_access(void *a)
{
	R.push_back(a);
	if(index.find(a)==index.end())
		index[a] = index.size()+1;
}

void printR()
{
	for(int i=0;i<R.size();++i)
		cout<<index[R[i]]<<endl;
}

vector<int> all;

void binarySearch(int begin, int end, int val) //does not contain the end index
{
	if(begin>=end-1)
	{
		add_access(&all[begin]);
		return;
	}
	int mid = (begin+end)/2;
	add_access(&all[mid]);
	if(all[mid]==val)
		return;
	else if(all[mid]<val)
		binarySearch(mid+1, end, val);
	else
		binarySearch(begin, mid, val);
}

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(1397+n);
	
	for(int i=0;i<n;++i)
		all.push_back(rand());
	sort(all.begin(), all.end());
	
	for(int i=0;i<n;++i)
		binarySearch(0, all.size(), rand());
		
	printR();
	return 0;
}
