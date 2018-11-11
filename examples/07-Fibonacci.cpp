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

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);

	vector<int> v(n+1);
	v[1] = 1;
	add_access(&v[1]);
	for(int i=2;i<=n;++i)
	{
		v[i] = v[i-1]+v[i-2];
		add_access(&v[i-1]);
		add_access(&v[i-2]);
		add_access(&v[i]);
	}
	
	printR();
	return 0;
}
