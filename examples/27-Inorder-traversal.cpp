#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <map>
using namespace std;

 
/* A binary tree node has data, a pointer to parent
and a vector of pointers to children */
struct Node
{
    int data;
    Node* parent;
    vector<Node*> children;
    Node(int data)
    {
        this->data = data;
        children.clear();
    }
};

/* Given a binary tree, print its nodes in inorder*/
void printInorder(struct Node* node)
{
    if (node == NULL)
        return;
 
    /* first recur on left child */
    if(node->children.size())
    	printInorder(node->children[0]);
 
    /* then print the data of node */
    cout << node->data <<endl;
 
    /* now recur on right child */
    for(int i=1; i<node->children.size(); i++)
    	printInorder(node->children[i]);
}

int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
    srand(1397+n);

    vector<Node*> nodes;

    for(int i=0; i<n; i++){
        Node* node = new Node(i);
        if(i!=0){
            node->parent = nodes[rand()%i];
            nodes[rand()%i]->children.push_back(node);
        }
        nodes.push_back(node);

    }

    printInorder(nodes[0]);
    return 0;
}	