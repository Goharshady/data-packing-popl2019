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
        //left = right = NULL;
    }
};
 
/* Given a binary tree, print its nodes according to the
"bottom-up" postorder traversal. */
void printPostorder(struct Node* node)
{
    if (node == NULL)
        return;
 
    for(int i=0; i<node->children.size(); i++){
        // first recur on left subtree
        printPostorder(node->children[i]);
    }
 
    // now deal with the node
    cout << node->data <<endl;
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

    printPostorder(nodes[0]);
    return 0;
}