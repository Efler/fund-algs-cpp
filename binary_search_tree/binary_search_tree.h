#ifndef BINARY_SEARCH_TREE_BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_BINARY_SEARCH_TREE_H
#include "associative_container.h"
#include "../allocator_2/abstract_allocator.h"
#include "../logger/logger_builder.h"
#include <stack>


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

public:

    class pref_iterator final
    {

    private:

        node* _tree_root;
        node* _current_node;
        node* _last_node;
        stack<node*> _path;

    public:

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
            if (_current_node == other._current_node && _path == other._path) return true;
            else return false;
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
                _current_node = nullptr;
                _path = stack<node*>();
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
                while(!_path.empty() && _path.top()->right == _current_node){
                    _current_node = _path.top();
                    _path.pop();
                }
                _current_node = _path.top()->right;
                return *this;
            }
        }

        pref_iterator& operator++(int a){
            pref_iterator& previous_state = *this;
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

    ///-----------------------------------------------------

    class inf_iterator final
    {

    private:

        node* _tree_root;
        node* _current_node;
        node* _last_node;
        stack<node*> _path;

    public:

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
            }else{ ///////----------------------------мб приколы
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

        inf_iterator& operator++(int a){
            inf_iterator& previous_state = *this;
            ++(*this);
            return previous_state;
        }

    };

    inf_iterator begin_inf() const {
        node* start = _root;
        while(start->left != nullptr) start = start->left;
        return inf_iterator(_root, start);
    }

    inf_iterator end_inf() const {
        return inf_iterator(_root, nullptr);
    }

    ///-----------------------------------------------------

    class postf_iterator final
    {

    private:

        node* _tree_root;
        node* _current_node;
        node* _last_node;
        stack<node*> _path;

    public:

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

        postf_iterator& operator++(int a){
            postf_iterator& previous_state = *this;
            ++(*this);
            return previous_state;
        }

    };

    postf_iterator begin_postf() const {
        node* start = _root;
        if(_root->left != nullptr) while (start->left != nullptr) start = start->left;
        else{
            while(start->left == nullptr && start->right != nullptr) start = start->right;
            while (start->left != nullptr) start = start->left;
        }
        return postf_iterator(_root, start);
    }

    postf_iterator end_postf() const {
        return postf_iterator(_root, nullptr);
    }

    ///-----------------------------------------------------

    bool find(typename associative_container<tkey, tvalue, tkey_comparer>::key_value_pair* target_key_and_result_value) override {
        bool result;
        try{
            result = _find->find_concrete(target_key_and_result_value, this);
            _find->after_find(target_key_and_result_value, this);
        }catch(logic_error& ex){
            _logger->log(ex.what(), logger::severity::debug);
        }
        return result;
    }

protected:

    class find_template_method
    {
    public:

        virtual bool find_concrete(typename associative_container<tkey, tvalue, tkey_comparer>::key_value_pair* target_key_and_result_value, binary_search_tree<tkey, tvalue, tkey_comparer>* bst_tree) const {
            inf_iterator begin = bst_tree->begin_inf();
            inf_iterator end = bst_tree->end_inf();
            if(begin == end){
                throw logic_error("Tree is empty!");
            }
            while(begin != end){
                if(target_key_and_result_value->_key == get<0>(*begin) && target_key_and_result_value->_value == get<1>(*begin)){
                    return true;
                }
                ++begin;
            }
            return false;
        }

        virtual void after_find(typename associative_container<tkey, tvalue, tkey_comparer>::key_value_pair* target_key_and_result_value, binary_search_tree<tkey, tvalue, tkey_comparer>* bst_tree){

        }

    public:

        virtual ~find_template_method() = default;

    };

public:

    void insert(const tkey &key, const tvalue &value) override {
        if(_root == nullptr){
            _root = reinterpret_cast<node*>(_allocator->allocate(sizeof(node)));
            new (_root) node{key, move(value), nullptr, nullptr};
            if(_logger != nullptr) _logger->log("Tree node created", logger::severity::debug);
            return;
        }
        _insert->insert_concrete(key, value, _root, this->_allocator, this->_logger);
        _insert->after_insert(key, value, _root, this->_logger);
    }

protected:

    class insert_template_method
    {
    private:

        stack<node*> _insert_path = stack<node*>();

    public:

        virtual void insert_concrete(const tkey &key, const tvalue &value, node* root, abstract_allocator* alloc, logger* logger){
            node* current = root;
            size_t turn;
            tkey_comparer comparer = tkey_comparer();
            while(current != nullptr){
                _insert_path.push(current);
                if(comparer(key, current->key) > 0){
                    turn = 1;
                    current = current->right;
                }else{
                    turn = 0;
                    current = current->left;
                }
            }
            if(turn == 0){
                _insert_path.top()->left = reinterpret_cast<node*>(alloc->allocate(sizeof(node)));
                _insert_path.top()->left->key = key;
                _insert_path.top()->left->value = value;
                _insert_path.top()->left->left = nullptr;
                _insert_path.top()->left->right = nullptr;
                _insert_path.push(_insert_path.top()->left);
                if(logger != nullptr) logger->log("Tree node created", logger::severity::debug);
            }else{
                _insert_path.top()->right = reinterpret_cast<node*>(alloc->allocate(sizeof(node)));
                _insert_path.top()->right->key = key;
                _insert_path.top()->right->value = value;
                _insert_path.top()->right->left = nullptr;
                _insert_path.top()->right->right = nullptr;
                _insert_path.push(_insert_path.top()->right);
                if(logger != nullptr) logger->log("Tree node created", logger::severity::debug);
            }
        }

        virtual void after_insert(const tkey &key, const tvalue &value, node* root, logger* logger){
            while(!_insert_path.empty()) _insert_path.pop();
            if(logger != nullptr) logger->log("after_insert: stack cleared", logger::severity::debug);
        }

    public:

        virtual ~insert_template_method() = default;

    };

public:

    tvalue&& remove(const tkey &key) override {
        if(_root == nullptr){
            if(_logger != nullptr) _logger->log("Tree is empty!", logger::severity::debug);
            throw logic_error("Tree is empty!");
        }
        node** p_root;
        *p_root = _root;
        tvalue&& result = move(_remove->remove_concrete(key, p_root, this->_allocator, this->_logger));
        _remove->after_remove(key, p_root, this->_logger);
        return move(result);
    }

protected:

    class remove_template_method
    {
    private:

        stack<node*> _remove_path = stack<node*>();

    public:

        virtual tvalue&& remove_concrete(const tkey &key, node** root, abstract_allocator* alloc, logger* logger){
            node* current = *root;
            size_t turn;
            tkey_comparer comparer = tkey_comparer();
            while(current->key != key){
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
            if(current->right == nullptr && current->left == nullptr){
                if(turn == 0){
                    _remove_path.top()->left = nullptr;
                }else{
                    _remove_path.top()->right = nullptr;
                }
                tvalue&& result = move(current->value);
                alloc->deallocate(reinterpret_cast<void*>(current));
                if(logger != nullptr) logger->log("Tree node deleted", logger::severity::debug);
                return move(result);
            }else if((current->right != nullptr && current->left == nullptr) || (current->right == nullptr && current->left != nullptr)){
                if(current->right == nullptr && current->left != nullptr){
                    _remove_path.top()->left = current->left;
                }else{
                    _remove_path.top()->right = current->right;
                }
                tvalue&& result = move(current->value);
                alloc->deallocate(reinterpret_cast<void*>(current));
                if(logger != nullptr) logger->log("Tree node deleted", logger::severity::debug);
                return move(result);
            }else if(current->right != nullptr && current->left != nullptr){
                node* next_node = current;
                _remove_path.push(current);
                next_node = next_node->right;
                if(next_node->left == nullptr){
                    current->key = next_node->key;
                    current->value = next_node->value;
                    current->right = next_node->right;
                    tvalue&& result = move(next_node->value);
                    alloc->deallocate(reinterpret_cast<void*>(next_node));
                    if(logger != nullptr) logger->log("Tree node deleted", logger::severity::debug);
                    return move(result);
                }else{
                    while(next_node->left != nullptr){
                        _remove_path.push(next_node);
                        next_node = next_node->left;
                    }
                    current->key = next_node->key;
                    current->value = next_node->value;
                    _remove_path.top()->left = next_node->right;
                    tvalue&& result = move(next_node->value);
                    alloc->deallocate(reinterpret_cast<void*>(next_node));
                    if(logger != nullptr) logger->log("Tree node deleted", logger::severity::debug);
                    return move(result);
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

public:

    void bypass() const override {
        for (auto it = begin_postf(); it != end_postf() ; ++it){
            cout << get<0>(*it) << " ";
        }
        cout << endl;
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
            _root(nullptr), _allocator(alloc), _logger(logger)
    {
        _find = reinterpret_cast<find_template_method*>(_allocator->allocate(sizeof(find_template_method)));
        _insert = reinterpret_cast<insert_template_method*>(_allocator->allocate(sizeof(insert_template_method)));
        _remove = reinterpret_cast<remove_template_method*>(_allocator->allocate(sizeof(remove_template_method)));
        if(_logger != nullptr) _logger->log("Binary_search_tree CREATED", logger::severity::debug);
    }

    binary_search_tree(const binary_search_tree<tkey, tvalue, tkey_comparer>& tree) = delete;

    binary_search_tree(binary_search_tree<tkey, tvalue, tkey_comparer>&& tree) = delete;

    binary_search_tree<tkey, tvalue, tkey_comparer>& operator=(const binary_search_tree<tkey, tvalue, tkey_comparer>& tree) = delete;

    binary_search_tree<tkey, tvalue, tkey_comparer>& operator=(binary_search_tree<tkey, tvalue, tkey_comparer>&& tree) = delete;

    ~binary_search_tree(){
        postf_iterator begin = begin_postf();
        postf_iterator end = end_postf();
        for(begin; begin != end; ++begin){
            _allocator->deallocate(reinterpret_cast<void*>(get<3>(*begin)));
            if(_logger != nullptr) _logger->log("DESTRUCTOR: Tree node deleted!", logger::severity::debug);
        }
        _allocator->deallocate(reinterpret_cast<void*>(_find));
        if(_logger != nullptr) _logger->log("DESTRUCTOR: find_template_method deleted!", logger::severity::debug);
        _allocator->deallocate(reinterpret_cast<void*>(_insert));
        if(_logger != nullptr) _logger->log("DESTRUCTOR: insert_template_method deleted!", logger::severity::debug);
        _allocator->deallocate(reinterpret_cast<void*>(_remove));
        if(_logger != nullptr) _logger->log("DESTRUCTOR: remove_template_method deleted!", logger::severity::debug);
    }

};


#endif //BINARY_SEARCH_TREE_BINARY_SEARCH_TREE_H
