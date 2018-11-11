//summarizes the log printed by run.sh in a single line
#include <iostream>
#include <vector>
using namespace std;

vector<string> times;

bool startsWith(string &a, string b)
{
	if(a.length()<b.length())
		return false;
	for(int i=0;i<b.length();++i)
		if(a[i]!=b[i])
			return false;
	return true;
}

int main()
{
	vector<int> misses;
	string ln;
	while(getline(cin, ln))
	{
		if(ln=="Cache misses:")
		{
			int x;
			cin>>x;
			misses.push_back(x);
		}
		if(startsWith(ln, "the tree width <="))
			cout<<ln<<endl<<endl;
		if(startsWith(ln, "real"))
			times.push_back(ln.substr(5));
	}
	
	cout<<"Cache misses:"<<endl;
	for(auto x:misses)
		cout<<x<<" ";
	cout<<endl<<endl;
	
	cout<<"Times: "<<endl;
	cout<<"Tree Decomposition:\t\t" <<times[0]<<endl;
	cout<<"Our Algorithm:\t\t\t"<<times[1]<<endl;
	cout<<"Pipeline A:\t\t\t"<<times[2]<<endl;
	cout<<"Pipeline B:\t\t\t"<<times[3]<<endl;
	cout<<"Pipeline C:\t\t\t"<<times[4]<<endl;
	cout<<"Pipeline D:\t\t\t"<<times[5]<<" + "<<times[6]<<endl;
	cout<<"Pipeline E:\t\t\t"<<times[7]<<" + "<<times[8]<<endl;
	cout<<"Pipeline F:\t\t\t"<<times[9]<<" + "<<times[10]<<endl;
	cout<<"Pipeline G:\t\t\t"<<times[11]<<endl;
	cout<<"Pipeline H:\t\t\t"<<times[12]<<endl;
	
	
	
	return 0;
}
