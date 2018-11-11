//scalar-matrix Multiplication (internal)
//Gets an integer N as input and outputs a sequence from multiplying two NxN matrices

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
	
	vector<vector<int> > ans=matrix;
	vector<vector<int> > matrix2 = matrix;


	//make the multiplication
	for(int i=0;i<n;++i)
	for(int j=0;j<n;++j)
	for(int k=0;k<n;++k)
	{
		ans[i][j]+=matrix[i][k] * matrix2[k][j];
		add_access(&matrix[i][k]);
		add_access(&matrix2[k][j]);
		add_access(&ans[i][j]);
	}

	printR();
	return 0;
}
