#ifndef BINARY_SEARCH_TREE_ASSOCIATIVE_CONTAINER_H
#define BINARY_SEARCH_TREE_ASSOCIATIVE_CONTAINER_H


template <typename tkey, typename tvalue, typename tkey_comparer>
class associative_container
{

public:

    struct key_value_pair{
        tkey _key;
        tvalue _value;
    };

    virtual bool find(key_value_pair* target_key_and_result_value) = 0;

    virtual void insert(const tkey &key, const tvalue &value) = 0;

    virtual tvalue* remove(const tkey &key) = 0;

    bool operator[](key_value_pair* target_key_and_result_value){
        return find(target_key_and_result_value);
    }

    void operator+=(key_value_pair& pair){
        insert(pair._key, pair._value);
    }

    tvalue* operator-=(const tkey &key){
        return remove(key);
    }

    virtual ~associative_container() = 0;

};


#endif //BINARY_SEARCH_TREE_ASSOCIATIVE_CONTAINER_H
