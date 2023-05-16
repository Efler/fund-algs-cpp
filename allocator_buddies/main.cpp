#include <iostream>
#include <list>
#include "allocator_buddies.h"


void testing_allocator(){

    unique_ptr<logger_builder> builder(new logger_builder());
    builder->add_stream("console", logger::severity::trace);

    logger* logger = builder->build();


    abstract_allocator *allocator1 = new allocator_buddies(20000, logger);


    std::list<void*> allocated_blocks;

    srand((unsigned)time(nullptr));

    void* ptr;
    for (size_t i = 0; i < 1500; ++i)
    {
    cout << "----- iteration #" << i+1 << " -----" << endl;
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

        cout << "iteration #" << i + 1 << " finished" << endl;
    }

    int l = 1;
    while (!allocated_blocks.empty())
    {
        cout << "----- clearing #" << l++ << " -----" << endl;
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

    delete allocator1;
    delete logger;
}



int main(){

    testing_allocator();

}