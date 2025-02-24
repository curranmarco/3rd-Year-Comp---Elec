#include <stdio.h>
#include "bst_skeleton.c"

int main() {
    char data[] = "FLOCCINAUCINIHILIPILIFICATION"; // example input string
    Tree_Node* root = create_bst(data);

    printf("Sorted tree elements: ");
    tree_print_sorted(root);
    printf("\n");

    char search_char = 'd';
    Tree_Node* result = tree_search(root, search_char);
    if (result) {
        printf("Character %c found in the tree.\n", search_char);
    } else {
        printf("Character %c not found in the tree.\n", search_char);
    }

    // Clean up the tree
    tree_delete(root);
    return 0;
}
