#include "TreeDecomposition.h"
using namespace std;



int m,p; 

const int infinity =(-1u)/2; //largest possible value of int, used in place of infinity


vector<state>computeRootStates(Bag *root, int numberOfVertices){
	vector<state> ret; //the answer to be returned
	

	//when the number of vertices is zero there is just one empty state
	if(numberOfVertices == 0)
	{
		state emptyState;
		ret.push_back(emptyState);
		return ret;
	}

	//computing states when there is only one vertex in root
	if(numberOfVertices == 1){

		for(int i=0; i<p; i++){ //the sz value for the single partition
	    	set<string> phi;
	    	phi.insert(root->vertices[0]);
	    	state s;
	    	s.insert(make_pair(phi,i));
	    	ret.push_back(s);
		}	
		return ret;

	}

	//if there are more than one vertices, compute the states recursively
	vector<state> statesWithoutLastElement = computeRootStates(root, numberOfVertices-1);
	
	for(state partialState:statesWithoutLastElement){ //we call each state over the other numberofvertices-1 elements, a partial state

		//create new states by adding the last vertex to an existing phi
		for(auto it=partialState.begin(); it!=partialState.end(); it++)
			if(it->stateSz>0){  
				string lastVertex = root->vertices[numberOfVertices-1];
				set<string> newPhi = it->statePhi;
				int newSz = it->stateSz;
				newSz--;
				newPhi.insert(lastVertex);
				auto newState = partialState;
				newState.erase(make_pair(it->statePhi, it->stateSz));
				newState.insert(make_pair(newPhi,newSz));
				ret.push_back(newState);
			}

			// creates new states by adding the last vertex to a new partition
			for(int newSz=0; newSz<p; newSz++){
				string lastVertex = root->vertices[numberOfVertices-1];
				state newState = partialState;
				set<string> newPhi;
				newPhi.insert(lastVertex);
				newState.insert(make_pair(newPhi, newSz));
				ret.push_back(newState);
			}

		}

	return ret;
}

int computeDP(Bag* bag, state s); //will follow later -- used for recursive invocation

int computeDPLeaf(Bag* bag, state s){
	return 0;
}

//pointwise subtraction, used in computeDPJoin
vector<int> operator - (const vector<int> &a, const vector<int> &b)
{
	vector<int> ans;
	for(int i=0;i<a.size();++i)
		ans.push_back(a[i]-b[i]);
	return ans;
}

//used in computeDPJoin to give the next possible value of sz1
bool nextSz1(vector<int> &sz1, const vector<int> &sz)
{
	int n = sz1.size();
	sz1[n-1]++;
	for(int i=n-1;i>0;--i)
		if(sz1[i]>sz[i])
		{
			sz1[i-1]++;
			sz1[i]=0;
		}
		else break;
	return sz1[0]<=sz[0];
}

int computeDPJoin(Bag* bag, state s){
	//make a vector of all sz values
	vector<int> szVector;
	for(auto partition:s)
		szVector.push_back(partition.stateSz);
	//make a zero vector of the same size
	vector<int> sz1;
	while(sz1.size() < szVector.size())
		sz1.push_back(0);

	int ans = infinity;
	//try all possible sz1's
	do
	{
		vector<int> sz2 = szVector - sz1;
		state s1, s2;
		int i = 0;
		for(auto partition:s)
		{
			s1.insert(make_pair(partition.statePhi, sz1[i]));
			s2.insert(make_pair(partition.statePhi, sz2[i]));
			i++;
		}
		ans = min(ans, computeDP(bag->children[0], s1)+computeDP(bag->children[1], s2));
	}while(nextSz1(sz1, szVector));

	return ans;
 }


bool missedEdge(Edge e, state s){
	HashMap<string,int> blockOfVertex;
	int i = 0;
	for(auto partition:s){
		i++;
		for(auto vertex:partition.statePhi){
			blockOfVertex[vertex] = i;
		}
	}

	int q = e.endpoints.size()-1;
	auto B = blockOfVertex[e.endpoints[q]];
	HashSet<int> visitedBlocks;
	for(int i = q-1; i>=0; i--)
	{
		auto Bi = blockOfVertex[e.endpoints[i]];
		if(Bi==B and visitedBlocks.size() < m)
			return false;
	}

	return true;


}

int computeDPIntroduceEdge(Bag* bag, state s){
	return computeDP(bag->children[0], s) + missedEdge(bag->introducedEdge, s);
}

int computeDPIntroduceVertex(Bag* bag, state s){
	auto introducedVertex = bag->introducedVertex;
	for(auto partition:s){
		auto &phi = partition.statePhi;
		if(phi.find(introducedVertex)!=phi.end()){
			phi.erase(introducedVertex);
			if(phi.size()==0)
				s.erase(partition);
			break;
		}
	}
	return computeDP(bag->children[0], s);
}

int computeDPForgetVertex(Bag* bag, state s){
	auto forgottenVertex = bag->forgottenVertex;


	int ans = infinity;

	//states that are formed by adding the new vertex to an existing partition
	for(auto partition:s)
		if(partition.stateSz > 0)//we can add to this partition
		{
			state sprime = s;
			auto newPhi = partition.statePhi;
			newPhi.insert(forgottenVertex);
			auto newSz = partition.stateSz - 1;
			sprime.erase(partition);
			sprime.insert(make_pair(newPhi, newSz));

			ans = min(ans, computeDP(bag->children[0], sprime));
		}

	// adding the forgotten vertex to a new partition
	for(int sz=0; sz<p; sz++){
		state sprime = s;
		set<string> phi;
		phi.insert(forgottenVertex);
		sprime.insert(make_pair(phi,sz));
		ans = min(ans, computeDP(bag->children[0], sprime));
	}

	return ans;
	
}

int computeDP(Bag* bag, state s){

	//check if this value was already computed
	if(bag->dp.find(s)!=bag->dp.end())
		return bag->dp[s];

	int ans  = infinity;
	if(bag->bagType=="L")
		ans = computeDPLeaf(bag, s);
	if(bag->bagType=="J")
		ans = computeDPJoin(bag, s);
	if(bag->bagType =="IE")
		ans = computeDPIntroduceEdge(bag, s);
	if(bag->bagType=="IV")
		ans = computeDPIntroduceVertex(bag, s);
	if(bag->bagType=="FV")
		ans = computeDPForgetVertex(bag, s);

	bag->dp[s] = ans;
	return ans;
}



int main(int argc, char **argv){

	m = atoi(argv[1]);
	p = atoi(argv[2]);

	Bag* root = readNiceTreeDecomposition("NiceTreeDecomposition.txt");
	//printNiceTreeDecomposition(root);

	//computing root states
	vector<state> rootStates = computeRootStates(root, root->vertices.size());


	//step 2, with step 1 embedded as calls to computeDP
	int answer = infinity;
	for(auto rootState:rootStates){
		answer = min(answer, computeDP(root, rootState));
	}

	cout<<"Cache misses:"<<endl;
	cout<<fixed<<answer<<endl;

	return 0;

}
