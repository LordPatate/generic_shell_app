struct array_builder {
    size_t count;
    size_t element_size;
    struct element_list *first;
};

struct element_list {
    struct element_list *tail;
    char head_contents;
};
