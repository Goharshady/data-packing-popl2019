//Given an integer n < 1000, creates a random array of n elements and insertion-sorts it
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
	{
		items.push_back(rand());
		int index = items.size()-1;
		add_access(&items[index]);
		add_access(&index);
		add_access(&items[index]);
		add_access(&items[index-1]);
		while(index>0 and items[index]>items[index-1])
		{
			swap(items[index], items[index-1]);
			add_access(&items[index]);
			add_access(&items[index-1]);
			index--;
			add_access(&index);
		}
	}
	
		
	printR();
	return 0;
}
