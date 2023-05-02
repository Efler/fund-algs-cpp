#ifndef RED_BLACK_TREE_RED_BLACK_TREE_H
#define RED_BLACK_TREE_RED_BLACK_TREE_H
#include "../binary_search_tree/binary_search_tree.h"


template <typename tkey, typename tvalue, typename tkey_comparer>
class red_black_tree final : public binary_search_tree<tkey, tvalue, tkey_comparer>
{
private:

    enum class color{
        red,
        black
    };

//    struct rb_node final : public binary_search_tree<tkey, tvalue, tkey_comparer>::node{
    struct node final : public binary_search_tree<tkey, tvalue, tkey_comparer>::node{     //////TODO ???
        node() = default;
        node(tkey k, tvalue v, node* l, node* r, color c):
                key(k), value(v), left(l), right(r), node_color(c){}

        tkey key;
        tvalue value;
        node* left;
        node* right;
        color node_color;

        virtual ~node() = default;
    };

private:

    class rb_find_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::find_template_method
    {
    public:

        void after_find(typename associative_container<tkey, tvalue>::key_value_pair* target_key_and_result_value, node* root) override {

        }

    };

    class rb_insert_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::insert_template_method
    {
    private:

        stack<node*> _insert_path = stack<node*>();

    public:

        void after_insert(const tkey &key, const tvalue &value, node** root, logger* logger) override {

        }

    };

    class rb_remove_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::remove_template_method
    {
    private:

        stack<node*> _remove_path = stack<node*>();

    public:

        void after_remove(const tkey &key, node** root, logger* logger) override {

        }

    };

private:

    logger* _logger;
    abstract_allocator* _allocator;
    rb_find_template_method* _find;
    rb_insert_template_method* _insert;
    rb_insert_template_method* _remove;

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
