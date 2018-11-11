//Creates a DSU structure on n elements and performs n random merges
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


struct node
{
	int id;
	node *parent;
	int subtree_size;
};

vector<node *> all;

void merge_nodes(node *u, node *v)
{
	add_access(u);
	add_access(v);
	while(u->parent!=u)
	{
		u = u->parent;
		add_access(u);
	}
	while(v->parent!=v)
	{
		v = v->parent;
		add_access(v);
	}
	add_access(u);
	add_access(v);
	if(u->subtree_size < v->subtree_size)
		swap(u, v);
	v->parent = u;
	u->subtree_size += v->subtree_size;
	add_access(v);
	add_access(u);
}

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(1397+n);
	
	for(int i=0;i<n;++i)
	{
		node *novo = new node();
		novo->id = i;
		novo->parent = novo;
		novo->subtree_size = 1;
		all.push_back(novo);
		add_access(novo);
	}
	
	for(int mrg=0;mrg<n;++mrg)
	{
		int u=rand()%n;
		int v=rand()%n;
		merge_nodes(all[u], all[v]);
	}
			
	printR();
	return 0;
}
