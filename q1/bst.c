#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bst.h"

// IMPORTANT: DO NOT CHANGE THESE VALUES
const int PRE_ORDER = 0;
const int IN_ORDER = 1;
const int POST_ORDER = 2;

// INTEGRITY STATEMENT (modify if necessary)
// I received help from the following sources:
// None. I am the sole author of this work 

// sign this statement by removing the line below and entering your name
// Name: Aditya Maru  
// login ID: a2maru

// implementation:

// see bst.h for details
struct bst *bst_create(void) 
{
    struct bst *new_bst = malloc(sizeof(struct bst));
    new_bst->root = NULL;
    return new_bst;
}

// destroy_helper(node) helps recursively destroy the
//individual nodes of the tree
// time: O(n)
static void destroy_helper(struct bstnode *node)
{
    if(node)
    {
        destroy_helper(node->left);
        destroy_helper(node->right);
        free(node);
    }
    else
    {
        return;
    }
}

// see bst.h for details
void bst_destroy(struct bst *t) 
{
    assert(t != NULL);
    if(t->root)
    {
        destroy_helper(t->root);
        free(t);
    }
    else
    {
        return;
    }
}


// see bst.h for details
int bst_size(struct bst *t) 
{
    assert(t != NULL);
    if(t->root)
    {
        return t->root->size;
    }
    else
    {
        return 0;
    }
}



// create_node(x, size_new) dynamically allocates memory to create a new node
//effects : allocates memory user must free
// time: O(1)
static struct bstnode *create_node(int x, int size_new)
{
    struct bstnode *new_node = malloc(sizeof(struct bstnode));
    new_node->item = x;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->size = size_new;
    return new_node;
}


// see bst.h for details
void bst_insert(int i, struct bst *t) 
{
    assert(t != NULL);
    struct bstnode *curnode = t->root;
    struct bstnode *prevnode = NULL;
    while (curnode)
    {
        if (curnode->item == i)
        {
            return;
        }
        else if(curnode->item < i)
        {
            curnode = curnode->right;
        }
        else
        {
            curnode = curnode->left;
        }
    }
    curnode = t->root;
    while(curnode)
    {
        prevnode = curnode;
        if (curnode->item < i)
        {
            ++curnode->size;
            curnode = curnode->right;
        }
        else
        {
            ++curnode->size;
            curnode = curnode->left;
        }
    }
    if (prevnode == NULL)
    {
        t->root = create_node(i, 1);
    }
    else if(prevnode->item < i)
    {
        prevnode->right = create_node(i, 1);
    }
    else
    {
        prevnode->left = create_node(i, 1);
    }
}


// see bst.h for details
bool bst_find(int i, struct bst *t) 
{
    assert (t != NULL);
    struct bstnode *curnode = t->root;
    while(curnode)
    {
        if(curnode->item == i)
        {
            return true;
        }
        else if(i < curnode->item)
        {
            curnode = curnode->left;
        }
        else
        {
            curnode = curnode->right;
        }
    }
    return false;
}



// select_helper(k, node) recursively locates the kth node of the bst 
//and return its value
// time: O(logn)
static int select_helper(int k, struct bstnode *node)
{
    int left_size = 0;
    if (node->left)
    {
        left_size = node->left->size;
    }
    if (left_size == k)
    {
        return node->item;
    }
    if (left_size > k)
    {
        return select_helper(k, node->left);
    }
    return select_helper(k - left_size - 1, node->right);
}


// see bst.h for details
int bst_select(int k, struct bst *t) 
{
    assert (t != NULL);
    assert (t->root->size >= k + 1);
    return select_helper(k, t->root);
}


//bst_remove_helper(i, curnode) recursively removes the node with value i 
//according to the given algorithm
// time: O(logn)
static struct bstnode *bst_remove_helper (int i, struct bstnode *curnode) 
{
    if(curnode == NULL)
    {
        return NULL;
    }
    if (i < curnode->item)
    {
        --curnode->size;
        curnode->left = bst_remove_helper(i, curnode->left);
    }
    else if (i > curnode->item)
    {
        --curnode->size;
        curnode->right = bst_remove_helper(i,curnode->right);
    }
    else if (curnode->left ==  NULL)
    {
        struct bstnode *backup = curnode->right;
        free(curnode);
        return backup;
    }
    else if(curnode->right == NULL)
    {
        struct bstnode *backup = curnode->left;
        free(curnode);
        return backup;
    }
    else
    {
        struct bstnode *next_node = curnode->right;
        while (next_node->left)
        {
            next_node = next_node->left;
        }
        curnode->item = next_node->item;
        --curnode->size;
        curnode->right = bst_remove_helper(curnode->item, curnode->right);   
    }
    return curnode;
}


// see bst.h for details
void bst_remove (int i, struct bst *t)
{
    assert(t != NULL);
    assert(bst_find(i,t));
    if(bst_find(i,t))
    {
        t->root = bst_remove_helper(i, t->root);
    }
    else
    {
        return;
    }
}


// bst_preorder(node, j) helps print the elements of the bst in preorder
// effects: prints out output
// time : O(n)
static void bst_preorder(struct bstnode *node, bool *j) 
{
    if(node == NULL)
    {
        return;
    }
    else
    {
        if(*j)
        {
            printf("%d", node->item);
            *j = false;
        }
        else
        {
            printf(",%d", node->item);
        }
        bst_preorder((node->left), j);
        bst_preorder((node->right), j);
    }
}


// bst_inorder(node, x) helps print the elements of the bst in inorder
// effects: prints out output
// time : O(n)
static void bst_inorder(struct bstnode *node, bool *x)
{
    if(node == NULL)
    {
        return;
    }
    if(node != NULL)
    {
        bst_inorder((node->left), x);
        if(*x)
        {
            printf("%d", node->item);
            *x = false; 
        }
        else
        {
            printf(",%d", node->item);
        }
        bst_inorder((node->right), x);
    }
}



// bst_postorder(node, y) helps print the elements of the bst in postorder
// effects: prints out output
// time : O(n)
static void bst_postorder(struct bstnode *node, bool *y)
{
    if(node == NULL)
    {
        return;
    }
    else
    {
        bst_postorder(node->left, y);
        bst_postorder(node->right, y);
        if(*y)
        {
            printf("%d", node->item);
            *y = false;
        }
        else
        {
            printf(",%d", node->item);
        }
    }
}


// see bst.h for details
void bst_print (int o, struct bst *t)
{
    assert(t != NULL);
    assert(o == 0 || o == 1 || o == 2);
    bool n = true;
    bool *comma = &(n); 
    if (t->root)
    {
        if (o == 0)
        {
            printf("[");
            bst_preorder((t->root), comma);
            printf("]\n");
        }
        else if (o == 1)
        {
            printf("[");

            bst_inorder((t->root), comma);
            printf("]\n");
        }
        else
        {
            printf("[");
            bst_postorder(t->root, comma);
            printf("]\n");
        }
    }
    else
    {
        printf("[");
        printf("]\n");
    }
}


// bst_inorder_arr(node, arr, index) inserts node values into an array 
// in inorder
// time : O(n)
static void bst_inorder_arr(struct bstnode *node, int *arr, int *index)
{
    if(node->left != NULL)
    {
        bst_inorder_arr((node->left), arr, index);
    }
    arr[*index] = node->item;
    *index = *index + 1;
    if (node->right != NULL)
    {
        bst_inorder_arr((node->right), arr, index);
    }
}


// see bst.h for details
int *bst_to_sorted_array(struct bst *t) 
{
    assert(t != NULL);
    int x = 0;
    int *index = &x;
    if(t->root)
    {
        int size = (t->root->size);
        int *new_arr = malloc(size * sizeof(int));
        bst_inorder_arr(t->root, new_arr, index);
        return new_arr;
    }
    else
    {
        return NULL;
    }
}


// see bst.h for details
int bst_range(int start, int end, struct bst *t)
{
    assert(t != NULL);
    assert(start <= end);
    int counter = 0;
    if (t->root)
    {
        int *array = bst_to_sorted_array(t);
        for (int i = 0; i < t->root->size; ++ i)
        {
            if(array[i] >= start && array[i] <= end)
            {
                ++counter;
            }
        }
        free(array);
    }
    return counter;
}



struct bst *sorted_array_to_bst(int *arr, int len) 
{
    int midpoint = len / 2;
    struct bst *new_bst = bst_create();
    new_bst->root = create_node(arr[midpoint], 1);
    for (int i = 0; i < midpoint; ++ i)
    {
        bst_insert(arr[i], new_bst);
    }
    for(int j = midpoint + 1; j < len; ++ j)
    {
        bst_insert(arr[j], new_bst);
    }
    return new_bst;
}


// see bst.h for details
void bst_rebalance(struct bst *t) 
{
    if(t->root)
    {
        int size_1 = t->root->size;
        int *arr = bst_to_sorted_array(t);
        struct bst *backup_1 = malloc(sizeof(struct bst));
        backup_1->root = t->root;
        struct bst *backup = sorted_array_to_bst(arr, size_1);
        t->root = backup->root;
        bst_destroy(backup_1);
        free(backup);
        free(arr);
    }
    else
    {
        return;
    }
}