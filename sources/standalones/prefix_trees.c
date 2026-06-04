#include "prefix_trees.h"

struct node *_new_leaf(void *data) {
    struct node *leaf = malloc(sizeof (struct node));
    if (leaf != NULL) {
        for (int i = 0; i < PTREE_NB_CHILDREN; ++i) {
            leaf->children[i] = NULL;
        }
        leaf->data = data;
    }
    return leaf;
}

struct prefix_tree *empty_prefix_tree() {
    struct prefix_tree *tree = malloc(sizeof (struct prefix_tree));
    if (tree != NULL) {
        tree->root = _new_leaf(NULL);
    }
    return tree;
}

struct result_or_error _ptree_push(struct node *tree, char *key, void *data) {
    if (tree == NULL) {
        tree = _new_leaf(NULL);
        if (tree == NULL) {
            return ERROR(PTREE_ALLOCATION_FAILED);
        }
    }
    if (*key) {
        if (*key >= PTREE_NB_CHILDREN) return ERROR(PTREE_INVALID_CHAR);
        struct result_or_error push_res = _ptree_push(tree->children[*key], key + 1, data);
        if (push_res.ok == ERROR_TYPE) return push_res;
        tree->children[*key] = push_res.result;
    } else {
        tree->data = data;
    }
    return RESULT(tree);
}
enum ptree_error_code ptree_push(struct prefix_tree *tree, char *key, void *data) {
    struct result_or_error push_res = _ptree_push(tree->root, key, data);
    if (push_res.ok) {
        tree->root = push_res.result;
        return PTREE_OK;
    } else {
        return push_res.error;
    }
}

struct result_or_error _ptree_search(struct node *tree, char *key) {
    if (tree == NULL) {
        return ERROR(PTREE_KEY_NOT_FOUND);
    } else {
        if (*key) {
            if (*key >= PTREE_NB_CHILDREN) return ERROR(PTREE_INVALID_CHAR);
            return _ptree_search(tree->children[*key], key + 1);
        } else {
            return RESULT(tree->data);
        }
    }
}
struct result_or_error ptree_search(struct prefix_tree *tree, char *key) {
    return _ptree_search(tree->root, key);
}

void _ptree_free(struct node *tree) {
    if (tree != NULL) {
        for (int i = 0; i < PTREE_NB_CHILDREN; ++i) {
            _ptree_free(tree->children[i]);
        }
        free(tree->data);
        free(tree);
    }
}
void ptree_free(struct prefix_tree *tree) {
    _ptree_free(tree->root);
}
