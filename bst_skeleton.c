#include <stdio.h>
#include <stdlib.h>
#include "bst.h"


//creating a node to search through
Tree_Node* createnode(char data){
    Tree_Node* new = (Tree_Node*)malloc(sizeof(Tree_Node));
    if (new){
        new ->data = data;
        new ->left = NULL;
        new ->right = NULL;
    }

    return new;
}

//inserts a new character into our tree and creaetes a new node if the root node is NULL. if not it finds the correct position 
void tree_insert(Tree_Node** root, char data){
    if (*root == NULL){
        *root = createnode(data);
    }
    //if the data being processed is smaller than the root it will put it at the left side 
    else if (data < (*root)->data){
        //will pass the value of the pointer pointing to the left side
        tree_insert(&((*root)->left), data);
    }
    //if the data is bigger than the root it will put it at the right side
    else if (data > (*root)->data){
        //will pass the value of the pointer pointing to the right side 
        tree_insert(&((*root)->right), data);
    }
}

//this function will create a bst from our array and it goes through the whole array inserting each character
Tree_Node* create_bst (char data[]){
    Tree_Node* root = NULL;
    //insert each charater until we reach null
    for (int i = 0; data[i] != '\0'; i++){
        tree_insert(&root, data[i]);
    }
    return root;
}

//this function will be used to search through our tree
Tree_Node* tree_search(Tree_Node* root, char data){
    //if the node we are searching is equal to the first data it will return the data
    if (root == NULL || root->data == data){
        return root;
    }
    //if the data searching is smaller than the current data being analyzed it will search the next node in the left side 
    else if (data < root->data){
        return tree_search(root->left, data);
    }
    //will do the same but if it's bigger and to the right 
    else if (data > root->data){
        return tree_search(root->right, data);
    }
    return NULL;
}

//will print our tree if existed
void tree_print_sorted(Tree_Node* root){
    if(root != NULL){
        //will print the left side of the root
        tree_print_sorted(root->left);
        printf("%c", root->data);
        //then will print the right side
        tree_print_sorted(root->right);
    }
}

//when finished to clear the memory
void tree_delete(Tree_Node* root){
    if (root != NULL){
        //delete left side
        tree_delete(root->left);
        //delete right side
        tree_delete(root->right);
        free(root);
    }
}

