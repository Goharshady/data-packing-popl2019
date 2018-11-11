//Scalar-Vector Multiplication 
//Gets an integer N as input and outputs a sequence from multiplyin a vector of length N with a constant

#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);
	vector<int> vec;
	for(int i=0;i<n;++i)
		vec.push_back(rand());
	int k = rand(); //k is numbered n+1
	for(int i=0;i<n;++i)
	{
		vec[i] = vec[i] * k; //vec[i] is numbered i+1
		cout<<i+1<<endl<<n+1<<endl<<i+1<<endl;
	}
	return 0;
}
