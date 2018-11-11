//Creates a set of n random points in 2d and finds the closest pair (recursively)
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

struct point
{
	int x;
	int y;
	int getX()
	{
		add_access(this);
		return x;
	}
	int getY()
	{
		add_access(this);
		return y;
	}
	int setX(int val)
	{
		add_access(this);
		x = val;
	}
	int setY(int val)
	{
		add_access(this);
		y = val;
	}
};

double dist(point *a, point *b)
{
	double diffX = a->getX() - b->getX();
	double diffY = a->getY() - b->getY();
	return sqrt(diffX*diffX + diffY*diffY);
}

vector<point *> allPoints;

double findClosest(vector<point *> points)
{
	if(points.size()<=3)
	{
		double ans = 1e19;
		for(point *a:points)
			for(point *b:points)
				if(a!=b)
					ans = min(ans, dist(a, b));
		return ans;				
	}
	
	//sort points by their x value
	sort(points.begin(), points.end(), [](point *a, point *b)->bool{return a->getX() < b->getX();});
	
	//break them in two parts
	vector<point *> points1, points2;
	double xmid = 0;
	for(int i=0;i<points.size();++i)
		if(i*2<points.size())
		{
			points1.push_back(points[i]);
			xmid = (points[i]->getX() + points[i+1]->getX())/2;
		}
		else
		{
			points2.push_back(points[i]);
		}
			
	double ans = min(findClosest(points1),findClosest(points2));
	
	//find all points within ans of xmid
	vector<point *> nearPoints;
	for(auto p:points)
		if(abs(p->getX()-xmid) < ans)
			nearPoints.push_back(p);
			
	//sort them by y value
	sort(nearPoints.begin(), nearPoints.end(), [](point *a, point *b)->bool{return a->getY() < b->getY();});
	
	double finalAns = ans;
	for(int i=0;i<nearPoints.size();++i)
		for(int j=i+1;j<nearPoints.size() and abs(nearPoints[j]->getY() - nearPoints[i]->getY())<ans;++j)
			finalAns = min(finalAns, dist(nearPoints[j], nearPoints[i]));
	
	return finalAns;

}

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	srand(2018+n);
	
	for(int i=0;i<n;++i)
	{
		point *p = new point();
		p->setX(rand()%10000);
		p->setY(rand()%10000);
		allPoints.push_back(p);
	}
	
	findClosest(allPoints);
	
	printR();
	return 0;
}
