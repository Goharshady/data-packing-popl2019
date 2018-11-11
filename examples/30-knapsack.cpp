//Given a knapsack of size n, with n objects of small size and random values, tries to fill the knapsack maximizing the total value of taken objects
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <cmath>
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


vector<int> sizes, values;

int dp[1000][2]; //dp[i][j%2] = maximum total with a knapsack of size i, using the first j elements

void knapsack(int n)
{
	for(int j=1;j<=n;++j) //j items
		for(int i=0;i<=n;++i) //knapsack of capacity i
		{
			add_access(&dp[i][(j-1)%2]);
			add_access(&dp[i][j%2]);
			dp[i][j%2] = dp[i][(j-1)%2];
			add_access(&sizes[j]);
			if(i>=sizes[j])
			{
				dp[i][j%2] = max(dp[i][j%2], dp[i-sizes[j]][(j-1)%2] + values[j]);
				add_access(&dp[i-sizes[j]][(j-1)%2]);
				add_access(&values[j]);
				add_access(&dp[i][j%2]);
			}
		}
}

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);
	
	sizes.push_back(0);
	values.push_back(0);
	
	for(int i=0;i<n;++i)
	{
		sizes.push_back(rand()%5);
		values.push_back(rand()%10000);
	}
	
	knapsack(n);
	
	printR();
	return 0;
}
