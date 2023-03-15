#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include "../logger/logger_builder.h"
using namespace std;

class memory
{

private:

    vector<void*> _pointers;
    const logger* _logger;

public:

    explicit memory(logger* l):
        _logger(l)
    {};

    memory(memory const &) = delete;

    memory &operator=(memory const &) = delete;

    virtual ~memory(){
        /////debug
//        for(void* i : _pointers){
//            cout << i << ' ';
//        }
        /////debug
        void* p;
        while(!_pointers.empty()){
            p = _pointers.back();
            //TODO----------------
            ::operator delete(p);
            _pointers.pop_back();
        }
    }

    string address_to_hex(void const * const pointer) noexcept{
        char address_buf[(sizeof(void const * const) << 1) + 3];
        sprintf(address_buf, "%#p", pointer);
        return std::string { address_buf };
    }

    void* allocate(size_t target_size){
        void* p;
        try{
            p = ::operator new(target_size);
        }catch(const bad_alloc &ba){
            p = nullptr;
            _logger->log("Allocation failed, pointer with nullptr value", logger::severity::debug);
        }
        _pointers.push_back(p);
        string s = "Allocation complete, address: ";
        s += address_to_hex(p);
        _logger->log(s, logger::severity::debug);
        return p;
    }

    void deallocate(void* target_to_dealloc){
        if(find(_pointers.begin(), _pointers.end(), target_to_dealloc) == _pointers.end()){
            _logger->log("Could not deallocate memory, unexpected pointer", logger::severity::debug);
            throw logic_error("Could not deallocate memory, unexpected pointer");
        }
        //TODO--------------------------
        _logger->log("Deallocation complete", logger::severity::debug);
        ::operator delete(target_to_dealloc);
        _pointers.erase(find(_pointers.begin(), _pointers.end(), target_to_dealloc));
    }

};