#ifndef RED_BLACK_TREE_RED_BLACK_TREE_H
#define RED_BLACK_TREE_RED_BLACK_TREE_H
#include "../binary_search_tree/binary_search_tree.h"


template <typename tkey, typename tvalue, typename tkey_comparer>
class red_black_tree final : public binary_search_tree<tkey, tvalue, tkey_comparer>
{

    ///*-------------------- RB_NODE FIELD --------------------*///

private:

    enum class color{
        red,
        black
    };

    struct rb_node final : public binary_search_tree<tkey, tvalue, tkey_comparer>::node{
        rb_node() = default;
        rb_node(const tkey& k, const tvalue& v, rb_node* l, rb_node* r, color c):
                key(k), value(v), left(l), right(r), node_color(c){}

        tkey key;
        tvalue value;
        rb_node* left;
        rb_node* right;
        color node_color;

        virtual ~rb_node() = default;
    };


    ///*-------------------- RB_INSERT_TEMPLATE_METHOD FIELD --------------------*///

private:

    class rb_insert_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::insert_template_method
    {
    private:

        size_t get_node_size() const override {
            return reinterpret_cast<size_t>(sizeof(rb_node));
        }

        void get_node_constructor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** x, const tkey& key, const tvalue& value) const override {
            new (*(reinterpret_cast<rb_node**>(x))) rb_node{key, value, nullptr, nullptr};
        }

    public:

        void after_insert(const tkey &key, const tvalue &value, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** root, logger* logger) override {
            auto** rb_root = reinterpret_cast<rb_node**>(root);
            ////TODO: INSERT BALANCE
        }

    };

    ///*-------------------- RB_REMOVE_TEMPLATE_METHOD FIELD --------------------*///

    class rb_remove_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::remove_template_method
    {
    private:

        void get_node_destructor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* x) const override {
            auto* y = reinterpret_cast<rb_node*>(x);
            y->~rb_node();
        }

    public:

        void after_remove(const tkey &key, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** root, logger* logger) override {
            auto** rb_root = reinterpret_cast<rb_node**>(root);
            ////TODO: REMOVE BALANCE
        }

    };


    ///*-------------------- TREE CONSTRUCTORS FIELD --------------------*///

public:

    explicit red_black_tree(abstract_allocator* alloc = nullptr, logger* logger = nullptr):
        binary_search_tree<tkey, tvalue, tkey_comparer>(alloc, logger, new typename binary_search_tree<tkey, tvalue, tkey_comparer>::find_template_method(), new rb_insert_template_method(), new rb_remove_template_method())
    {

    }

    red_black_tree(const red_black_tree<tkey, tvalue, tkey_comparer>& tree){
        //TODO
    }

    red_black_tree(red_black_tree<tkey, tvalue, tkey_comparer>&& tree) noexcept {
        //TODO
    }

    red_black_tree<tkey, tvalue, tkey_comparer>& operator=(const red_black_tree<tkey, tvalue, tkey_comparer>& tree){
        //TODO
    }

    red_black_tree<tkey, tvalue, tkey_comparer>& operator=(red_black_tree<tkey, tvalue, tkey_comparer>&& tree) noexcept {
        //TODO
    }

    ///*-------------------- TREE DESTRUCTOR FIELD --------------------*///

    ~red_black_tree(){
        //TODO
    }

};


#endif //RED_BLACK_TREE_RED_BLACK_TREE_H
