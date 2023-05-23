#ifndef AVL_TREE_AVL_TREE_H
#define AVL_TREE_AVL_TREE_H
#include "../binary_search_tree/binary_search_tree.h"
#include "debug_avl_tree_printing.h"


template <typename tkey, typename tvalue, typename tkey_comparer>
class avl_tree final : public binary_search_tree<tkey, tvalue, tkey_comparer>
{

    ///*-------------------- AVL_NODE FIELD --------------------*///

private:

    struct avl_node final : public binary_search_tree<tkey, tvalue, tkey_comparer>::node{
        avl_node() = default;
        avl_node(const tkey& k, const tvalue& v, avl_node* l, avl_node* r, size_t h):
                binary_search_tree<tkey, tvalue, tkey_comparer>::node(k, v, l, r){
            height = h;
        }

        size_t height;

        virtual ~avl_node() = default;
    };


    ///*-------------------- AVL_INSERT_TEMPLATE_METHOD FIELD --------------------*///

private:

    class avl_insert_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::insert_template_method
    {
    public:

        explicit avl_insert_template_method(avl_tree<tkey, tvalue, tkey_comparer>* this_avl):
                binary_search_tree<tkey, tvalue, tkey_comparer>::insert_template_method(this_avl){}

    private:

        avl_tree<tkey, tvalue, tkey_comparer>* _this;

    private:

        size_t get_node_size() const override {
            return sizeof(avl_node);
        }

        void get_node_constructor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** x, const tkey& key, const tvalue& value) const override {
            new (*(reinterpret_cast<avl_node**>(x))) avl_node{key, value, nullptr, nullptr, 1};
        }

        size_t get_height(avl_node* x) const {
            if(x == nullptr) return 0;
            else return x->height;
        }

        void height_update(avl_node* x) const {
            auto* son_l = reinterpret_cast<avl_node*>(x->left);
            auto* son_r = reinterpret_cast<avl_node*>(x->right);
            x->height = max(get_height(son_l), get_height(son_r)) + 1;
        }

        int height_factor(avl_node* x) const {
            auto* son_l = reinterpret_cast<avl_node*>(x->left);
            auto* son_r = reinterpret_cast<avl_node*>(x->right);
            return (int)get_height(son_l) - (int)get_height(son_r);
        }

        void insert_balance(stack<avl_node*>* _insert_path, avl_node** avl_root, logger* logger){
            avl_node* target = _insert_path->top();
            _insert_path->pop();

            if(height_factor(target) > 1){
                if(logger != nullptr) logger->log("insert_balance: wrong balance, rotating...", logger::severity::debug);

                auto* a = reinterpret_cast<avl_node*>(target->left);
                auto* b = target;

                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** upper_p;
                if(!_insert_path->empty()){
                    if(reinterpret_cast<avl_node*>(_insert_path->top()->left) == target) upper_p = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&_insert_path->top()->left);
                    else upper_p = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&_insert_path->top()->right);
                }else upper_p = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(avl_root);

                if(height_factor(a) < 0){
                    if(logger != nullptr) logger->log("insert_balance: extra rotate...", logger::severity::debug);
                    auto* c = reinterpret_cast<avl_node*>(a->right);
                    auto* d = a;
                    _this->rotate_left(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&(target->left)));
                    height_update(d);
                    height_update(c);
                    a = reinterpret_cast<avl_node*>(target->left);
                }
                _this->rotate_right(upper_p);
                height_update(b);
                height_update(a);
                if(!_insert_path->empty()) insert_balance(_insert_path, avl_root, logger);

            }else if(height_factor(target) < -1){
                if(logger != nullptr) logger->log("insert_balance: wrong balance, rotating...", logger::severity::debug);

                auto* a = reinterpret_cast<avl_node*>(target->right);
                auto* b = target;

                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** upper_p;
                if(!_insert_path->empty()){
                    if(reinterpret_cast<avl_node*>(_insert_path->top()->left) == target) upper_p = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&_insert_path->top()->left);
                    else upper_p = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&_insert_path->top()->right);
                }else upper_p = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(avl_root);

                if(height_factor(a) > 0){
                    if(logger != nullptr) logger->log("insert_balance: extra rotate...", logger::severity::debug);
                    auto* c = reinterpret_cast<avl_node*>(a->left);
                    auto* d = a;
                    _this->rotate_right(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&(target->right)));
                    height_update(d);
                    height_update(c);
                    a = reinterpret_cast<avl_node*>(target->right);
                }
                _this->rotate_left(upper_p);
                height_update(b);
                height_update(a);
                if(!_insert_path->empty()) insert_balance(_insert_path, avl_root, logger);

            }else{
                height_update(target);
                if(!_insert_path->empty()) insert_balance(_insert_path, avl_root, logger);
            }
        }

    public:

        void after_insert(const tkey &key, const tvalue &value, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** root, logger* logger, stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node*>* _i_path) override {
            auto** avl_root = reinterpret_cast<avl_node**>(root);
            auto* _insert_path = reinterpret_cast<stack<avl_node*>*>(_i_path);

            insert_balance(_insert_path, avl_root, logger);

            if(logger != nullptr) logger->log("after_insert: Tree was balanced!", logger::severity::debug);
        }

    };

    ///*-------------------- AVL_REMOVE_TEMPLATE_METHOD FIELD --------------------*///

    class avl_remove_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::remove_template_method
    {
    public:

        explicit avl_remove_template_method(avl_tree<tkey, tvalue, tkey_comparer>* this_avl):
                binary_search_tree<tkey, tvalue, tkey_comparer>::remove_template_method(this_avl){}

    private:

        avl_tree<tkey, tvalue, tkey_comparer>* _this;

    private:

        void get_node_destructor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* x) const override {
            auto* y = reinterpret_cast<avl_node*>(x);
            y->~avl_node();
        }

        size_t get_height(avl_node* x) const {
            if(x == nullptr) return 0;
            else return x->height;
        }

        void height_update(avl_node* x) const {
            auto* son_l = reinterpret_cast<avl_node*>(x->left);
            auto* son_r = reinterpret_cast<avl_node*>(x->right);
            x->height = max(get_height(son_l), get_height(son_r)) + 1;
        }

        int height_factor(avl_node* x) const {
            auto* son_l = reinterpret_cast<avl_node*>(x->left);
            auto* son_r = reinterpret_cast<avl_node*>(x->right);
            return (int)get_height(son_l) - (int)get_height(son_r);
        }

        void remove_balance(stack<avl_node*>* _remove_path, avl_node** avl_root, logger* logger){
            if(_remove_path->empty()){
                if(*avl_root != nullptr) height_update(*avl_root);
                return;
            }
            avl_node* target = _remove_path->top();
            _remove_path->pop();

            if(height_factor(target) > 1){
                if(logger != nullptr) logger->log("remove_balance: wrong balance, rotating...", logger::severity::debug);

                auto* a = reinterpret_cast<avl_node*>(target->left);
                auto* b = target;

                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** upper_p;
                if(!_remove_path->empty()){
                    if(reinterpret_cast<avl_node*>(_remove_path->top()->left) == target) upper_p = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&_remove_path->top()->left);
                    else upper_p = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&_remove_path->top()->right);
                }else upper_p = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(avl_root);

                if(height_factor(a) < 0){
                    if(logger != nullptr) logger->log("remove_balance: extra rotate...", logger::severity::debug);
                    auto* c = reinterpret_cast<avl_node*>(a->right);
                    auto* d = a;
                    _this->rotate_left(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&(target->left)));
                    height_update(d);
                    height_update(c);
                    a = reinterpret_cast<avl_node*>(target->left);
                }
                _this->rotate_right(upper_p);
                height_update(b);
                height_update(a);
                if(!_remove_path->empty()) remove_balance(_remove_path, avl_root, logger);

            }else if(height_factor(target) < -1){
                if(logger != nullptr) logger->log("remove_balance: wrong balance, rotating...", logger::severity::debug);

                auto* a = reinterpret_cast<avl_node*>(target->right);
                auto* b = target;

                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** upper_p;
                if(!_remove_path->empty()){
                    if(reinterpret_cast<avl_node*>(_remove_path->top()->left) == target) upper_p = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&_remove_path->top()->left);
                    else upper_p = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&_remove_path->top()->right);
                }else upper_p = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(avl_root);

                if(height_factor(a) > 0){
                    if(logger != nullptr) logger->log("remove_balance: extra rotate...", logger::severity::debug);
                    auto* c = reinterpret_cast<avl_node*>(a->left);
                    auto* d = a;
                    _this->rotate_right(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&(target->right)));
                    height_update(d);
                    height_update(c);
                    a = reinterpret_cast<avl_node*>(target->right);
                }
                _this->rotate_left(upper_p);
                height_update(b);
                height_update(a);
                if(!_remove_path->empty()) remove_balance(_remove_path, avl_root, logger);

            }else{
                height_update(target);
                if(!_remove_path->empty()) remove_balance(_remove_path, avl_root, logger);
            }
        }

    public:

        void after_remove(const tkey &key, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** root, logger* logger, stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node*>* _r_path, size_t& side, size_t& additional) override {
            auto** avl_root = reinterpret_cast<avl_node**>(root);
            auto* _remove_path = reinterpret_cast<stack<avl_node*>*>(_r_path);

            if(*avl_root != nullptr) remove_balance(_remove_path, avl_root, logger);

            if(logger != nullptr) logger->log("after_remove: Tree was balanced!", logger::severity::debug);
        }

    };

    ///*-------------------- AVL_TREE DEBUG_PRINTING FIELD --------------------*///

private:

                    ////TODO: CLOSED DUE TO COURSEWORK
//    void debug_tree_printing_function(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* root) const override {
//        debug_avl_tree_printing<tkey, tvalue>(reinterpret_cast<void*>(root));
//    }

    ///*-------------------- ADDITIONAL FUNCTIONS FIELD --------------------*///

    void copy_additional_data(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* copied_node, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* to_copy) const override {
        auto* avl_copied_node = reinterpret_cast<avl_node*>(copied_node);
        auto* avl_to_copy = reinterpret_cast<avl_node*>(to_copy);
        avl_copied_node->height = avl_to_copy->height;
    }

    size_t get_node_size() const override {
        return sizeof(avl_node);
    }

    void get_node_constructor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* copied_node, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* to_copy) const override {
        auto* avl_copied_node = reinterpret_cast<avl_node*>(copied_node);
        auto* avl_to_copy = reinterpret_cast<avl_node*>(to_copy);
        new (avl_copied_node) avl_node { avl_to_copy->key, avl_to_copy->value, reinterpret_cast<avl_node*>(this->copy_inner(avl_to_copy->left)), reinterpret_cast<avl_node*>(this->copy_inner(avl_to_copy->right)), avl_to_copy->height};
    }

    ///*-------------------- TREE CONSTRUCTORS FIELD --------------------*///

public:

    explicit avl_tree(abstract_allocator* alloc = nullptr, logger* logger = nullptr):
            binary_search_tree<tkey, tvalue, tkey_comparer>(alloc, logger, new typename binary_search_tree<tkey, tvalue, tkey_comparer>::find_template_method(), new avl_insert_template_method(this), new avl_remove_template_method(this))
    {
        if(logger != nullptr) logger->log("AVL_tree CREATED!", logger::severity::debug);
    }

    avl_tree(const avl_tree<tkey, tvalue, tkey_comparer>& tree){
        this->_logger = tree._logger;
        this->_allocator = tree._allocator;
        this->_find = new typename binary_search_tree<tkey, tvalue, tkey_comparer>::find_template_method();
        this->_insert = new avl_insert_template_method(this);
        this->_remove = new avl_remove_template_method(this);
        this->_root = tree.copy();
        if(this->_logger != nullptr) this->_logger->log("AVL_tree COPIED!", logger::severity::debug);
    }
    avl_tree(avl_tree<tkey, tvalue, tkey_comparer>&& tree) noexcept {
        this->_root = tree._root;
        this->_allocator = tree._allocator;
        this->_logger = tree._logger;
        this->_find = new typename binary_search_tree<tkey, tvalue, tkey_comparer>::find_template_method();
        this->_insert = new avl_insert_template_method(this);
        this->_remove = new avl_remove_template_method(this);
        delete tree._find;
        delete tree._insert;
        delete tree._remove;
        tree._allocator = nullptr;
        tree._logger = nullptr;
        tree._insert = nullptr;
        tree._find = nullptr;
        tree._remove = nullptr;
        tree._root = nullptr;
        if(this->_logger) this->_logger->log("AVL_tree MOVED!", logger::severity::debug);
    }

    avl_tree<tkey, tvalue, tkey_comparer>& operator=(const avl_tree<tkey, tvalue, tkey_comparer>& tree){
        if(this != &tree){
            this->clear();
            this->_root = tree.copy();
        }
        if(this->_logger != nullptr) this->_logger->log("AVL_tree ASSIGNED!", logger::severity::debug);
        return *this;
    }

    avl_tree<tkey, tvalue, tkey_comparer>& operator=(avl_tree<tkey, tvalue, tkey_comparer>&& tree) noexcept {
        if(&tree != this){
            this->clear();
            this->_root = tree._root;
            this->_logger = tree._logger;
            this->_allocator = tree._allocator;
            delete tree._find;
            delete tree._insert;
            delete tree._remove;
            tree._root = nullptr;
            tree._allocator = nullptr;
            tree._logger = nullptr;
            tree._insert = nullptr;
            tree._find = nullptr;
            tree._remove = nullptr;
        }
        if(this->_logger != nullptr) this->_logger->log("AVL_tree MOVED!", logger::severity::debug);
        return *this;
    }

    ///*-------------------- TREE DESTRUCTOR FIELD --------------------*///

    ~avl_tree(){
        if(this->_logger) this->_logger->log("AVL_tree DELETED!", logger::severity::debug);
    }

};


#endif //AVL_TREE_AVL_TREE_H