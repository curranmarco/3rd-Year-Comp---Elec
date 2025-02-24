#include "bstdb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Write your submission in this file
//
// A main function and some profiling tools have already been set up to test
// your code in the task2.c file. All you need to do is fill out this file
// with an appropriate Binary Search Tree implementation.
//
// The input data will be books. A book is comprised of a title and a word
// count. You should store both these values in the tree along with a unique
// integer ID which you will generate.
//
// We are aiming for speed here. A BST based database should be orders of
// magnitude faster than a linked list implementation if the BST is written
// correctly.
//
// We have provided an example implementation of what a linked list based
// solution to this problem might look like in the db/listdb.c file. If you
// are struggling to understand the problem or what one of the functions
// below ought to do, consider looking at that file to see if it helps your
// understanding.
//
// There are 6 functions you need to look at. Each is provided with a comment
// which explains how it should behave. The functions are:
//
//  + bstdb_init
//  + bstdb_add
//  + bstdb_get_word_count
//  + bstdb_get_name
//  + bstdb_stat
//  + bstdb_quit
//
// Do not rename these functions or change their arguments/return types.
// Otherwise the profiler will not be able to find them. If you think you
// need more functionality than what is provided by these 6 functions, you
// may write additional functions in this file.

//structure that stores different variables of our books
typedef struct BSTN {
	int doc_id;
	char *title;
	int wordcount;
	char *author;
	//pointers to the left and right sides of the parent nodes
	struct BSTN *left;
	struct BSTN *right;
} BSTN;

BSTN *root = NULL;
int nodecount = 0;


int
bstdb_init ( void ) {
	//initialize the root pointer 
	root = NULL;
	return 1;
}

int bstdb_add ( char *name, int word_count, char *author ) {
	//update global variable
	nodecount++;
	//create our inital doc_id variable and give it a random value
	int doc_id = rand();
	//check if the value already exists and if exists give it a new random value
	while (bstdb_get_word_count(doc_id) != -1){
		int doc_id = rand();
	}
	
	//initialize new node and if null exit
	BSTN *newnode = (BSTN *)malloc(sizeof(BSTN));
	if (newnode == NULL){
		return -1;
	}

	//add the values of all this node to the structure 
	newnode->doc_id = doc_id;
	//allocate this memory dinamically and add one counting null termiantor 
	newnode->title = (char*)malloc(strlen(name) + 1);
	//if non existant free newnode
	if (newnode->title == NULL){
		free(newnode);
		return -1;
	}
	
	//same here with author
	newnode->author = (char*)malloc(strlen(name) + 1);
	if (newnode->author == NULL){
		free(newnode);
		return -1;
	}
	//pointers to give values of wordcount and also set the values of the children to null
	newnode->wordcount = word_count;
	newnode->left = newnode->right = NULL;

	//this will be for our initial node
	if (root == NULL){
		root = newnode;
	}
	
	else {
		//pointer used to go through the tree
		BSTN *current = root;
		//pointer to keep track of the parent of the current node
		BSTN *parent = NULL;

		//traverses the tree to find the position for the new node
		while(current != NULL){
			parent = current;
			//if smaller point current to the left node
			if (doc_id < current->doc_id){
				current = current->left;
			}
			//if larger point to the right node
			else {
				current = current->right;
			}
		}
		//here it will see if the node needs to go to the left or right side of the parent node
		if (doc_id < parent->doc_id){
			parent->left = newnode;
		}
		else {
			parent->right = newnode;
		}
	}
	

	//return the doc id for our  tree
	return doc_id;
}

int bstdb_get_word_count ( int doc_id ) {
	//variable and we give it the root value
	BSTN *current = root;

	//run till end of tree reached or node found
	while (current != NULL){
		//return the pointer to the word count if equal id's
		if (doc_id == current->doc_id){
			return current->wordcount;
		}
		//if smaller point current to the left node
		else if (doc_id < current->doc_id){
			current = current->left;
		}
		//if larger point current to the right node
		else {
			current = current->right;
		}
	}

	return -1;
}

//function to get the title of a book
char* bstdb_get_name ( int doc_id ) {
	//giving a pointer the root value
	BSTN *current = root;

	//iterate till the end of the tree or node found
	while (current != NULL){
		//return the pointer to the title if equal id's
		if (doc_id == current->doc_id){
			return current->title;
		}
		//if smaller point current to the left node
		else if (doc_id < current->doc_id){
			current = current->left;
		}
		//if larger point current to the right node
		else {
			current = current->right;
		}
	}
	return NULL;
}

//counts the total number of nodes in the tree
int bstcountnodes (BSTN *node){
	//here is our base case (only if there is no subtree)
	if (node == NULL){
		return 0;
	}
	//here we will count the root node, plus the nodes on the left side and the right side
	return 1 + bstcountnodes(node->left) + bstcountnodes(node->right);
}

//calculates the height of the tree
int treeheight (BSTN *node){
	//base just in case there is an empty tree
	if (node == NULL){
		return -1;
	}

	//recursive functions ot calculate the height on the left and right side
	int lheight = treeheight(node->left);
	int rheight = treeheight(node->right);

	//here we take the max value and add 1
	return (lheight > rheight ? lheight : rheight) + 1;
}

//counts number of nodes visited during a search
int searchnodes(BSTN *node, int doc_id, int count){
	//this is for when there is no further nodes
	if (node == NULL){
		return count;
	}

	count ++;

	//recursive functions comparing the doc_id wiht the current node's doc_id
	//if smaller it goes through the left subtree
	if (doc_id < node->doc_id){
		return searchnodes(node->left, doc_id, count);
	}
	//if larger goes through the right subtree
	else if (doc_id > node->doc_id){
		return searchnodes(node->right, doc_id, count);
	}
	else return count;
}

//function that calculates the height, compares node counts, and does average number of visits and prints it
void bstdb_stat ( void ) {
	//base case if tree is empty
	if (root == NULL){
		printf("Tree is empty.\n");
		return;
	}

	//calculate the height of the tree on the left side and right side of our root
	int left = treeheight(root->left);
	int right = treeheight(root->right);

	printf("Root left subtree height: %d, right subtree height: %d\n", left, right);

	//it will compare the value of the global variable that counted nodes, and the value the function created for it gives
	int anodecount = bstcountnodes(root);
	printf("Expected node count: %d, Actual node count: %d\n", nodecount, anodecount);
	if (anodecount == nodecount){
		printf("Node count is as expected.\n");
	}
	else {
		printf("Mismatch in node count.\n");
	}
	
	
	int totalvisits = 0;
	int numbersearches = 0;

	//here it goes through all the potental id's from 0 and uses the function search nodes to measure how many visits are done 
	for (int i = 0; i < anodecount; i++){
		int visits = searchnodes(root, i, 0);
		totalvisits += visits;
		numbersearches++;
	}

	//here we are calculating the average
	double averagevisits = (double)totalvisits/numbersearches;
	printf("Average number of nodes visited: %.2f\n", averagevisits);
		
}

//helper function to free the tree
void freetree (BSTN *node){
	//checks if there is a value in the node and frees the left and right side 
	if (node != NULL){
		freetree(node->left);
		freetree(node->right);

		//also frees the pointers to title and author if exist
		if (node->title != NULL){
			free(node->title);
		}
		if (node->author != NULL){
			free(node->author);
		}
		free(node);
	}
}

//function that will run and quit using freetree
void bstdb_quit ( void ) {
	freetree(root);
	
	root = NULL;
	
}
