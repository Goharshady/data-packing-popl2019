//Given an integer n < 1000, chooses a random r < n and computes C(n, r) by dynamic programming
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

int dp[1000][1000]; 



int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);

	int r = rand()%n;
	
	for(int i=1;i<=n;++i)
		for(int j=0;j<=i;++j)
		{
			if(j==0)
			{
				dp[i][j] = 1;
				add_access(&dp[i][j]);
			}
			else
			{
				dp[i][j] = dp[i-1][j] + dp[i-1][j-1];
				add_access(&dp[i-1][j]);
				add_access(&dp[i-1][j-1]);
				add_access(&dp[i][j]);
			}
		}
	
	printR();
	return 0;
}
