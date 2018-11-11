//Given an integer n < 1000, creates a random array of n elements and quick-sorts it
#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>
using namespace std;
#define array items

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

void merge(int begin, int end, int mid)
{
	add_access(&begin);
	add_access(&end);
	add_access(&mid);
	vector<int> temp;
	int i1=begin;
	add_access(&i1);
	add_access(&begin);
	int i2 = mid+1;
	add_access(&i2);
	add_access(&mid);
	add_access(&i1);
	add_access(&mid);
	add_access(&i2);
	add_access(&end);
	while(i1<=mid or i2<=end)
	{
		add_access(&i1);
		add_access(&mid);
		add_access(&i2);
		add_access(&end);
		add_access(&i2);
		add_access(&end);
		add_access(&i1);
		add_access(&mid);
		add_access(&items[i1]);
		add_access(&items[i2]);
		if(i2>end or (i1<=mid and items[i1]<=items[i2]))
		{
			temp.push_back(items[i1]);
			add_access(&items[i1]);
			add_access(&temp[temp.size()-1]);
			i1++;
			add_access(&i1);
		}
		else
		{
			temp.push_back(items[i2]);
			add_access(&items[i2]);
			add_access(&temp[temp.size()-1]);
			i2++;
			add_access(&i2);
		}
	}
}

void quick_sort(int begin, int end)
{
	add_access(&begin);
	add_access(&end);
	if(begin>=end)
		return;
	
	add_access(&end);
	add_access(&begin);
	int sz = end-begin+1;
	add_access(&sz);
	int x = items[begin + (rand()%sz)];
	add_access(&sz);
	add_access(&begin);
	add_access(&x);
	
	vector<int> lesseq, greater;

	add_access(&begin);
	for(int i=begin;i<=end;++i)
	{
		add_access(&i);
		add_access(&end);
		add_access(&i);
		add_access(&array[i]);
		add_access(&x);
		if(items[i]<=x)
		{
			lesseq.push_back(array[i]);
			add_access(&i);
			add_access(&array[i]);
			add_access(&lesseq[lesseq.size()-1]);
		}
		else
		{
			greater.push_back(array[i]);
			add_access(&i);
			add_access(&array[i]);
			add_access(&greater[greater.size()-1]);
		}
	}
	
	for(int i=0;i<lesseq.size();++i)
	{
		add_access(&i);
		items[begin+i] = lesseq[i];
		add_access(&i);
		add_access(&lesseq[i]);
		add_access(&begin);
		add_access(&i);
		add_access(&items[begin+i]);
	}
	
	
	for(int i=0;i<greater.size();++i)
	{
		add_access(&i);
		items[begin+lesseq.size()+i] = greater[i];
		add_access(&i);
		add_access(&greater[i]);
		add_access(&begin);
		add_access(&i);
		add_access(&items[begin+lesseq.size()+i]);
	}
	
		
	quick_sort(begin, begin+lesseq.size()-1);
	quick_sort(begin+lesseq.size(), end);
	merge(begin, end, begin+lesseq.size()-1);
}

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);

	for(int i=0;i<n;++i)
	{
		items.push_back(rand());
	}
	
	quick_sort(0, items.size()-1);
	
		
	printR();
	return 0;
}
