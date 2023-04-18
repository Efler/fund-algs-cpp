#ifndef BINARY_SEARCH_TREE_BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_BINARY_SEARCH_TREE_H
#include "associative_container.h"
#include "../allocator_2/abstract_allocator.h"
#include "../logger/logger_builder.h"


template <typename tkey, typename tvalue, typename tkey_comparer>
class binary_search_tree: public associative_container<tkey, tvalue, tkey_comparer>
{

protected:

    struct node{
        tkey key;
        tvalue value;
        node* left;
        node* right;
    };

private:

    node* _root;
    abstract_allocator* _allocator;
    logger* _logger;

public:

    explicit binary_search_tree(abstract_allocator* alloc = nullptr, logger* logger = nullptr):
            _root(nullptr), _allocator(alloc), _logger(logger)
    {
        if(_logger != nullptr) _logger->log("Binary_search_tree CREATED", logger::severity::debug);
    }

    binary_search_tree(const binary_search_tree<tkey, tvalue, tkey_comparer>& tree) = delete;

    binary_search_tree(binary_search_tree<tkey, tvalue, tkey_comparer>&& tree) = delete;

    binary_search_tree<tkey, tvalue, tkey_comparer>& operator=(const binary_search_tree<tkey, tvalue, tkey_comparer>& tree) = delete;

    binary_search_tree<tkey, tvalue, tkey_comparer>& operator=(binary_search_tree<tkey, tvalue, tkey_comparer>&& tree) = delete;

    ~binary_search_tree() override {
        ///TODO
    }



};


#endif //BINARY_SEARCH_TREE_BINARY_SEARCH_TREE_H
