#include <iostream>
#include <ctime>
#include "red_black_tree.h"
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
    associative_container<int, string>* rbt = new red_black_tree<int, string, my_int_comparer>(allocator, logger1);
//    auto* rbt = new red_black_tree<int, string, my_int_comparer>(allocator, logger1);

//---------------------------------
//constructors test
//
//
//    auto k = associative_container<int, std::string>::key_value_pair{0, std::move(std::string("1234"))};
//    *rbt += k;
//    k = associative_container<int, std::string>::key_value_pair{-1, std::move(std::string("2345"))};
//    *rbt += k;
//    k = associative_container<int, std::string>::key_value_pair{1, std::move(std::string("3456"))};
//    *rbt += k;
//    k = associative_container<int, std::string>::key_value_pair{-2, std::move(std::string("4567"))};
//    *rbt += k;
//    k = associative_container<int, std::string>::key_value_pair{2, std::move(std::string("5678"))};
//    *rbt += k;
//    k = associative_container<int, std::string>::key_value_pair{-3, std::move(std::string("6789"))};
//    *rbt += k;
//    k = associative_container<int, std::string>::key_value_pair{3, std::move(std::string("7890"))};
//    *rbt += k;
//
//    rbt->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    auto* rbt2 = new red_black_tree<int, string, my_int_comparer>(*rbt);
//
//    rbt->bypass(associative_container<int, string>::bypass_mode::infix);
//    rbt2->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    auto* rbt3 = new red_black_tree<int, string, my_int_comparer>(std::move(*rbt2));
//
//    rbt->bypass(associative_container<int, string>::bypass_mode::infix);
//    rbt2->bypass(associative_container<int, string>::bypass_mode::infix);
//    rbt3->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    auto* rbt4 = new red_black_tree<int, string, my_int_comparer>(allocator, logger1);
//    (*rbt4) = (*rbt3);
//
//    rbt->bypass(associative_container<int, string>::bypass_mode::infix);
//    rbt2->bypass(associative_container<int, string>::bypass_mode::infix);
//    rbt3->bypass(associative_container<int, string>::bypass_mode::infix);
//    rbt4->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    auto* rbt5 = new red_black_tree<int, string, my_int_comparer>(allocator, logger1);
//    (*rbt5) = std::move(*rbt3);
//
//    rbt->bypass(associative_container<int, string>::bypass_mode::infix);
//    rbt2->bypass(associative_container<int, string>::bypass_mode::infix);
//    rbt3->bypass(associative_container<int, string>::bypass_mode::infix);
//    rbt4->bypass(associative_container<int, string>::bypass_mode::infix);
//    rbt5->bypass(associative_container<int, string>::bypass_mode::infix);
//    cout << "---" <<endl;
//
//    delete rbt5;
//    delete rbt4;
//    delete rbt3;
//    delete rbt2;

//-------------------------------------
//stress test

    srand(time(nullptr));
    for(int i = 0; i < 1000; ++i){
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
                    rbt->insert(random_key,value_str);
                }catch(const logic_error& ex){
                    logger1->log("Insertion of " + std::to_string(random_key) + " failed, exception message: \"" + ex.what() + "\"", logger::severity::debug);
                }
                break;
            case 1:
                try{
                    logger1->log("removing " + std::to_string(random_key), logger::severity::debug);
                    rbt->remove(random_key);
                }catch(const logic_error& ex){
                    logger1->log("Removing of " + std::to_string(random_key) + " failed, exception message: \"" + ex.what() + "\"", logger::severity::debug);
                }
                break;
            case 2:
                try{
                    logger1->log("searching " + std::to_string(random_key), logger::severity::debug);
                    result = rbt->get(random_key);
                    cout << "Value by key " << random_key << " == " << result << endl;
                }catch(const logic_error& ex){
                    logger1->log("Searching of " + std::to_string(random_key) + " failed, exception message: \"" + ex.what() + "\"", logger::severity::debug);
                }
        }

        switch(bp_mode){
            case 0:
                logger1->log("prefix iterator", logger::severity::debug);
                rbt->bypass(associative_container<int, string>::bypass_mode::prefix);
                break;
            case 1:
                logger1->log("infix iterator", logger::severity::debug);
                rbt->bypass(associative_container<int, string>::bypass_mode::infix);
                break;
            case 2:
                logger1->log("postfix iterator", logger::severity::debug);
                rbt->bypass(associative_container<int, string>::bypass_mode::postfix);
                break;
        }
    }


    delete rbt;
    delete allocator;
    delete logger1;
    delete builder;
}