//Computing fibonacci numbers from F1 to Fn

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

int fibo(int n)
{
	add_access(&n);
	add_access(&n);
	if(n==0)
		return 0;
	if(n==1)
		return 1;
	int ans = fibo(n-1)+fibo(n-2);
	add_access(&ans);
	return ans;
}

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);

	fibo(n);
	
	printR();
	return 0;
}
