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

struct PT { int x; int y; };
vector<PT> p; //PTs of the Polygon to be processed
vector<PT> S; //Contains the convex hull
const double PI = 2.0*acos(0.0);
const double EPS = 1e-9; //too small/big?????
int orientation(PT p1, PT p2, PT p3) {

  add_access(&p1);
  add_access(&p2);
  add_access(&p3);

  int val = (p2.y - p1.y) * (p3.x - p2.x) -
            (p2.x - p1.x) * (p3.y - p2.y);
  add_access(&val);
  if (abs(val) < EPS) return 0;  // colinear
  add_access(&val);
  return (val > 0)? 1: 2; // clock(1) or counterclockwise(2)
}
//Returns the square of distance
int distSq(PT p1, PT p2) {
  add_access(&p1);
  add_access(&p2);
  return (p1.x - p2.x)*(p1.x - p2.x) +
          (p1.y - p2.y)*(p1.y - p2.y);
}
bool cmp (PT p1, PT p2) {
  add_access(&p1);
  add_access(&p2);
  int o = orientation(p1, p[0], p2);
  add_access(&o);
  if (o==0) return (distSq(p[0], p1) <= distSq(p[0], p2));
  add_access(&o);
  return (o==1);
}
void convexHull (int n=p.size()) {
  add_access(&n);
  add_access(&p);
  add_access(&p[0]);
  // Find the bottommost-leftmost PT
  int ymn = p[0].y, mn = 0;
  for (int i = 1; i < n; i++) {
    add_access(&i);
    add_access(&n);
    add_access(&i);
    add_access(&p[i]);
    int y = p[i].y;
    add_access(&y);
    add_access(&ymn);
    add_access(&p[i]);
    add_access(&p[mn]);
    if ((y < ymn) || (ymn == y && p[i].x < p[mn].x)) {
      add_access(&ymn);
      add_access(&p[i]);
      add_access(&mn);
      add_access(&i);
      ymn = p[i].y, mn = i;
    }
  }
  add_access(&p[0]);
  add_access(&p[mn]);
  swap(p[0], p[mn]);
  sort(p.begin()+1, p.end(), cmp);
  int m=1; //Removing collinears and same PTs
  for (int i=1; i<n; i++) {
    add_access(&i);
    add_access(&n);
    add_access(&i);
    add_access(&n);
    while (i < n-1 && orientation(p[0], p[i], p[i+1]) == 0) {
      i++;
      add_access(&i);
      add_access(&n);
    }
    add_access(&m);
    add_access(&p[m]);
    add_access(&i);
    add_access(&p[i]);
    p[m] = p[i]; 
    add_access(&m);
    m++;
  }
  add_access(&m);
  if (m<3) return; // Go for the convex hull

  add_access(&p[0]);
  add_access(&p[1]);
  add_access(&p[2]);
  S.push_back(p[0]); S.push_back(p[1]); S.push_back(p[2]);

  for (int i = 3; i < m; i++) {
    add_access(&i);
    add_access(&m);

    // Keep removing top while the turn is not ccw
    while (orientation(S[S.size()-2], S[S.size()-1], p[i]) != 2)
        S.pop_back();

    add_access(&i);
    add_access(&p[i]);
    S.push_back(p[i]);
   }
}

int maxx = 1e6;
int main(int argc, char **argv){

  int n = atoi(argv[1]);
  srand(time(NULL));
  
  for(int i=0; i<n; i++){
    int xx = rand()%maxx;
    int yy = rand()%maxx;
    PT point;
    point.x = xx;
    point.y = yy;
    p.push_back(point);
  }

  convexHull();

  printR();

  return 0;
}


