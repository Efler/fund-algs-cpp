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