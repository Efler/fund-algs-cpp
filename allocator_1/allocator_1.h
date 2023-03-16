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

    map<void*, size_t> _pointers;
    const logger* _logger;

public:

    explicit memory(logger* l):
        _logger(l)
    {};

    memory(memory const &) = delete;

    memory &operator=(memory const &) = delete;

    string memory_dump(void* const p, const size_t t){
        auto* p2 = reinterpret_cast<unsigned char*>(p);
        string s = "";
        size_t dump;
        for(int k = 0; k < t; ++k){
            dump = (size_t)(*p2);
            s += to_string(dump);
            s += ' ';
            p2++;
        }
        return s;
    }

    string address_to_hex(void const * const pointer) noexcept{
        char address_buf[(sizeof(void const * const) << 1) + 3];
        sprintf(address_buf, "%#p", pointer);
        return std::string { address_buf };
    }

    virtual ~memory(){
        /////debug
//        for(void* i : _pointers){
//            cout << i << ' ';
//        }
        /////debug
        for(pair<void*, size_t> pointer : _pointers){
            string msg = "Emergency deallocation of memory, address: ";
            msg += address_to_hex(pointer.first);
            msg += ", dump: ";
            msg += memory_dump(pointer.first, pointer.second);
            _logger->log(msg, logger::severity::debug);
            ::operator delete(pointer.first);
            _pointers.erase(pointer.first);
        }
    }

    void* allocate(size_t target_size){
        void* p;
        try{
            p = ::operator new(target_size);
        }catch(const bad_alloc &ba){
            p = nullptr;
            _logger->log("Allocation failed, pointer with nullptr value", logger::severity::debug);
        }
        _pointers[p] = target_size;
        string msg = "Allocation complete, address: ";
        msg += address_to_hex(p);
        _logger->log(msg, logger::severity::debug);
        return p;
    }

    void deallocate(void* target_to_dealloc){
        auto iter = _pointers.find(target_to_dealloc);
        if(iter == _pointers.end()){
            _logger->log("Could not deallocate memory, unexpected pointer", logger::severity::debug);
            throw logic_error("Could not deallocate memory, unexpected pointer");
        }
        string msg = "Deallocation complete, address: ";
        msg += address_to_hex(target_to_dealloc);
        msg += ", dump: ";
        msg += memory_dump(target_to_dealloc, _pointers[target_to_dealloc]);
        _logger->log(msg, logger::severity::debug);
        ::operator delete(target_to_dealloc);
        _pointers.erase(iter->first);
    }

};