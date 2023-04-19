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

private:

    node* _root;
    abstract_allocator* _allocator;
    logger* _logger;

public:

    explicit binary_search_tree(abstract_allocator* alloc = nullptr, logger* logger = nullptr):
            _root(nullptr), _allocator(alloc), _logger(logger)
    {
        if(_logger != nullptr) _logger->log("Binary_search_tree CREATED", logger::severity::debug);
        ///TODO
    }

    binary_search_tree(const binary_search_tree<tkey, tvalue, tkey_comparer>& tree) = delete;

    binary_search_tree(binary_search_tree<tkey, tvalue, tkey_comparer>&& tree) = delete;

    binary_search_tree<tkey, tvalue, tkey_comparer>& operator=(const binary_search_tree<tkey, tvalue, tkey_comparer>& tree) = delete;

    binary_search_tree<tkey, tvalue, tkey_comparer>& operator=(binary_search_tree<tkey, tvalue, tkey_comparer>&& tree) = delete;

    ~binary_search_tree() override {
        ///TODO
    }

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
                tkey_comparer comparer;
                node* search = _tree_root;
                while(search != _current_node){
                    _path.push(search);
                    if(comparer(_current_node, search) > 0) search = search->right;
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

        tuple<tkey const &, tvalue const &, unsigned int> operator*(){
            return tuple<tkey const &, tvalue const &, unsigned int>(_current_node->key, _current_node->value, _path.size());
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

        explicit inf_iterator(node* root, node* current):
                _tree_root(root), _last_node(root)
        {
            if(_tree_root != nullptr){
                while(_last_node->right != nullptr) _last_node = _last_node->right;
            }
            _path = stack<node*>();
            if(current != nullptr){
                _current_node = current;
                tkey_comparer comparer;
                node* search = _tree_root;
                while(search != _current_node){
                    _path.push(search);
                    if(comparer(_current_node, search) > 0) search = search->right;
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

        tuple<tkey const &, tvalue const &, unsigned int> operator*(){
            return tuple<tkey const &, tvalue const &, unsigned int>(_current_node->key, _current_node->value, _path.size());
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
                tkey_comparer comparer;
                node* search = _tree_root;
                while(search != _current_node){
                    _path.push(search);
                    if(comparer(_current_node, search) > 0) search = search->right;
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

        tuple<tkey const &, tvalue const &, unsigned int> operator*(){
            return tuple<tkey const &, tvalue const &, unsigned int>(_current_node->key, _current_node->value, _path.size());
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

};


#endif //BINARY_SEARCH_TREE_BINARY_SEARCH_TREE_H
