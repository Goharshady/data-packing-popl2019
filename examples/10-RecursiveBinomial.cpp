//Given an integer n < 1000, chooses a random r < n and computes C(n, r) by recursion
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

int computeBinom(int n, int r)
{
	add_access(&n);
	if(n==0)
		return 1;
	add_access(&r);
	if(r==0)
		return 1;
	add_access(&n);
	add_access(&r);
	add_access(&n);
	add_access(&r);
	int ans =  computeBinom(n-1, r-1)+computeBinom(n-1, r);
	add_access(&ans);
	return ans;
}


int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);

	int r = rand()%n;
	
	computeBinom(n, r);
	
	printR();
	return 0;
}
