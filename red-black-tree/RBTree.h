#ifndef RBTREE_RBTREE_H
#define RBTREE_RBTREE_H

// a color of a Node.
// enum defines a new data type (much like struct)
// the enum names get a value, starting from 0. Each consecutive
// name is +1 of the last. In this case, RED==0, BLACK==1.
typedef enum Color
{
	RED, BLACK
} Color;

/**
 * pointer to a function that compares tree items.
 * @a, @b: two items.
 * @return: equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
typedef int (*CompareFunc)(const void *a, const void *b);

/**
 * pointer to a function to apply on all tree items.
 * @object: a pointer to an item of the tree.
 * @args: pointer to other arguments for the function.
 * @return: 0 on failure, other on success.
 */
typedef int (*forEachFunc)(const void *object, void *args);

/**
 * pointer to a function that frees a data item
 * @object: a pointer to an item of the tree.
 */
typedef void (*FreeFunc)(void *data);

/*
 * a node of the tree.
 */
typedef struct Node
{
	struct Node *parent, *left, *right;
	Color color;
	void *data;
} Node;

/**
 * represents the tree
 */
typedef struct RBTree
{
	Node *root;
	CompareFunc compFunc;
	FreeFunc freeFunc;
	long unsigned size;
} RBTree;

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc); // implement it in RBTree.c

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree *tree, void *data); // implement it in RBTree.c

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data); // implement it in RBTree.c

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to check an item in.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data); // implement it in RBTree.c



/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args); // implement it in RBTree.c

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree); // implement it in RBTree.c


#endif //RBTREE_RBTREE_H