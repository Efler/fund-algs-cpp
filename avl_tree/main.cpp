#include <iostream>
#include <ctime>
#include "avl_tree.h"
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
    associative_container<int, string>* avl = new avl_tree<int, string, my_int_comparer>(allocator, logger1);
//    auto* avl = new avl_tree<int, string, my_int_comparer>(allocator, logger1);

//-------------------------------------
//stress test

    srand(time(nullptr));
    for(int i = 0; i < 1500; ++i){
        cout << "== iteration #" << i+1 << " ==" << endl;
        int action = rand() % 3;
        int bp_mode = rand() % 3;

        int random_key = rand() % 56;
        string value_str = "str";
        string result;
        switch(action){
            case 0:
                try{
                    logger1->log("inserting " + std::to_string(random_key), logger::severity::debug);
                    avl->insert(random_key, value_str);
                }catch(const logic_error& ex){
                    logger1->log("Insertion of " + std::to_string(random_key) + " failed, exception message: \"" + ex.what() + "\"", logger::severity::debug);
                }
                break;
            case 1:
                try{
                    logger1->log("removing " + std::to_string(random_key), logger::severity::debug);
                    avl->remove(random_key);
                }catch(const logic_error& ex){
                    logger1->log("Removing of " + std::to_string(random_key) + " failed, exception message: \"" + ex.what() + "\"", logger::severity::debug);
                }
                break;
            case 2:
                try{
                    logger1->log("searching " + std::to_string(random_key), logger::severity::debug);
                    result = avl->get(random_key);
                    cout << "Value by key " << random_key << " == " << result << endl;
                }catch(const logic_error& ex){
                    logger1->log("Searching of " + std::to_string(random_key) + " failed, exception message: \"" + ex.what() + "\"", logger::severity::debug);
                }
        }

        switch(bp_mode){
            case 0:
                logger1->log("prefix iterator", logger::severity::debug);
                avl->bypass(associative_container<int, string>::bypass_mode::prefix);
                break;
            case 1:
                logger1->log("infix iterator", logger::severity::debug);
                avl->bypass(associative_container<int, string>::bypass_mode::infix);
                break;
            case 2:
                logger1->log("postfix iterator", logger::severity::debug);
                avl->bypass(associative_container<int, string>::bypass_mode::postfix);
                break;
        }
    }

//---------------------------------
//constructors test


//    auto k = associative_container<int, std::string>::key_value_pair{0, std::move(std::string("1234"))};
//    *avl += k;
//    k = associative_container<int, std::string>::key_value_pair{-1, std::move(std::string("2345"))};
//    *avl += k;
//    k = associative_container<int, std::string>::key_value_pair{1, std::move(std::string("3456"))};
//    *avl += k;
//    k = associative_container<int, std::string>::key_value_pair{-2, std::move(std::string("4567"))};
//    *avl += k;
//    k = associative_container<int, std::string>::key_value_pair{2, std::move(std::string("5678"))};
//    *avl += k;
//    k = associative_container<int, std::string>::key_value_pair{-3, std::move(std::string("6789"))};
//    *avl += k;
//    k = associative_container<int, std::string>::key_value_pair{3, std::move(std::string("7890"))};
//    *avl += k;
//
//    avl->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    auto* avl2 = new avl_tree<int, string, my_int_comparer>(*avl);
//
//    avl->bypass(associative_container<int, string>::bypass_mode::infix);
//    avl2->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    auto* avl3 = new avl_tree<int, string, my_int_comparer>(std::move(*avl2));
//
//    avl->bypass(associative_container<int, string>::bypass_mode::infix);
//    avl2->bypass(associative_container<int, string>::bypass_mode::infix);
//    avl3->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    auto* avl4 = new avl_tree<int, string, my_int_comparer>(allocator, logger1);
//    (*avl4) = (*avl3);
//
//    avl->bypass(associative_container<int, string>::bypass_mode::infix);
//    avl2->bypass(associative_container<int, string>::bypass_mode::infix);
//    avl3->bypass(associative_container<int, string>::bypass_mode::infix);
//    avl4->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    auto* avl5 = new avl_tree<int, string, my_int_comparer>(allocator, logger1);
//    (*avl5) = std::move(*avl3);
//
//    avl->bypass(associative_container<int, string>::bypass_mode::infix);
//    avl2->bypass(associative_container<int, string>::bypass_mode::infix);
//    avl3->bypass(associative_container<int, string>::bypass_mode::infix);
//    avl4->bypass(associative_container<int, string>::bypass_mode::infix);
//    avl5->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    delete avl5;
//    delete avl4;
//    delete avl3;
//    delete avl2;

    delete avl;
    delete allocator;
    delete logger1;
    delete builder;
}