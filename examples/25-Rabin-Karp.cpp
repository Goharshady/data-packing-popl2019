//Creates a random string of length n of digits, and a random pattern string of min(5, n/2+1), then searches the first one for the second one using the Rabin-Karp hashing algorithm, we are using the sum of values as a fingerprint
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
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

string pattern, mainString;

bool searchFrom(int index)
{
	for(int i=0;i<pattern.length();++i)
	{
		add_access(&pattern[i]);
		add_access(&mainString[index+i]);
		if(pattern[i]!=mainString[index+i])
			return false;
	}
	return true;
}

int fingerprintOf(string &x)
{
	int ans = 0;
	for(int i=0;i<x.length();++i)
	{
		add_access(&x[i]);
		ans+=x[i];
	}
	
}

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(1397+n);

	for(int i=0;i<n;++i)
	{
		mainString += "0";
		mainString[mainString.size()-1]+=rand()%10;
	}
	
	for(int i=0;i<5 and i<n/2+1;++i)
	{
		pattern += "0";
		pattern[pattern.size()-1]+=rand()%10;
	}
	
	int patternHash;
	add_access(&patternHash); 
	patternHash = fingerprintOf(pattern);
	int currentHash = 0;
	add_access(&currentHash);
	for(int i=0;i<mainString.length();++i)
	{
		currentHash += mainString[i];
		add_access(&mainString[i]);
		add_access(&currentHash);
		if(i-pattern.length()>=0)
		{
			currentHash -= mainString[i-pattern.length()];
			add_access(&mainString[i-pattern.length()]);
			add_access(&currentHash);
		}
		add_access(&currentHash);
		add_access(&patternHash);
		if(currentHash == patternHash)
			if(searchFrom(i-pattern.length()+1))
				break;
	}
	printR();
	return 0;
}
