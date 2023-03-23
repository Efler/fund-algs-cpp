#include <iostream>
#include <memory>
#include "allocator_2.h"
using namespace std;


int main(){
    unique_ptr<logger_builder> builder(new logger_builder());
    builder->add_stream("console", logger::severity::debug);

    shared_ptr<logger> logger(builder->build());

    try{

        unique_ptr<allocator_2> alloc(new allocator_2(300, allocator_2::mode::best, logger));

        void* p = alloc->allocate(10);
        void* p1 = alloc->allocate(15);
        void* p2 = alloc->allocate(10);

        int* i1 = reinterpret_cast<int*>(p2);
        *i1 = 1000;

        alloc->deallocate(p1);
        alloc->deallocate(p2);
        void* p3 = alloc->allocate(14);


    }catch(const logic_error& ex){
        cout << ex.what() << endl;
    }

}