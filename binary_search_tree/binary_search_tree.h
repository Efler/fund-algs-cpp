#ifndef BINARY_SEARCH_TREE_BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_BINARY_SEARCH_TREE_H
#include "associative_container.h"
#include "../allocator_2/abstract_allocator.h"
#include "../logger/logger_builder.h"
#include "make_string.h"
#include "debug_tree_printing.h"   ///TODO DEBUG!!!!!!!!!!!!!!!!!!!!
#include <stack>


template <typename tkey, typename tvalue, typename tkey_comparer>
class binary_search_tree: public associative_container<tkey, tvalue>
{

protected:

    struct node{
        node() = default;
        node(tkey const &k, tvalue const &v, node* l, node* r):
            key(k), value(v), left(l), right(r){}

        tkey key;
        tvalue value;
        node* left;
        node* right;

        virtual ~node() = default;
    };

public:

    class pref_iterator final
    {

        friend class binary_search_tree<tkey, tvalue, tkey_comparer>;

    private:

        node* _tree_root;
        node* _current_node;
        node* _last_node;
        stack<node*> _path;

    private:

        explicit pref_iterator(node* root, node* current = nullptr):
            _tree_root(root), _last_node(root)
        {
            if(_tree_root != nullptr){
                while(_last_node->right != nullptr) _last_node = _last_node->right;
            }
            _path = stack<node*>();
            if(current != nullptr){
                _current_node = current;
                tkey_comparer comparer = tkey_comparer();
                node* search = _tree_root;
                while(search != _current_node){
                    _path.push(search);
                    if(comparer(_current_node->key, search->key) > 0) search = search->right;
                    else search = search->left;
                }
            }else{
                _current_node = nullptr;
            }
        }

        bool operator==(pref_iterator const & other) const {
            return _current_node == other._current_node && _path == other._path;
        }

        bool operator!=(pref_iterator const & other) const {
            return !(*this == other);
        }

        tuple<tkey const &, tvalue const &, unsigned int, node*> operator*(){
            return tuple<tkey const &, tvalue const &, unsigned int, node*>(_current_node->key, _current_node->value, _path.size(), _current_node);
        }

        pref_iterator& operator++(){
            if(_current_node == nullptr) return *this;
            if(_current_node == _last_node){
                if(_tree_root == _last_node && _tree_root->left != nullptr){
                    _path.push(_current_node);
                    _current_node = _current_node->left;
                }else{
                    _current_node = nullptr;
                    _path = stack<node*>();
                }
                return *this;
            }else if(_current_node->left != nullptr){
                _path.push(_current_node);
                _current_node = _current_node->left;
                return *this;
            }else if(_current_node->left == nullptr && _current_node->right != nullptr){
                _path.push(_current_node);
                _current_node = _current_node->right;
                return *this;
            }else if(_current_node->left == nullptr && _current_node->right == nullptr){
                while(!_path.empty() && ((_path.top()->left == _current_node && _path.top()->right == nullptr) || _path.top()->right == _current_node)){
                    _current_node = _path.top();
                    _path.pop();
                }
                if(_path.empty()){
                    if(_tree_root->right == nullptr){
                        _current_node = nullptr;
                        _path = stack<node*>();
                        return *this;
                    }else _current_node = _tree_root->right;
                }
                else _current_node = _path.top()->right;
                return *this;
            }
        }

        pref_iterator operator++(int a){
            pref_iterator previous_state = *this;
            ++(*this);
            return previous_state;
        }

    };

    pref_iterator begin_pref() const {
        return pref_iterator(_root, _root);
    }

    pref_iterator end_pref() const {
        return pref_iterator(_root, nullptr);
    }


    class inf_iterator final
    {

        friend class binary_search_tree<tkey, tvalue, tkey_comparer>;

    private:

        node* _tree_root;
        node* _current_node;
        node* _last_node;
        stack<node*> _path;

    private:

        inf_iterator(node* root, node* current):
                _tree_root(root), _last_node(root)
        {
            if(_tree_root != nullptr){
                while(_last_node->right != nullptr) _last_node = _last_node->right;
            }
            _path = stack<node*>();
            if(current != nullptr){
                _current_node = current;
                tkey_comparer comparer = tkey_comparer();
                node* search = _tree_root;
                while(search != _current_node){
                    _path.push(search);
                    if(comparer(_current_node->key, search->key) > 0) search = search->right;
                    else search = search->left;
                }
            }else{
                _current_node = nullptr;
            }
        }

        bool operator==(inf_iterator const & other) const {
            if (_current_node == other._current_node && _path == other._path) return true;
            else return false;
        }

        bool operator!=(inf_iterator const & other) const {
            return !(*this == other);
        }

        tuple<tkey const &, tvalue const &, unsigned int, node*> operator*(){
            return tuple<tkey const &, tvalue const &, unsigned int, node*>(_current_node->key, _current_node->value, _path.size(), _current_node);
        }

        inf_iterator& operator++(){
            if(_current_node == nullptr) return *this;
            if(_current_node == _last_node) {
                _current_node = nullptr;
                _path = stack<node*>();
                return *this;
            }else if(_current_node->right != nullptr){
                _path.push(_current_node);
                _current_node = _current_node->right;
                while(_current_node->left != nullptr){
                    _path.push(_current_node);
                    _current_node = _current_node->left;
                }
            }else{
                if (_path.top()->left == _current_node){
                    _current_node = _path.top();
                    _path.pop();
                }else{
                    while (!_path.empty() && _path.top()->right == _current_node){
                        _current_node = _path.top();
                        _path.pop();
                    }
                    _current_node = _path.top();
                    _path.pop();
                }
            }
            return *this;
        }

        inf_iterator operator++(int a){
            inf_iterator previous_state = *this;
            ++(*this);
            return previous_state;
        }

    };

    inf_iterator begin_inf() const {
        node* start = _root;
        while(start != nullptr && start->left != nullptr) start = start->left;
        return inf_iterator(_root, start);
    }

    inf_iterator end_inf() const {
        return inf_iterator(_root, nullptr);
    }


    class postf_iterator final
    {

        friend class binary_search_tree<tkey, tvalue, tkey_comparer>;

    private:

        node* _tree_root;
        node* _current_node;
        node* _last_node;
        stack<node*> _path;

    private:

        explicit postf_iterator(node* root, node* current):
                _tree_root(root), _last_node(root)
        {
            if(_tree_root != nullptr){
                while(_last_node->right != nullptr) _last_node = _last_node->right;
            }
            _path = stack<node*>();
            if(current != nullptr){
                _current_node = current;
                tkey_comparer comparer = tkey_comparer();
                node* search = _tree_root;
                while(search != _current_node){
                    _path.push(search);
                    if(comparer(_current_node->key, search->key) > 0) search = search->right;
                    else search = search->left;
                }
            }else{
                _current_node = nullptr;
            }
        }

        bool operator==(postf_iterator const & other) const {
            if (_current_node == other._current_node && _path == other._path) return true;
            else return false;
        }

        bool operator!=(postf_iterator const & other) const {
            return !(*this == other);
        }

        tuple<tkey const &, tvalue const &, unsigned int, node*> operator*(){
            return tuple<tkey const &, tvalue const &, unsigned int, node*>(_current_node->key, _current_node->value, _path.size(), _current_node);
        }

        postf_iterator& operator++(){
            if(_current_node == nullptr) return *this;
            if(!_path.empty()){
                if(_path.top()->right == _current_node){
                    _current_node = _path.top();
                    _path.pop();
                }else{
                    if(_path.top()->right != nullptr){
                        _current_node = _path.top()->right;
                        while(_current_node->left != nullptr || _current_node->right != nullptr){
                            _path.push(_current_node);
                            if(_current_node->left == nullptr) _current_node = _current_node->right;
                            else _current_node = _current_node->left;
                        }
                    }else{
                        _current_node = _path.top();
                        _path.pop();
                    }
                }
            }else _current_node = nullptr;
            return *this;
        }

        postf_iterator operator++(int a){
            postf_iterator previous_state = *this;
            ++(*this);
            return previous_state;
        }

    };

    postf_iterator begin_postf() const {
        node* start = _root;
        if(_root != nullptr){
            if(_root->left == nullptr) while (start->left == nullptr && start->right != nullptr) start = start->right;
            while(start->left != nullptr || start->right != nullptr){
                if(start->left == nullptr) start = start->right;
                else start = start->left;
            }
        }
        return postf_iterator(_root, start);
    }

    postf_iterator end_postf() const {
        return postf_iterator(_root, nullptr);
    }


public:

    bool find(typename associative_container<tkey, tvalue>::key_value_pair* target_key_and_result_value) override {
        bool result;
        result = _find->find_concrete(target_key_and_result_value, _root);
        _find->after_find(target_key_and_result_value, _root);
        return result;
    }

protected:

    class find_template_method
    {
    public:

        bool find_concrete(typename associative_container<tkey, tvalue>::key_value_pair* target_key_and_result_value, node* root) const {
            node* current = root;
            tkey_comparer comparer = tkey_comparer();
            if(current != nullptr){
                if(comparer(target_key_and_result_value->_key, current->key) == 0 && target_key_and_result_value->_value == current->value) return true;
                while((current->left != nullptr && comparer(target_key_and_result_value->_key, current->key) < 0) || (current->right != nullptr && comparer(target_key_and_result_value->_key, current->key) > 0)){
                    if(comparer(target_key_and_result_value->_key, current->key) > 0) current = current->right;
                    else current = current->left;
                    if(comparer(target_key_and_result_value->_key, current->key) == 0 && target_key_and_result_value->_value == current->value) return true;
                }
            }
            return false;
        }

        virtual void after_find(typename associative_container<tkey, tvalue>::key_value_pair* target_key_and_result_value, node* root){}

    public:

        virtual ~find_template_method() noexcept = default;

    };

public:

    void insert(const tkey &key, const tvalue &value) override {
        stack<node*> _insert_path = stack<node*>();
        node** p_root;
        p_root = &_root;
        _insert->insert_concrete(key, value, p_root, this->_allocator, this->_logger, &_insert_path);
        _insert->after_insert(key, value, p_root, this->_logger, &_insert_path);
    }

protected:

    class insert_template_method
    {
    public:

        explicit insert_template_method(binary_search_tree<tkey, tvalue, tkey_comparer>* this_bst):
            _this(this_bst){}

    private:

        binary_search_tree<tkey, tvalue, tkey_comparer>* _this;

    protected:

        virtual size_t get_node_size() const {
            return reinterpret_cast<size_t>(sizeof(node));
        }

        virtual void get_node_constructor(node** x, const tkey& key, const tvalue& value) const {
            new (*x) node{key, value, nullptr, nullptr};
        }

    public:

        void insert_concrete(const tkey &key, const tvalue &value, node** root, abstract_allocator* alloc, logger* logger, stack<node*>* _insert_path){
            node* current = *root;
            size_t turn;
            tkey_comparer comparer = tkey_comparer();
            if(*root == nullptr){
                *root = reinterpret_cast<node*>(alloc->allocate(get_node_size()));
                get_node_constructor(root, key, value);
                _insert_path->push(*root);
                if(logger != nullptr) logger->log("Tree node created", logger::severity::debug);
                return;
            }
            while(current != nullptr){
                _insert_path->push(current);
                int comp_result = comparer(key, current->key);
                if(comp_result == 0){
                    throw logic_error("Duplicate key!");
                }
                else if(comp_result > 0){
                    turn = 1;
                    current = current->right;
                }else{
                    turn = 0;
                    current = current->left;
                }
            }

            node** temp =
                    turn == 0  ? &_insert_path->top()->left
                               : &_insert_path->top()->right;

            (*temp) = reinterpret_cast<node*>(alloc->allocate(get_node_size()));
            get_node_constructor(temp, key, value);
            _insert_path->push(*temp);
            if(logger != nullptr) logger->log("Tree node created", logger::severity::debug);
        }

        virtual void after_insert(const tkey &key, const tvalue &value, node** root, logger* logger, stack<node*>* _insert_path){
            while(!_insert_path->empty()) _insert_path->pop();
            if(logger != nullptr) logger->log("after_insert: stack cleared", logger::severity::debug);
        }

    public:

        virtual ~insert_template_method() = default;

    };

public:

    tvalue remove(const tkey &key) override {
        if(_root == nullptr){
            throw logic_error("Tree is empty!");
        }
        node** p_root;
        p_root = &_root;
        tvalue result = _remove->remove_concrete(key, p_root, this->_allocator, this->_logger);
        _remove->after_remove(key, p_root, this->_logger);
        return result;
    }

protected:

    class remove_template_method
    {
    private:

        stack<node*> _remove_path = stack<node*>();   ////TODO: LOCAL STACK

    protected:

        virtual void get_node_destructor(node* x) const {
            x->~node();
        }

    public:

        tvalue remove_concrete(const tkey &key, node** root, abstract_allocator* alloc, logger* logger){
            node* current = *root;
            size_t turn = 2;
            tkey_comparer comparer = tkey_comparer();
            while(current != nullptr && (comparer(key, current->key) != 0)){
                _remove_path.push(current);
                if(comparer(key, current->key) > 0){
                    current = current->right;
                    turn = 1;
                }
                else{
                    current = current->left;
                    turn = 0;
                }
            }
            if (current == nullptr)
            {
                throw logic_error("Key not found!");
            }
            if(current->right == nullptr && current->left == nullptr){
                if(turn == 0){
                    _remove_path.top()->left = nullptr;
                }else if(turn == 1){
                    _remove_path.top()->right = nullptr;
                }
                tvalue result = std::move(current->value);
                get_node_destructor(current);
                alloc->deallocate(reinterpret_cast<void*>(current));
                if(turn == 2) *root = nullptr;
                if(logger != nullptr) logger->log("Tree node deleted", logger::severity::debug);
                return result;
            }else if((current->right != nullptr && current->left == nullptr) || (current->right == nullptr && current->left != nullptr)){
                if(current->right == nullptr && current->left != nullptr){
                    if(current == *root) *root = current->left;
                    else turn == 0 ? _remove_path.top()->left = current->left : _remove_path.top()->right = current->left;
                }else{
                    if(current == *root) *root = current->right;
                    else turn == 0 ? _remove_path.top()->left = current->right : _remove_path.top()->right = current->right;
                }
                tvalue result = std::move(current->value);
                get_node_destructor(current);
                alloc->deallocate(reinterpret_cast<void*>(current));
                if(logger != nullptr) logger->log("Tree node deleted", logger::severity::debug);
                return result;
            }else if(current->right != nullptr && current->left != nullptr){
                node* next_node = current;
                _remove_path.push(current);
                next_node = next_node->right;
                if(next_node->left == nullptr){
                    tvalue result = std::move(current->value);
                    current->key = next_node->key;
                    current->value = next_node->value;
                    current->right = next_node->right;
                    get_node_destructor(current);
                    alloc->deallocate(reinterpret_cast<void*>(next_node));
                    if(logger != nullptr) logger->log("Tree node deleted", logger::severity::debug);
                    return result;
                }else{
                    while(next_node->left != nullptr){
                        _remove_path.push(next_node);
                        next_node = next_node->left;
                    }
                    tvalue result = std::move(current->value);
                    current->key = next_node->key;
                    current->value = next_node->value;
                    _remove_path.top()->left = next_node->right;
                    get_node_destructor(current);
                    alloc->deallocate(reinterpret_cast<void*>(next_node));
                    if(logger != nullptr) logger->log("Tree node deleted", logger::severity::debug);
                    return result;
                }
            }
        }

        virtual void after_remove(const tkey &key, node** root, logger* logger){
            while(!_remove_path.empty()) _remove_path.pop();
            logger->log("after_remove: stack cleared", logger::severity::debug);
        }

    public:

        virtual ~remove_template_method() = default;

    };

protected:

    void rotate_left(node** subtree_root) const {
        node* tmp = (*subtree_root)->right;
        (*subtree_root)->right = tmp->left;
        tmp->left = (*subtree_root);
        (*subtree_root) = tmp;
    }

    void rotate_right(node** subtree_root) const {
        node* tmp = (*subtree_root)->left;
        (*subtree_root)->left = tmp->right;
        tmp->right = (*subtree_root);
        (*subtree_root) = tmp;
    }

public:

    const tvalue& get(const tkey &key) const override {
        node* current = _root;
        if(current == nullptr){
            throw logic_error("Tree is empty!");
        }
        tkey_comparer comparer = tkey_comparer();
        if(comparer(key, current->key) == 0) return current->value;
        while((current->left != nullptr && comparer(key, current->key) < 0) || (current->right != nullptr && comparer(key, current->key) > 0)){
            if(comparer(key, current->key) > 0) current = current->right;
            else current = current->left;
            if(comparer(key, current->key) == 0) return current->value;
        }
        throw logic_error("Key not found!");
    }

protected:

    virtual void debug_tree_printing_function(node* root) const {
        debug_tree_printing<tkey, tvalue>(reinterpret_cast<void*>(root));        ////TODO DEBUG!!!!!!!!!!!!!!
    }

public:

    void bypass(typename associative_container<tkey,tvalue>::bypass_mode mode) const override {
        string msg;
        switch(mode){
            case associative_container<tkey,tvalue>::bypass_mode::prefix:
                for (auto begin = begin_pref(); begin != end_pref(); ++begin){
                    msg += make_string(std::get<0>(*begin));
                    msg += " : ";
                    msg += make_string(std::get<1>(*begin));
                    msg += "   ";
                }
                _logger->log(msg, logger::severity::debug);
                debug_tree_printing_function(_root);                                       ////TODO DEBUG!!!!!!!!!!!!!!
                break;
            case associative_container<tkey,tvalue>::bypass_mode::infix:
                for (auto begin = begin_inf(); begin != end_inf(); ++begin){
                    msg += make_string(std::get<0>(*begin));
                    msg += " : ";
                    msg += make_string(std::get<1>(*begin));
                    msg += "   ";
                }
                _logger->log(msg, logger::severity::debug);
                debug_tree_printing_function(_root);                                       ////TODO DEBUG!!!!!!!!!!!!!!
                break;
            case associative_container<tkey,tvalue>::bypass_mode::postfix:
                for (auto begin = begin_postf(); begin != end_postf(); ++begin){
                    msg += make_string(std::get<0>(*begin));
                    msg += " : ";
                    msg += make_string(std::get<1>(*begin));
                    msg += "   ";
                }
                _logger->log(msg, logger::severity::debug);
                debug_tree_printing_function(_root);                                       ////TODO DEBUG!!!!!!!!!!!!!!
                break;
            default:
                return;
        }
    }


private:

    node* _root;
    abstract_allocator* _allocator;
    logger* _logger;
    find_template_method* _find;
    insert_template_method* _insert;
    remove_template_method* _remove;

public:

    explicit binary_search_tree(abstract_allocator* alloc = nullptr, logger* logger = nullptr):
            binary_search_tree<tkey, tvalue, tkey_comparer>(alloc, logger, new find_template_method(), new insert_template_method(this),new remove_template_method())
    {
        if(_logger != nullptr) _logger->log("Binary_search_tree CREATED!", logger::severity::debug);
    }

protected:

    binary_search_tree(abstract_allocator* alloc, logger* logger, find_template_method *find, insert_template_method *insert, remove_template_method *remove):
            _allocator(alloc), _logger(logger), _find(find), _insert(insert), _remove(remove), _root(nullptr){}

public:

    binary_search_tree(const binary_search_tree<tkey, tvalue, tkey_comparer>& tree):
            _root(nullptr), _allocator(tree._allocator), _logger(tree._logger)
    {
        _find = new find_template_method();
        _insert = new insert_template_method(this);
        _remove = new remove_template_method();
        auto it = tree.begin_pref();
        auto end = tree.end_pref();
        for(; it != end; ++it){
            this->insert(std::get<0>(*it), std::get<1>(*it));
        }
        if(_logger != nullptr) _logger->log("Binary_search_tree CREATED!", logger::severity::debug);
    };

    binary_search_tree(binary_search_tree<tkey, tvalue, tkey_comparer>&& tree) noexcept :
            _root(tree._root), _allocator(tree._allocator), _logger(tree._logger)
    {
        _find = new find_template_method();
        _insert = new insert_template_method(this);
        _remove = new remove_template_method();
        tree._root = nullptr;
    };

    binary_search_tree<tkey, tvalue, tkey_comparer>& operator=(const binary_search_tree<tkey, tvalue, tkey_comparer>& tree){
        if(this != &tree){
            postf_iterator begin = begin_postf();
            postf_iterator end = end_postf();
            for(; begin != end; ++begin){
                std::get<3>(*begin)->~node();
                _allocator->deallocate(reinterpret_cast<void*>(std::get<3>(*begin)));
                if(_logger != nullptr) _logger->log("OPERATOR=: Tree node deleted!", logger::severity::debug);
            }
            auto it = tree.begin_pref();
            auto t_end = tree.end_pref();
            for(; it != t_end; ++it){
                this->insert(std::get<0>(*it), std::get<1>(*it));
            }
            if(_logger != nullptr) _logger->log("Binary_search_tree COPIED!", logger::severity::debug);
        }

        return *this;
    }

    binary_search_tree<tkey, tvalue, tkey_comparer>& operator=(binary_search_tree<tkey, tvalue, tkey_comparer>&& tree) noexcept {
        if(&tree != this){
            postf_iterator begin = begin_postf();
            postf_iterator end = end_postf();
            for(; begin != end; ++begin){
                std::get<3>(*begin)->~node();
                _allocator->deallocate(reinterpret_cast<void*>(std::get<3>(*begin)));
                if(_logger != nullptr) _logger->log("OPERATOR=: Tree node deleted!", logger::severity::debug);
            }
            this->_allocator = tree._allocator;
            this->_root = tree._root;
            tree._root = nullptr;
            if(_logger != nullptr) _logger->log("Binary_search_tree MOVED!", logger::severity::debug);
        }

        return *this;
    }

    ~binary_search_tree(){
        postf_iterator begin = begin_postf();
        postf_iterator end = end_postf();
        for(; begin != end; ++begin){
            std::get<3>(*begin)->~node();
            _allocator->deallocate(reinterpret_cast<void*>(std::get<3>(*begin)));
            if(_logger != nullptr) _logger->log("DESTRUCTOR: Tree node deleted!", logger::severity::debug);
        }

        delete _find;
        if(_logger != nullptr) _logger->log("DESTRUCTOR: find_template_method deleted!", logger::severity::debug);
        delete _insert;
        if(_logger != nullptr) _logger->log("DESTRUCTOR: insert_template_method deleted!", logger::severity::debug);
        delete _remove;
        if(_logger != nullptr) _logger->log("DESTRUCTOR: remove_template_method deleted!", logger::severity::debug);
        if(_logger != nullptr) _logger->log("DESTRUCTOR: Binary_search_tree DELETED!", logger::severity::debug);
    }

};


#endif //BINARY_SEARCH_TREE_BINARY_SEARCH_TREE_H