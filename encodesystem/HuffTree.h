#ifndef HUFFTREE_H
#define HUFFTREE_H

#include "HuffNode.h"
#include "LeafNode.h"
#include "IntlNode.h"

template <typename E>
class HuffTree {
private:
HuffNode<E>* Root;
// Tree root
public:
HuffTree(E& val, int freq) // Leaf constructor
{ Root = new LeafNode<E>(val, freq); }
// Internal node constructor
HuffTree(HuffTree<E>* l, HuffTree<E>* r)
{ Root = new IntlNode<E>(l->root(), r->root()); }
 ~HuffTree() {}
// Destructor
HuffNode<E>* root() { return Root; }
// Get root
int weight() { return Root->weight(); } // Root weight
};




#endif // HUFFTREE_H
