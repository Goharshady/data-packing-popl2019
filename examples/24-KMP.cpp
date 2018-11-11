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

void buildTable(string& w, vector <int>& t){
  t = vector <int>(w.length());
  int i = 2, j = 0;
  t[0] = -1; t[1] = 0;
  add_access(&t[0]);
  add_access(&t[1]);

  while(i < w.length()) {
    add_access(&w[i-1]);
    add_access(&w[j]);
    if(w[i-1] == w[j]) {add_access(&t[i]); t[i] = j+1; i++; j++; }
    else if(j > 0) {add_access(&t[j]); j = t[j];}
    else {add_access(&t[i]); t[i] = 0; i++; }
  }
}
int KMP(string& s, string& w) {
  int m = 0, i = 0;
  vector <int> t;
  buildTable(w, t);
  // add_access(&m);
  // add_access(&i);
  while(m+i < s.length()) {
    add_access(&w[i]);
    add_access(&s[m+i]);
    if(w[i] == s[m+i]) {
      i++;
      if(i == w.length()) return m;
    } else {
      add_access(&t[i]);
      m += i-t[i];
      if(i > 0){ 
        add_access(&t[i]);
        i = t[i];
      }
    }
  }
  return s.length();
}

string pattern, mainString;


int main(int argc, char **argv)
{
  int n = atoi(argv[1]);
  srand(1397+n);

  for(int i=0;i<n;++i)
  {
    mainString += "0";
    mainString[mainString.size()-1]+=rand()%10;
    add_access(&mainString[mainString.size()-1]);
  }
  
  for(int i=0;i<5 and i<n/2+1;++i)
  {
    pattern += "0";
    pattern[pattern.size()-1]+=rand()%10;
    add_access(&pattern[pattern.size()-1]);
  }
    KMP(mainString, pattern);
    printR();
    return 0;
}
