//Vector-Vector Multiplication (internal)
//Gets an integer N as input and outputs a sequence from multiplying two vectors of length N

#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);
	vector<int> vec1;
	vector<int> vec2;
	for(int i=0;i<n;++i)
	{
		vec1.push_back(rand());
		vec2.push_back(rand());
	}
	int k = rand(); //k is numbered 2n+1
	for(int i=0;i<n;++i)
	{
		vec1[i] = vec1[i] * vec2[i]; //vec1[i] is numbered i+1, vec2[i] is numbered n+i+1
		cout<<i+1<<endl<<i+n+1<<endl<<i+1<<endl;
	}
	return 0;
}
