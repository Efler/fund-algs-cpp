#include <iostream>
#include <memory>
#include <list>
#include "allocator_2.h"
using namespace std;


void testing_allocator(){

    unique_ptr<logger_builder> builder(new logger_builder());
    builder->add_stream("console", logger::severity::trace);

    logger* logger = builder->build();


    abstract_allocator *allocator1 = new allocator_2(5000000, allocator_2::mode::best, logger);
    abstract_allocator *allocator2 = new allocator_2(2500000, allocator_2::mode::worst, nullptr);


    std::list<void*> allocated_blocks;

    srand((unsigned)time(nullptr));

    for (size_t i = 0; i < 1000; ++i)
    {
        void* ptr;

        switch (rand() % 2)
        {
            case 0:
                try
                {
                    ptr = reinterpret_cast<void*>(allocator1->allocate(rand() % 81 + 20)); // разность макс и мин с включенными границами + минимальное
                    allocated_blocks.push_back(ptr);
                }
                catch (std::exception const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
            case 1:

                if (allocated_blocks.empty())
                {
                    break;
                }

                try
                {
                    auto iter = allocated_blocks.begin();
                    std::advance(iter, rand() % allocated_blocks.size());
                    allocator1->deallocate(*iter);
                    allocated_blocks.erase(iter);
                }
                catch (std::exception const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
        }

        //std::cout << "iter # " << i + 1 << " finish" << std::endl;
    }

    while (!allocated_blocks.empty())
    {
        try
        {
            auto iter = allocated_blocks.begin();
            allocator1->deallocate(*iter);
            allocated_blocks.erase(iter);
        }
        catch (std::exception const &ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }

    delete allocator2;
    delete allocator1;
    delete logger;
}


int main(){

    testing_allocator();

}