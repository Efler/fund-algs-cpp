#ifndef RED_BLACK_TREE_RED_BLACK_TREE_H
#define RED_BLACK_TREE_RED_BLACK_TREE_H
#include "../binary_search_tree/binary_search_tree.h"
#include "debug_rb_tree_printing.h"


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
                binary_search_tree<tkey, tvalue, tkey_comparer>::node(k, v, l, r){
                node_color = c;
        }

        color node_color;

        virtual ~rb_node() = default;
    };


    ///*-------------------- RB_INSERT_TEMPLATE_METHOD FIELD --------------------*///

private:

    class rb_insert_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::insert_template_method
    {
    public:

        explicit rb_insert_template_method(red_black_tree<tkey, tvalue, tkey_comparer>* this_rbt):
                binary_search_tree<tkey, tvalue, tkey_comparer>::insert_template_method(this_rbt){}

    private:

        red_black_tree<tkey, tvalue, tkey_comparer>* _this;

    private:

        size_t get_node_size() const override {
            return reinterpret_cast<size_t>(sizeof(rb_node));
        }

        void get_node_constructor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** x, const tkey& key, const tvalue& value) const override {
            new (*(reinterpret_cast<rb_node**>(x))) rb_node{key, value, nullptr, nullptr, color::red};
        }

        void insert_balance(stack<rb_node*>* _insert_path, rb_node** rb_root){
            rb_node* target = _insert_path->top();
            _insert_path->pop();
            if(_insert_path->empty()) target->node_color = color::black;
            else{
                if(_insert_path->top()->node_color != color::black){
                    rb_node* dad = _insert_path->top();
                    rb_node* uncle;
                    _insert_path->pop();
                    if(_insert_path->top()->left == dad) uncle = reinterpret_cast<rb_node*>(_insert_path->top()->right);
                    else uncle = reinterpret_cast<rb_node*>(_insert_path->top()->left);
                    if(uncle != nullptr && uncle->node_color == color::red){
                        dad->node_color = color::black;
                        uncle->node_color = color::black;
                        _insert_path->top()->node_color = color::red;
                        insert_balance(_insert_path, rb_root);
                    }else{
                        rb_node* grandpa = _insert_path->top();
                        rb_node* grand_grand;
                        if(_insert_path->size() > 1){
                            _insert_path->pop();
                            grand_grand = _insert_path->top();
                        }else grand_grand = nullptr;
                        if(grandpa->left == dad){
                            if(dad->right == target){
                                _this->rotate_left(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&grandpa->left));
                                if(grand_grand != nullptr){
                                    if(grand_grand->left == grandpa) _this->rotate_right(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&grand_grand->left));
                                    else _this->rotate_right(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&grand_grand->right));
                                }else{
                                    _this->rotate_right(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(rb_root));
                                }
                                target->node_color = color::black;
                                grandpa->node_color = color::red;
                            }else{
                                if(grand_grand != nullptr){
                                    if(grand_grand->left == grandpa){
                                        _this->rotate_right(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&grand_grand->left));
                                    }else{
                                        _this->rotate_right(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&grand_grand->right));
                                    }
                                }else{
                                    _this->rotate_right(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(rb_root));
                                }
                                dad->node_color = color::black;
                                grandpa->node_color = color::red;
                            }
                        }else {
                            if (dad->left == target) {
                                _this->rotate_right(
                                        reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(&grandpa->right));
                                if(grand_grand != nullptr){
                                    if(grand_grand->left == grandpa) _this->rotate_left(
                                                reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(&grand_grand->left));
                                    else _this->rotate_left(
                                                reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(&grand_grand->right));
                                }else{
                                    _this->rotate_left(
                                            reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(rb_root));
                                }
                                target->node_color = color::black;
                                grandpa->node_color = color::red;
                            } else {
                                if(grand_grand != nullptr){
                                    if(grand_grand->left == grandpa) _this->rotate_left(
                                                reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(&grand_grand->left));
                                    else _this->rotate_left(
                                                reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(&grand_grand->right));
                                }else{
                                    _this->rotate_left(
                                            reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(rb_root));
                                }
                                dad->node_color = color::black;
                                grandpa->node_color = color::red;
                            }
                        }
                    }
                }
            }
        }

    public:

        void after_insert(const tkey &key, const tvalue &value, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** root, logger* logger, stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node*>* _i_path) override {
            auto** rb_root = reinterpret_cast<rb_node**>(root);
            auto* _insert_path = reinterpret_cast<stack<rb_node*>*>(_i_path);
            if(_insert_path->size() == 1){
                _insert_path->top()->node_color = color::black;
                if(logger != nullptr) logger->log("after_insert: Tree was balanced!", logger::severity::debug);
                return;
            }else{
                _insert_path->top()->node_color = color::red;
                insert_balance(_insert_path, rb_root);
                if(logger != nullptr) logger->log("after_insert: Tree was balanced!", logger::severity::debug);
                return;
            }
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

    ///*-------------------- TREE DEBUG_PRINTING FIELD --------------------*///

private:

    void debug_tree_printing_function(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* root) const override {
        debug_rb_tree_printing<tkey, tvalue>(reinterpret_cast<void*>(root));        ////TODO DEBUG!!!!!!!!!!!!!!
    }

    ///*-------------------- TREE CONSTRUCTORS FIELD --------------------*///

public:

    explicit red_black_tree(abstract_allocator* alloc = nullptr, logger* logger = nullptr):
        binary_search_tree<tkey, tvalue, tkey_comparer>(alloc, logger, new typename binary_search_tree<tkey, tvalue, tkey_comparer>::find_template_method(), new rb_insert_template_method(this), new rb_remove_template_method())
    {
        if(logger != nullptr) logger->log("Red_black_tree CREATED!", logger::severity::debug);
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