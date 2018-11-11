//Vector-Vector Multiplication (internal)
//Gets an integer N as input and outputs a sequence from multiplying a vector of length N with an NxN matrix

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

	//create the input
	vector<int> vec(n);
	vector<vector<int> > matrix;
	for(int i=0;i<n;++i)
		matrix.push_back(vec);
	vector<int> ans(n);

	//make the multiplication
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			ans[i]+=vec[j]*matrix[i][j];
			add_access(&ans[i]);
			add_access(&vec[j]);
			add_access(&matrix[i][j]);
		}

	printR();
	return 0;
}
