//Makes a csv file from the experiment results

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int timeInMillis(string data)
{
	for(int i=0;i<data.size();++i)
		if(data[i]=='m' or data[i]=='s' or data[i]=='.')
			data[i] = ' ';
	stringstream cnv(data);
	int m,s,mil;
	cnv>>m>>s>>mil;
	s += 60*m;
	mil+=1000*s;
	return mil;
}

bool file_exists(string path)
{
	ifstream infile(path.c_str());
	return infile.good();
}

int getN(string path)
{
	path+="/input/ref.txt";
	ifstream cin(path.c_str());
	int N=0;
	string x;
	while(cin>>x)
		N++;
	cin.close();
	return N;
}

void short_report_to_csv(string path)
{
	path += "/output/short_report.txt";
	ifstream cin(path.c_str());
	string ignore;
	cin>>ignore; //the
	cin>>ignore; //tree
	cin>>ignore; //width
	cin>>ignore; // <=
	int tw;
	cin>>tw;
	cin>>ignore; //cache
	cin>>ignore; //misses:
	int misses[9];
	for(int i=0;i<9;++i)
		cin>>misses[i];
	cin>>ignore; //times:
	int tm[9];
	int tdtm;
	cin>>ignore; //tree
	cin>>ignore; //decomposition:
	string data;
	cin>>data;
	tdtm = timeInMillis(data);
	for(int i=0;i<9;++i)
	{
		cin>>ignore;
		cin>>ignore;
		cin>>data;
		tm[i] = timeInMillis(data);
		if(i>=4 and i<=6)
		{
			//read the second time
			cin>>ignore; //+
			cin>>data;
			tm[i] += timeInMillis(data);
		}
	}
	cout<<tw<<", ";
	for(int i=0;i<9;++i)
		cout<<misses[i]<<", ";
	cout<<tdtm<<", ";
	for(int i=0;i<9;++i)
	{
		cout<<tm[i];
		if(i!=8)
			cout<<", ";
	}
	cin.close();
}

int main()
{
	cout<<"m, p, en, enpar (n), N, tw, misses_ours, misses_A, misses_B, misses_C, misses_D, misses_E, misses_F, misses_G, misses_H, time_TD, time_ours, time_A, time_B, time_C, time_D, time_E, time_F, time_G, time_H"<<endl;
	
	for(int m=1;m<=5;++m)
		for(int p=2;p<=5;++p)
			for(int en=1;en<=31;++en)
				for(int n=2;n<=100;++n)
				{
					string path = "Experiment-"+to_string(m)+"-"+to_string(p)+"-"+to_string(en)+"-"+to_string(n);
					if(file_exists(path+"/output/error.txt")) //if an error has happened in the experiment
						continue;
					cout<<m<<", "<<p<<", "<<en<<", "<<n<<", ";
					cout<<getN(path)<<", ";
					short_report_to_csv(path);
					cout<<endl;
				}
	
	return 0;
}
