#include <iostream>
#include <memory>
#include "allocator_2.h"
using namespace std;


int main(){
    unique_ptr<logger_builder> builder(new logger_builder());
    builder->add_stream("console", logger::severity::debug);

    shared_ptr<logger> logger(builder->build());
    unique_ptr<allocator_2> alloc(new allocator_2(1000, allocator_2::mode::first, logger));

    try{
        void* p = alloc->allocate(10);
        void* p1 = alloc->allocate(10);
        void* p2 = alloc->allocate(10);
    }catch(const logic_error& ex){
        cout << ex.what() << endl;
    }

}