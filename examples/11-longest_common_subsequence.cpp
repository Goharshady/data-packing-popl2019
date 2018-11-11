//Given an integer n < 1000, chooses a random m <= n, creates two random sequences of lengths n and m of digits, computes the longest common subsequence
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

vector<int> seq1, seq2;
int lcm[1000][1000];

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);

	int m = rand()%n + 1;
	
	for(int i=0;i<n;++i)
	{
		seq1.push_back(rand()%10);
		add_access(&seq1[seq1.size()-1]);
	}
	
	for(int i=0;i<m;++i)
	{
		seq2.push_back(rand()%10);
		add_access(&seq2[seq2.size()-1]);
	}
	
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
		{
			if(i==0 or j==0)
			{
				lcm[i][j] = 0;
				add_access(&lcm[i][j]);
			}
			else
			{
				add_access(&seq1[i]);
				add_access(&seq2[j]);
				if(seq1[i]==seq2[j])
				{
					lcm[i][j] = lcm[i-1][j-1]+1;
					add_access(&lcm[i-1][j-1]);
					add_access(&lcm[i][j]);
				}
				else
				{
					lcm[i][j] = max(lcm[i-1][j], lcm[i][j-1]);
					add_access(&lcm[i][j-1]);
					add_access(&lcm[i-1][j]);
					add_access(&lcm[i][j]);
				}
			}
		}
	
	printR();
	return 0;
}
