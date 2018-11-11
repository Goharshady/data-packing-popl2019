//scalar-matrix Multiplication (internal)
//Gets an integer N as input. Creates min(10, n/2+1) random N-dimensional vectors and applies Gram-Schmidt 

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

void internal_mult(vector<double> &a, vector<double> &b)
{
	int x = 0;
	for(int i=0;i<a.size();++i)
		{
			x+=a[i]*b[i];
			add_access(&a[i]);
			add_access(&b[i]);
			add_access(&x);
		}
}


vector<double> operator * (vector<double> &a, vector<double> &b)
{
	internal_mult(a,b);
	internal_mult(b,b);
	internal_mult(b,b);
	return a;
}

void operator -= (vector<double> &a, vector<double> &b)
{
	for(int i=0;i<a.size();++i)
	{
		a[i] = a[i] - b[i];
		add_access(&a[i]);
		add_access(&b[i]);
		add_access(&a[i]);
	}
}


void Gram_Schmidt(vector< vector<double> > &V)
{
	for(int i=0;i<V.size();++i)
		for(int j=0;j<i;++j)
		{
			auto x = V[i]*V[j];
			V[i] -= x;
		}
}

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);

	int m = min(10, n/2+1);
	vector< vector<double> > V;
	for(int i=0;i<m;++i)
	{
		vector<double> add;
		for(int i=0;i<n;++i)
			add.push_back(rand());
		V.push_back(add);		
	}
	
	Gram_Schmidt(V);
	
	printR();
	return 0;
}
