//Creates a max heap and adds n random elements to it
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

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);
	heap.push_back(0); //dummy element
	add_access(&heap[0]);

	for(int i=0;i<n;++i)
		heapInsert(rand());
		
	printR();
	return 0;
}
