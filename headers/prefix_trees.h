#ifndef _PREFIX_TREES
#define _PREFIX_TREES

#include <stdlib.h>

#include "res_or_err.h"

enum ptree_nb_children {PTREE_NB_CHILDREN = 128};

enum ptree_error_code {
    PTREE_OK,
    PTREE_INVALID_CHAR,
    PTREE_KEY_NOT_FOUND,
    PTREE_ALLOCATION_FAILED
};

struct node {
    struct node *children[128];
    void *data;
};

struct prefix_tree {
    struct node *root;
};

// Return a new empty prefix tree or NULL if allocation failed.
struct prefix_tree *empty_prefix_tree();

// Add the key to the tree with data associated.
// Return 0 on success or an appropriate error code otherwise.
enum ptree_error_code ptree_push(struct prefix_tree *tree, char *key, void *data);

// Return a pointer to the data associated with the key in the tree,
// or an appropriate error code in case of failure.
struct result_or_error ptree_search(struct prefix_tree *tree, char *key);

// Free the data of each node and the nodes themselves.
// Does not free the given `tree` pointer.
void ptree_free(struct prefix_tree *tree);

#endif  // _PREFIX_TREES
