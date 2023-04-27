#ifndef RED_BLACK_TREE_RED_BLACK_TREE_H
#define RED_BLACK_TREE_RED_BLACK_TREE_H
#include "../binary_search_tree/binary_search_tree.h"


template <typename tkey, typename tvalue, typename tkey_comparer>
class red_black_tree final : public binary_search_tree<tkey, tvalue, tkey_comparer>
{




private:

    logger* _logger;
    abstract_allocator* _allocator;
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::find_template_method* _find;
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::insert_template_method* _insert;
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::remove_template_method* _remove;

public:

    explicit red_black_tree(abstract_allocator* alloc = nullptr, logger* logger = nullptr){
        //TODO
    }

    red_black_tree(const red_black_tree<tkey, tvalue, tkey_comparer>& tree){
        //TODO
    }

    red_black_tree(red_black_tree<tkey, tvalue, tkey_comparer>&& tree){
        //TODO
    }

    red_black_tree<tkey, tvalue, tkey_comparer>& operator=(const red_black_tree<tkey, tvalue, tkey_comparer>& tree){
        //TODO
    }

    red_black_tree<tkey, tvalue, tkey_comparer>& operator=(red_black_tree<tkey, tvalue, tkey_comparer>&& tree){
        //TODO
    }

    ~red_black_tree(){
        //TODO
    }

};


#endif //RED_BLACK_TREE_RED_BLACK_TREE_H
