//Creates a binary search tree and performs n random insertions, followed by n random searches
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

struct node
{
	node *left = NULL;
	node *right = NULL;
	int value = 0;
};

void addElement(node *place, int val)
{
	add_access(place);
	if(place->value == val)
		return;
	if(place->value > val)
		if(place->left!=NULL)
			addElement(place->left, val);
		else
		{
			node *novo = new node();
			novo->value = val;
			place->left = novo;
			add_access(novo);
		}
	else
		if(place->right!=NULL)
			addElement(place->right, val);
		else
		{
			node *novo = new node();
			novo->value = val;
			place->right = novo;
			add_access(novo);
		}
}

void searchElement(node *root, int val)
{
	add_access(root);
	if(root->value == val)
		return;
	else if(root->value>val and root->left!=NULL)
		searchElement(root->left, val);
	else if(root->value <= val and root->right!=NULL)
		searchElement(root->right, val);
}

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(1397+n);
	
	node *root = new node();
	root->value = rand();
	add_access(root);
	
	for(int i=1;i<n;++i)
		addElement(root, rand());
		
	for(int i=0;i<n;++i)
		searchElement(root, rand());
		
	printR();
	return 0;
}
