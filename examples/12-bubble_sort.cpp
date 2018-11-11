//Given an integer n < 1000, creates a random array of n elements and bubble-sorts it
#include <iostream>
#include <vector>
#include <cstdlib>
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

vector<int> items;

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);

	for(int i=0;i<n;++i)
		items.push_back(rand());
	
	//bubble sort
	for(int i=0;i<n;++i)
		for(int j=1;j<n;++j)
		{
			add_access(&items[j-1]);
			add_access(&items[j]);
			if(items[j-1]>items[j])
			{
				swap(items[j], items[j-1]);
				add_access(&items[j]);
				add_access(&items[j-1]);
			}
		}
	
	printR();
	return 0;
}
