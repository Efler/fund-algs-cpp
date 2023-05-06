#include <iostream>
#include <ctime>
#include "binary_search_tree.h"
#include "../allocator_3/allocator_3.h"


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
    abstract_allocator* allocator = new allocator_3(50000, allocator_3::mode::best);
    associative_container<int, string>* bst = new binary_search_tree<int, string, my_int_comparer>(allocator, logger1);
//    auto* bst = new binary_search_tree<int, string, my_int_comparer>(allocator, logger1);



//-----------------------------------------

//stress test

    srand(time(nullptr));
    for(int i = 0; i < 300; ++i){
        cout << "== iteration #" << i+1 << " ==" << endl;
        int action = rand() % 3;
        int bp_mode = rand() % 3;

        int random_key = rand() % 21;
        string value_str = "str";
        string result;
        switch(action){
            case 0:
                try{
                    logger1->log("inserting " + std::to_string(random_key), logger::severity::debug);
                    bst->insert(random_key,value_str);
                }catch(const logic_error& ex){
                    logger1->log("Insertion of " + std::to_string(random_key) + " failed, exception message: \"" + ex.what() + "\"", logger::severity::debug);
                }
                break;
            case 1:
                try{
                    logger1->log("removing " + std::to_string(random_key), logger::severity::debug);
                    bst->remove(random_key);
                }catch(const logic_error& ex){
                    logger1->log("Removing of " + std::to_string(random_key) + " failed, exception message: \"" + ex.what() + "\"", logger::severity::debug);
                }
                break;
            case 2:
                try{
                    logger1->log("searching " + std::to_string(random_key), logger::severity::debug);
                    result = bst->get(random_key);
                    cout << "Value by key " << random_key << " == " << result << endl;
                }catch(const logic_error& ex){
                    logger1->log("Searching of " + std::to_string(random_key) + " failed, exception message: \"" + ex.what() + "\"", logger::severity::debug);
                }
        }

        switch(bp_mode){
            case 0:
                logger1->log("prefix iterator", logger::severity::debug);
                bst->bypass(associative_container<int, string>::bypass_mode::prefix);
                break;
            case 1:
                logger1->log("infix iterator", logger::severity::debug);
                bst->bypass(associative_container<int, string>::bypass_mode::infix);
                break;
            case 2:
                logger1->log("postfix iterator", logger::severity::debug);
                bst->bypass(associative_container<int, string>::bypass_mode::postfix);
                break;
        }
    }

//-----------------------------------

////////constructors test
//
//    auto k = associative_container<int, std::string>::key_value_pair{0, std::move(std::string("1234"))};
//    *bst += k;
//    k = associative_container<int, std::string>::key_value_pair{-1, std::move(std::string("2345"))};
//    *bst += k;
//    k = associative_container<int, std::string>::key_value_pair{1, std::move(std::string("3456"))};
//    *bst += k;
//    k = associative_container<int, std::string>::key_value_pair{-2, std::move(std::string("4567"))};
//    *bst += k;
//    k = associative_container<int, std::string>::key_value_pair{2, std::move(std::string("5678"))};
//    *bst += k;
//    k = associative_container<int, std::string>::key_value_pair{-3, std::move(std::string("6789"))};
//    *bst += k;
//    k = associative_container<int, std::string>::key_value_pair{3, std::move(std::string("7890"))};
//    *bst += k;
//
//    bst->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    auto* bst2 = new binary_search_tree<int, string, my_int_comparer>(*bst);
//
//    bst->bypass(associative_container<int, string>::bypass_mode::infix);
//    bst2->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    auto* bst3 = new binary_search_tree<int, string, my_int_comparer>(std::move(*bst2));
//
//    bst->bypass(associative_container<int, string>::bypass_mode::infix);
//    bst2->bypass(associative_container<int, string>::bypass_mode::infix);
//    bst3->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    auto* bst4 = new binary_search_tree<int, string, my_int_comparer>(allocator, logger1);
//    (*bst4) = (*bst3);
//
//    bst->bypass(associative_container<int, string>::bypass_mode::infix);
//    bst2->bypass(associative_container<int, string>::bypass_mode::infix);
//    bst3->bypass(associative_container<int, string>::bypass_mode::infix);
//    bst4->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    auto* bst5 = new binary_search_tree<int, string, my_int_comparer>(allocator, logger1);
//    (*bst5) = std::move(*bst3);
//
//    bst->bypass(associative_container<int, string>::bypass_mode::infix);
//    bst2->bypass(associative_container<int, string>::bypass_mode::infix);
//    bst3->bypass(associative_container<int, string>::bypass_mode::infix);
//    bst4->bypass(associative_container<int, string>::bypass_mode::infix);
//    bst5->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    delete bst5;
//    delete bst4;
//    delete bst3;
//    delete bst2;

//---------------------------------------------

//////test #1
//
//    auto k = associative_container<int, std::string>::key_value_pair{0, std::move(std::string("1234"))};
//    *bst += k;
//    k = associative_container<int, std::string>::key_value_pair{-1, std::move(std::string("2345"))};
//    *bst += k;
//    k = associative_container<int, std::string>::key_value_pair{1, std::move(std::string("3456"))};
//    *bst += k;
//    k = associative_container<int, std::string>::key_value_pair{-2, std::move(std::string("4567"))};
//    *bst += k;
//    k = associative_container<int, std::string>::key_value_pair{2, std::move(std::string("5678"))};
//    *bst += k;
//    k = associative_container<int, std::string>::key_value_pair{-3, std::move(std::string("6789"))};
//    *bst += k;
//    k = associative_container<int, std::string>::key_value_pair{3, std::move(std::string("7890"))};
//    *bst += k;
//
//    associative_container<int, std::string>::key_value_pair find;
//    (*bst)[&find];
//
//    auto const &str = find._value;
//    *const_cast<std::string *>(&str) = "12345678";
//
//    std::cout << "Prefix iterator:" << std::endl;
//    bst->bypass(associative_container<int, std::string>::bypass_mode::prefix);
//
//    std::cout << std::endl << "Infix iterator:" << std::endl;
//    bst->bypass(associative_container<int, std::string>::bypass_mode::infix);
//
//    std::cout << std::endl << "Postfix iterator:" << std::endl;
//    bst->bypass(associative_container<int, std::string>::bypass_mode::postfix);
//
//    auto removed_value = *bst -= 0;
//    std::cout << "removed_value: " << removed_value << std::endl;
//
//    bst->bypass(associative_container<int, std::string>::bypass_mode::prefix);



//---------------------------------------

//////question ?

//    binary_search_tree<int, string, my_int_comparer>::inf_iterator my_iter = bst->begin_inf();
//    binary_search_tree<int, string, my_int_comparer>::inf_iterator my_iter_end = bst->end_inf();
//    for(my_iter; my_iter != my_iter_end; ++my_iter){
//
//    }
//---------------------------------------

///////test #2

//    auto* my_pair = new associative_container<int, string>::key_value_pair{1, "Hello"};
//    *bst += *my_pair;
//    bst->insert(2, "Yo!");
//    bool answer = (*bst)[my_pair];
//    if(answer) cout << "Good" << endl;
//    else cout << "Bad" << endl;
//    bst->insert(4, "Nigga!");
//    bst->insert(20, "LOL!");
//    bst->bypass(associative_container<int, string>::bypass_mode::infix);
//    string test = bst->remove(4);
//    cout << test << endl;
//    bst->bypass(associative_container<int, string>::bypass_mode::infix);
//    try{ string s = bst->get(9); cout << s << endl; }
//        catch(const logic_error& ex){ cout << ex.what() << endl; }
//    *bst -= 20;
//    bst->bypass(associative_container<int, string>::bypass_mode::infix);
//    delete my_pair;

//--------------------------------
//
//////iterators test
//
//    bst->insert(4, "str-4");
//    bst->insert(2, "str-2");
//    bst->insert(5, "str-5");
//    bst->insert(7, "str-7");
//    bst->insert(6, "str-6");
//    bst->insert(8, "str-8");
//    bst->insert(1, "str-1");
//    bst->insert(3, "str-3");
//
//    bst->bypass(associative_container<int, string>::bypass_mode::prefix);
//    bst->bypass(associative_container<int, string>::bypass_mode::infix);
//    bst->bypass(associative_container<int, string>::bypass_mode::postfix);
//    cout << bst->remove(4) << endl;
//    bst->bypass(associative_container<int, string>::bypass_mode::prefix);
//    bst->bypass(associative_container<int, string>::bypass_mode::infix);
//    bst->bypass(associative_container<int, string>::bypass_mode::postfix);
//--------------------------------


    delete bst;
    delete allocator;
    delete logger1;
    delete builder;
}