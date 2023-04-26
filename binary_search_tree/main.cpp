#include <iostream>
#include "binary_search_tree.h"
#include "../allocator_1/allocator_1.h"


class my_int_comparer
{
public:

    my_int_comparer() = default;

    int operator()(int first, int second){
        return first - second;
    }
};


int main(){
    auto* builder = new logger_builder();

    logger* logger1 = builder->add_stream("console", logger::severity::debug)
                            ->build();
    abstract_allocator* allocator = new allocator_1(logger1);
    associative_container<int, string>* bst = new binary_search_tree<int, string, my_int_comparer>(allocator, logger1);


    auto* my_pair = new associative_container<int, string>::key_value_pair{1, "Hello"};
    *bst += *my_pair;
    bst->insert(2, "Yo!");
    bool answer = (*bst)[my_pair];
    if(answer) cout << "Good" << endl;
    else cout << "Bad" << endl;
    bst->insert(4, "Nigga!");
    bst->insert(20, "LOL!");
    bst->bypass(associative_container<int, string>::bypass_mode::infix);

//    binary_search_tree<int, string, my_int_comparer>::inf_iterator my_iter = bst->begin_inf();      ////спросить
//    binary_search_tree<int, string, my_int_comparer>::inf_iterator my_iter_end = bst->end_inf();
//    for(my_iter; my_iter != my_iter_end; ++my_iter){
//
//    }

    bst->remove(4);
    bst->bypass(associative_container<int, string>::bypass_mode::infix);
    try{ string s = bst->get(9); cout << s << endl; }
        catch(const logic_error& ex){ cout << ex.what() << endl; }
    *bst -= 20;
    bst->bypass(associative_container<int, string>::bypass_mode::infix);

    //TODO: после трабла с мувом протестить дерево

    delete my_pair;
    delete bst;
    delete allocator;
    delete logger1;
    delete builder;
}