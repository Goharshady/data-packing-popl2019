//Creates a max heap and adds n random elements to it, then pops them out in sorted order
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

vector<int> heap;
int heapSize = 0;


void bubbleUp(int index)
{
	if(index==1)
		return;
	add_access(&heap[index]);
	add_access(&heap[index/2]);
	if(heap[index]>heap[index/2])
	{
		swap(heap[index], heap[index/2]);
		add_access(&heap[index]);
		add_access(&heap[index/2]);
		bubbleUp(index/2);
	}
}

void heapInsert(int x)
{
	add_access(&x);
	heap.push_back(x);
	add_access(&heap[heap.size()-1]);
	heapSize++;
	add_access(&heapSize);
	bubbleUp(heap.size()-1);
}

void bubbleDown(int index)
{
	int self = index;
	add_access(&index);
	add_access(&self);
	int child1 = index * 2;
	add_access(&index);
	add_access(&child1);
	int child2 = index * 2 + 1;
	add_access(&index);
	add_access(&child2);
	int mx = self;
	add_access(&self);
	add_access(&mx);
	add_access(&child1);
	add_access(&heapSize);
	if(child1<=heapSize)
	{
		add_access(&mx);
		add_access(&heap[mx]);
		add_access(&child1);
		add_access(&heap[child1]);
		if(heap[child1]>heap[mx])
		{
			mx = child1;
			add_access(&mx);
			add_access(&child1);
		}
	}
	add_access(&child2);
	add_access(&heapSize);
	if(child2<=heapSize)
	{
		add_access(&mx);
		add_access(&heap[mx]);
		add_access(&child2);
		add_access(&heap[child2]);
		if(heap[child2]>heap[mx])
		{
			mx = child2;
			add_access(&child2);
			add_access(&mx);
		}
	}
	add_access(&mx);
	add_access(&self);
	if(mx!=self)
	{
		swap(heap[self], heap[mx]);
		add_access(&mx);
		add_access(&heap[mx]);
		add_access(&self);
		add_access(&heap[self]);
		bubbleDown(self);
	}
}

void removeTop()
{
	swap(heap[1], heap[heapSize]);
	heapSize--;
	bubbleDown(1);
}

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2017+n);
	heap.push_back(0); //dummy element
	add_access(&heap[0]);

	for(int i=0;i<n;++i)
		heapInsert(rand());
		
	add_access(&heapSize);
	while(heapSize>0)
	{
		add_access(&heapSize);
		removeTop();
	}
		
	printR();
	return 0;
}
