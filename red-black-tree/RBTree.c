#include <stdlib.h>
#include "RBTree.h"

void insertToRBTreeHelper(RBTree* tree, Node* currentNode, Node* newNode);
void handleViolation(RBTree* tree, Node* newNode);
void swapColor(Color *first, Color *second);
void rotateRight(RBTree* tree, Node* node);
void rotateLeft(RBTree* tree, Node* node);
void deleteFromRBTreeHelper(RBTree *tree, Node* node, void *data);
void noChildrenDelete(RBTree* tree, Node* node);
void oneChildDelete(RBTree* tree, Node *node);
void caseThree(RBTree* tree, Node* child, Node* parent, Node* brother, Node* closestNephew, Node* furtherNephew);
Node* findInRBTree(RBTree *tree, Node *currentNode, void *data);
Node* findSuccessor(Node* node);
void rbTreeContainsHelper(const RBTree* tree, const Node *node, const void *data, int* contains);
int forEachRBTreeHelper(const Node *node, forEachFunc func, void *args);
void freeRBTreeHelper(RBTree **tree, Node *node);

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    if (compFunc == NULL || freeFunc == NULL)
    {
        return NULL;
    }
    RBTree* tree = (RBTree *)malloc(sizeof(RBTree));
    if (tree == NULL)
    {
        return NULL;
    }
    tree->compFunc = compFunc;
    tree->freeFunc = freeFunc;
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL || RBTreeContains(tree, data))
    {
        return 0;
    }
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        return 0;
    }
    Node * leftChild = NULL;
    Node * rightChild = NULL;
    newNode->right = rightChild;
    newNode->left = leftChild;
    newNode->data = data;
    if (tree->root == NULL)
    {
        tree->root = newNode;
        newNode->parent = NULL;
        newNode->color = BLACK;
        tree->size++;
        return 1;
    }
    insertToRBTreeHelper(tree, tree->root, newNode);
    newNode->color = RED;
    handleViolation(tree, newNode);
    tree->size ++;
    tree->root = newNode;
    while (tree->root->parent != NULL)
    {
        tree->root = tree->root->parent;
    }
    tree->root->color = BLACK;
    return 1;
}

/**
 * a helper function that adds the new node to the tree.
 * @param tree: the tree to add an item to.
 * @param root: the root of the tree.
 * @param newNode: the new node that is added to the tree.
 */
void insertToRBTreeHelper(RBTree* tree, Node* root, Node* newNode)
{
    if (root != NULL)
    {
        if (tree->compFunc(newNode->data, root->data) < 0)
        {
            if (root->left != NULL)
            {
                insertToRBTreeHelper(tree, root->left, newNode);
                return;
            }
            else
            {
                root->left = newNode;
            }
        }
        else
        {
            if (root->right != NULL)
            {
                insertToRBTreeHelper(tree, root->right, newNode);
                return;
            }
            else
            {
                root->right = newNode;
            }
        }
    }
    newNode->parent = root;
}

/**
 * a helper function that handles violations according to the red black tree rules.
 * @param tree: the tree.
 * @param newNode: the new node that is added to the tree.
 */
void handleViolation(RBTree* tree, Node* newNode)
{
    Node *uncle = NULL, *child = NULL;
    child = newNode;
    if (child->parent == NULL)
    {
        child->color = BLACK;
        return;
    }
    else if (child->parent->color == BLACK)
    {
        return;
    }
    while (child->parent != NULL && child->parent->color == RED)
    {
        if (child->parent == child->parent->parent->left)
        {
            uncle = child->parent->parent->right;
            if (uncle != NULL && uncle->color == RED)
            {
                child->parent->color = BLACK;
                uncle->color = BLACK;
                child->parent->parent->color = RED;
                child = child->parent->parent;
                continue;
            }
            else if (child == child->parent->right)
            {
                child = child->parent;
                rotateLeft(tree, child);
            }
            child->parent->color = BLACK;
            child->parent->parent->color = RED;
            rotateRight(tree, child->parent->parent);
        }
        else
        {
            uncle = child->parent->parent->left;
            if (uncle != NULL && uncle->color == RED)
            {
                child->parent->color = BLACK;
                uncle->color = BLACK;
                child->parent->parent->color = RED;
                child = child->parent->parent;
                continue;
            }
            else if (child == child->parent->left)
            {
                child = child->parent;
                rotateRight(tree, child);
            }
            child->parent->color = BLACK;
            child->parent->parent->color = RED;
            rotateLeft(tree, child->parent->parent);
        }
    }
}

/**
 * a helper function that rotates right a given node.
 * @param tree: the tree.
 * @param node: the node that is being rotated.
 */
void rotateRight(RBTree* tree, Node* node)
{
    Node* y = node->left;
    node->left = y->right;
    if (y->right != NULL)
    {
        y->right->parent = node;
    }
    y->parent = node->parent;
    if (node->parent == NULL)
    {
        tree->root = y;
    }
    else if (node == node->parent->right)
    {
        node->parent->right = y;
    }
    else
    {
        node->parent->left = y;
    }
    y->right = node;
    node->parent = y;
}

/**
 * a helper function that rotates left a given node.
 * @param tree: the tree.
 * @param node: the node that is being rotated.
 */
void rotateLeft(RBTree* tree, Node* node)
{
    Node* y = node->right;
    node->right = y->left;
    if (y->left != NULL)
    {
        y->left->parent = node;
    }
    y->parent = node->parent;
    if (node->parent == NULL)
    {
        tree->root = y;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = y;
    }
    else
    {
        node->parent->right = y;
    }
    y->left = node;
    node->parent = y;
}

/**
 * a helper function that swaps the colors of given nodes.
 * @param first: a pointer of a color of a curtain node.
 * @param second: a pointer of a color of a curtain node.
 */
void swapColor(Color *first, Color *second)
{
    Color temp = *first;
    *first = *second;
    *second = temp;
}

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || tree->root == NULL || data == NULL || !RBTreeContains(tree, data))
    {
        return 0;
    }
    if (tree->size == 1 && tree->compFunc(tree->root->data, data) == 0)
    {
        tree->freeFunc(tree->root->data);
        tree->root->data = NULL;
        free(tree->root);
        tree->root = NULL;
        tree->size = 0;
        return 1;
    }
    Node* node = findInRBTree(tree, tree->root, data);
    deleteFromRBTreeHelper(tree, node, data);
    tree->size--;
    return 1;
}

/**
 * a helper function that removes the given node from the tree.
 * @param tree: the tree to remove an item from
 * @param node: the node to remove.
 * @param data: the data of the node to remove.
 */
void deleteFromRBTreeHelper(RBTree *tree, Node* node, void *data)
{
    if ((node->left == NULL && node->right == NULL))
    {
        noChildrenDelete(tree, node);
    }
    else if((node->left != NULL && node->right == NULL) || (node->left == NULL && node->right != NULL))
    {
        oneChildDelete(tree, node);
    }
    else
    {
        Node* replacement;
        replacement = findSuccessor(node);
        node->data = replacement->data;
        replacement->data = data;
        deleteFromRBTreeHelper(tree, replacement, data);
    }
}

/**
 * a helper function that handles the case when the node to delete has one child.
 * @param tree: the tree.
 * @param node: the node to remove.
 */
void oneChildDelete(RBTree* tree, Node *node)
{
    Node* replacement;
    if (node->parent == NULL)
    {
        if (node->left != NULL)
        {
            replacement = node->left;
            tree->root = node->left;
            node->left->parent = NULL;
        }
        else
        {
            replacement = node->right;
            tree->root = node->right;
            node->right->parent = NULL;
        }
    }
    else if (node->parent->left == node)
    {
        if (node->left != NULL)
        {
            replacement = node->left;
            node->parent->left = node->left;
            node->left->parent = node->parent;
        }
        else
        {
            replacement = node->right;
            node->parent->left = node->right;
            node->right->parent = node->parent;
        }
    }
    else
    {
        if (node->left != NULL)
        {
            replacement = node->left;
            node->parent->right = node->left;
            node->left->parent = node->parent;
        }
        else
        {
            replacement = node->right;
            node->parent->right = node->right;
            node->right->parent = node->parent;
        }
    }
    if (node->color == BLACK && replacement->color == RED)
    {
        replacement->color = BLACK;
    }
    tree->freeFunc(node->data);
    node->data = NULL;
    free(node);
    node = NULL;
}

/**
 * a helper function that handles the case when the node to delete has no children.
 * @param tree: the tree.
 * @param node: the node to remove.
 */
void noChildrenDelete(RBTree* tree, Node* node)
{
    Node* brother;
    Node* parent;
    Node* child;
    Node* closestNephew = NULL;
    Node* furtherNephew = NULL;
    if (node->parent->left != NULL && node->parent->left == node)
    {
        parent = node->parent;
        brother = node->parent->right;
        if (brother != NULL)
        {
            closestNephew = brother->left;
            furtherNephew = brother->right;
        }
        child = node->left;
        node->parent->left = node->left;
    }
    else
    {
        parent = node->parent;
        brother = node->parent->left;
        if (brother != NULL)
        {
            closestNephew = brother->right;
            furtherNephew = brother->left;
        }
        child = node->right;
        node->parent->right = node->right;
    }
    if (node->color == BLACK)
    {
        caseThree(tree, child, parent, brother, closestNephew, furtherNephew);
    }
    tree->freeFunc(node->data);
    node->data = NULL;
    free(node);
    node = NULL;
}

/**
 * a helper function that handles case 3.
 * @param tree: the tree.
 * @param child: the child.
 * @param parent: the parent.
 * @param brother: the brother.
 * @param closestNephew: the closest nephew.
 * @param furtherNephew: the further nephew.
 */
void caseThree(RBTree* tree, Node* child, Node* parent, Node* brother, Node* closestNephew, Node* furtherNephew)
{
    while (child == NULL || child->color == BLACK)
    {
        if (tree->root == child) // case 3a
        {
            return;
        }
        else if (brother->color == BLACK && (brother->left == NULL || brother->left->color == BLACK) &&
                 (brother->right == NULL || brother->right->color == BLACK)) //case 3b
        {
            if (parent->color == RED) // case 3bi
            {
                parent->color = BLACK;
                brother->color = RED;
                return;
            }
            else if (parent->color == BLACK) // case 3bii
            {
                brother->color = RED;
                child = parent;
                parent = child->parent;
                if (parent != NULL && parent->left == child)
                {
                    brother = parent->right;
                    closestNephew = brother->left;
                    furtherNephew = brother->right;
                }
                else if (parent != NULL && parent->right == child)
                {
                    brother = parent->left;
                    closestNephew = brother->right;
                    furtherNephew = brother->left;
                }
            }
        }
        else if (brother->color == RED) // case 3c
        {
            brother->color = BLACK;
            parent->color = RED;
            if (parent->left == child)
            {
                rotateLeft(tree, parent);
            }
            else
            {
                rotateRight(tree, parent);
            }
            if (parent->left == child)
            {
                brother = parent->right;
                closestNephew = brother->left;
                furtherNephew = brother->right;
            }
            else
            {
                brother = parent->left;
                closestNephew = brother->right;
                furtherNephew = brother->left;
            }
        }
        else if (brother->color == BLACK && (furtherNephew == NULL || furtherNephew->color == BLACK) &&
                 (closestNephew != NULL && closestNephew->color == RED)) // case 3d
        {
            closestNephew->color = BLACK;
            brother->color = RED;
            if (parent->left == child)
            {
                rotateRight(tree, brother);
                brother = parent->right;
                closestNephew = brother->left;
                furtherNephew = brother->right;
            }
            else
            {
                rotateLeft(tree, brother);
                brother = parent->left;
                closestNephew = brother->right;
                furtherNephew = brother->left;
            }
        }
        else if (brother->color == BLACK && (furtherNephew != NULL && furtherNephew->color == RED)) // case 3e
        {
            swapColor(&(parent->color), &(brother->color));
            if (parent->left == child)
            {
                rotateLeft(tree, parent);
            }
            else
            {
                rotateRight(tree, parent);
            }
            furtherNephew->color = BLACK;
            return;
        }
    }
}


/**
 * a helper function that finds the successor of a curtain node.
 * @param node: the node.
 * @return the successor node.
 */
Node* findSuccessor(Node* node)
{
    if (node->right != NULL)
    {
        Node* current = node->right;
        while (current->left != NULL)
        {
            current = current->left;
        }
        return current;
    }
    Node* parent = node->parent;
    while (parent != NULL && node == parent->right)
    {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

/**
 * a helper function that finds node based on the data.
 * @param tree: the tree.
 * @param currentNode: the root of the tree.
 * @param data: the data of the node to be found.
 * @return the node that holds the data given.
 */
Node* findInRBTree(RBTree *tree, Node *currentNode, void *data)
{
    if (currentNode == NULL)
    {
        return NULL;
    }
    if (tree->compFunc(currentNode->data, data) > 0)
    {
        return findInRBTree(tree, currentNode->left, data);
    }
    else if (tree->compFunc(currentNode->data, data) < 0)
    {
        return findInRBTree(tree, currentNode->right, data );
    }
    else
    {
        return currentNode;
    }
}

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data)
{
    int contains = 0;
    if (tree == NULL || data == NULL || tree->size == 0)
    {
        return 0;
    }
    rbTreeContainsHelper(tree, tree->root, data, &contains);
    return contains;
}

/**
 * a helper function that checks if the tree contains the data.
 * @param tree: the tree.
 * @param node: the root.
 * @param data: the data to be found.
 * @param contains: a flag that is equal 1 when data is found.
 */
void rbTreeContainsHelper(const RBTree* tree, const Node *node, const void *data, int* contains)
{
        if (node == NULL)
        {
            return;
        }
        if (tree->compFunc(node->data, data) > 0)
        {
            rbTreeContainsHelper(tree, node->left, data, contains);
        }
        else if (tree->compFunc(node->data, data) < 0)
        {
            rbTreeContainsHelper(tree, node->right, data, contains);
        }
        else if (tree->compFunc(node->data, data) == 0)
        {
            *contains = 1;
        }
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
    if (tree == NULL || func == NULL)
    {
        return 0;
    }
    if (!forEachRBTreeHelper(tree->root, func, args))
    {
        return 0;
    }
    return 1;
}

/**
 * a helper function that iterates over all the nodes recursively and runs a given function on each node.
 * @param node: the root.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTreeHelper(const Node* node, forEachFunc func, void *args)
{
    if (node == NULL)
    {
        return 1;
    }
    if (!forEachRBTreeHelper(node->left, func, args) || !func(node->data, args) ||
        !forEachRBTreeHelper(node->right, func, args))
    {
        return 0;
    }
    return 1;
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree)
{
    freeRBTreeHelper(tree, (*tree)->root);
    free(*tree);
    *tree = NULL;
}

/**
 * a helper function that frees all memory of the data structure recursively.
 * @param tree: pointer to the tree to free.
 * @param node: the root of the tree.
 */
void freeRBTreeHelper(RBTree **tree, Node *node)
{
    if (node != NULL)
    {
        freeRBTreeHelper(tree, node->right);
        (*tree)->freeFunc(node->data);
        node->data = NULL;
        freeRBTreeHelper(tree, node->left);
        free(node);
        node = NULL;
    }
}
