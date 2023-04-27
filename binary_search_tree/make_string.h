#ifndef BINARY_SEARCH_TREE_MAKE_STRING_H
#define BINARY_SEARCH_TREE_MAKE_STRING_H
#include <iostream>


template <typename T>
std::string make_string(T a){
    std::stringstream stream;
    stream << a;
    return stream.str();
}


#endif //BINARY_SEARCH_TREE_MAKE_STRING_H
