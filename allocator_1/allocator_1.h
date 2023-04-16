#include <iostream>
#include "../logger/logger_builder.h"
#include "../allocator_2/abstract_allocator.h"
using namespace std;


class allocator_1 final : public abstract_allocator
{

private:

    const logger* _logger;

public:

    explicit allocator_1(const logger* l = nullptr):
        _logger(l){
        if(_logger != nullptr) _logger->log("ALLOCATOR CREATED", logger::severity::debug);
    };

    allocator_1(const allocator_1& alloc) = delete;

    allocator_1(allocator_1&& alloc) = delete;

    allocator_1& operator=(const allocator_1& alloc) = delete;

    allocator_1& operator=(allocator_1&& alloc) = delete;

    ~allocator_1() override {
        if(_logger != nullptr) _logger->log("ALLOCATOR DELETED", logger::severity::debug);
    }

private:

    template <typename T>
    string to_str(const T i) const {
        stringstream stream;
        stream << i;
        return stream.str();
    }

    string memory_dump(void* const p, const size_t t) const {
        auto* p2 = reinterpret_cast<unsigned char*>(p);
        string s;
        size_t dump;
        for(int k = 0; k < t; ++k){
            dump = (size_t)(*p2);
            s += to_string(dump);
            s += ' ';
            p2++;
        }
        return s;
    }

    string address_to_hex(void const * const pointer) const noexcept {
        char address_buf[(sizeof(void const * const) << 1) + 3];
        sprintf(address_buf, "%#p", pointer);
        return std::string { address_buf };
    }

public:

    void* allocate(size_t target_size) const override {
        void* p;
        try{
            p = ::operator new(target_size + sizeof(size_t));
        }catch(const bad_alloc &ba){
            throw logic_error("Bad Allocation!");
        }
        *reinterpret_cast<size_t*>(p) = target_size;
        p = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p) + 1);

        if(_logger != nullptr){
            string msg = "Allocation complete, address: ";
            msg += address_to_hex(p);
            msg += ", size: ";
            msg += to_str(target_size);
            _logger->log(msg, logger::severity::debug);
        }

        return p;
    }

    void deallocate(void* target_to_dealloc) const override {
        size_t dump_size = *(reinterpret_cast<size_t*>(target_to_dealloc) - 1);
        ::operator delete(reinterpret_cast<void*>(reinterpret_cast<size_t*>(target_to_dealloc) - 1));

        if(_logger != nullptr){
            string msg = "Deallocation complete, address: ";
            msg += address_to_hex(target_to_dealloc);
            msg += ", size: ";
            msg += to_str(dump_size);
            msg += ", dump: ";
            msg += memory_dump(target_to_dealloc, dump_size);
            _logger->log(msg, logger::severity::debug);
        }
    }

};