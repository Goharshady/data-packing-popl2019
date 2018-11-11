#ifndef __TDLIBRARY__
#define __TDLIBRARY__
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <typeinfo>
#include <sstream>

using namespace std;

#define HashSet unordered_set
#define HashMap unordered_map
#define state set<pair<set<string>,int> >
#define stateSz second
#define statePhi first

struct STATE_HASH{ //used for hashing states
    size_t operator()(const state &x)const{
      string hs[x.size()];
      string ret = "";
      int i=0;
      for(auto it:x){
        for(auto& element : it.first){
          hs[i] += element+',';
        }
        hs[i] += to_string(it.second)+' ';
        i++;
    }
    for(int j=0; j<x.size(); j++){
      ret += hs[j]+"-";
    }
    return hash<string>()(ret);
  }
};

struct Edge
{
	vector<string> endpoints;
	int weight = 1;

	void addVertex(string vertex)
	{
		endpoints.push_back(vertex);
	}
};

struct Bag {
	int id;
	vector<string> vertices;
	Bag* parent = NULL;
	vector<Bag*> children;
	string bagType = "";
	Edge introducedEdge;
	string introducedVertex = "";
  string forgottenVertex = "";

  HashMap<state,int, STATE_HASH> dp;

    void addVertex(string vertex){
    	vertices.push_back(vertex);
    }

    void addChild(Bag* child){
    	children.push_back(child);
    }
};



bool isComment(const string &line)
{
	return line.length()>1 and line[0]=='-' and line[1]=='-';
}

bool readLine(ifstream &infile, string &line)
{
	while(getline(infile, line))
	{
		if(!isComment(line))
			return true;
	}
	return false;
}

//replaces all instances of ch in source with space
void removeCharacter(string &source, char ch)
{
	for(int i=0;i<source.length();++i)
		if(source[i]==ch)
			source[i]=' ';
}

istream& operator >> (istream &cin, Edge &e)
{
	string input;
	cin>>input;
	removeCharacter(input, ',');
	removeCharacter(input, '=');
	removeCharacter(input, '<');
	removeCharacter(input, '>');
	stringstream ss(input);
	string read;
	while(ss>>read)
	{
		if(read=="weight")
			ss>>e.weight;
		else
			e.addVertex(read);
	}
	return cin;
}



//reads the nice tree decomposition from fileName
Bag* readNiceTreeDecomposition(string fileName){
	Bag* root = NULL;
	ifstream infile;
    infile.open(fileName);
    string line;
    HashMap<int, Bag*> idBagMap;
    while(readLine(infile,line)){

    	// reads bag id
    	stringstream ss(line);
    	Bag* bagAddress = new Bag();
    	Bag &bag = *bagAddress;
    	ss>>bag.id;
    	idBagMap[bag.id] = bagAddress;


    	//reads bag vertices
    	readLine(infile,line);
    	removeCharacter(line, ',');
    	removeCharacter(line, '[');
    	removeCharacter(line,']');
    	stringstream verticesSs(line);
    	string vertex;
    	while(verticesSs>>vertex)
    		bag.addVertex(vertex);

    	//reads the parent
    	readLine(infile,line);
    	if(line != "null"){
    		stringstream parentSs(line);
    		int parentId;
    		parentSs>>parentId;
    		bag.parent = idBagMap[parentId];

    		// adds the current bag to the children of its parent
    		idBagMap[parentId]->addChild(bagAddress);
   		}
   		else //this is the root bag
   		{
   			root = bagAddress;
   			cerr<<"root is:"<<bagAddress<<endl;
   		}

   		//reads the bag type
   		readLine(infile,line);
   		bag.bagType = line;

   		if(bag.bagType == "IV"){
   			readLine(infile,line);
   			bag.introducedVertex = line;
   		}

   		if(bag.bagType == "FV"){
   			readLine(infile,line);
   			bag.forgottenVertex = line;
   		}

   		if(bag.bagType == "IE")
   		{
   			readLine(infile, line);
   			stringstream edgeSs(line);
   			Edge e;
   			edgeSs >> e;
   			bag.introducedEdge = e;
    	}

    }
    return root;
}

void printNiceTreeDecomposition(Bag* bag){
	cout<<"--Bag Number = "<<endl;
    cout<<bag->id<<endl;
    cout<<"--Vertices = "<<endl<<"[";
    for(int i = 0; i<bag->vertices.size(); i++){
    	cout<<bag->vertices[i];
    	if(i!=bag->vertices.size()-1)
    		cout<<", ";
    }
   
    cout<<"]"<<endl;

    cout<<"--Parent = "<<endl;
    if(bag->parent==NULL)
    	cout<<"null"<<endl;
    else
    	cout<<bag->parent->id<<endl;
    

    string bagType = bag->bagType;
    cout<<"--Bag type = "<<endl<<bagType<<endl;

    if(bagType=="IE"){
    	auto introducedEdge = bag->introducedEdge;
    	auto endpoints = introducedEdge.endpoints;
        cout<<"--The bag is introducing the edge : "<<endl<<'<';
        for(auto endpoint:endpoints){
        	cout<<endpoint<<',';
        }
        cout<<"weight=" <<introducedEdge.weight<<'>'<<endl;
    }

    if(bagType=="IV"){
    	cout<<"--The bag is introducing the vertex : "<<endl<<bag->introducedVertex<<endl;
    }

    if(bagType=="FV"){
    	cout<<"--The bag is forgetting the vertex : "<<endl<<bag->forgottenVertex<<endl;
    }


    for(auto childBag:bag->children){
        printNiceTreeDecomposition(childBag);
    }
}

#endif