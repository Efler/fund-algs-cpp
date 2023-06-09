#ifndef BIG_INT_BIG_INT_CONCRETE_H
#define BIG_INT_BIG_INT_CONCRETE_H
#include <iostream>
#include <vector>
#include "abstract_big_int.h"
#include "../allocator_2/abstract_allocator.h"
#include "../logger/logger_builder.h"


class big_int_concrete final: public abstract_big_int
{

    ////------------------------------------ PRIVATE FIELDS ------------------------------------////

private:

    int _sign;
    vector<int>* _digits;
    abstract_allocator* _alloc;
    logger* _logger;

    ////-------------------------------- ADDITIONAL FUNCTIONS FIELD --------------------------------////

private:

    static bool fits_into_int(const string& str_num, int sign){
        string str_max_int = to_string(static_cast<unsigned int>(0) - 1);
        string str_min_int = str_max_int.substr(0, str_max_int.size() - 1) + (to_string(stoi(str_max_int.substr(str_max_int.size() - 1, 1)) + 1));

        return (str_num.size() < str_max_int.size() ||
          (str_num.size() == str_max_int.size() && ((!sign && str_num.compare(str_max_int) <= 0) ||
                                                    (sign && str_num.compare(str_min_int) <= 0))));
    }

    ////------------------------------------ CONSTRUCTORS FIELD ------------------------------------////

public:

    big_int_concrete(const string& string_number, abstract_allocator* al = nullptr, logger* l = nullptr):
            _alloc(al), _logger(l), _sign(0)
    {
        string str_num = string_number;
        if(str_num[0] == '-'){
            _sign = 1;
            str_num = str_num.substr(1);
        }

        for(const char& c : str_num){
            if(!isdigit(c)) throw logic_error("Error: string does not represent a big integer!");
        }

        if(fits_into_int(str_num, _sign)){
            if(_sign) _sign = stoi('-' + str_num);
            else _sign = stoi(str_num);
            _digits = nullptr;
        }else{

            if(_alloc != nullptr){
                _digits = reinterpret_cast<vector<int>*>(_alloc->allocate(sizeof(vector<int>)));
                new (_digits) vector<int>;
            }else{
                _digits = new vector<int>;
            }

            ////TODO: !!!
        }

    }

    big_int_concrete(const big_int_concrete& other){
        ////TODO: !!
    }

    big_int_concrete(big_int_concrete&& other){
        ////TODO: !!
    }

    big_int_concrete& operator = (const big_int_concrete& other){
        if(this != &other){
            ////TODO: !!
        }
        return *this;
    }

    big_int_concrete& operator = (big_int_concrete&& other) noexcept {
        if(this != &other){
            ////TODO: !!
        }
        return *this;
    }

    ////------------------------------------ DESTRUCTOR FIELD ------------------------------------////

    ~big_int_concrete() override {
        if(_digits != nullptr){
            if(_alloc != nullptr){
                _digits->~vector<int>();
                _alloc->deallocate(reinterpret_cast<void*>(_digits));
            }else{
                delete _digits;
            }
        }

        if(_logger != nullptr) _logger->log("DESTRUCTOR: big integer DELETED!", logger::severity::debug);
    }

};


#endif //BIG_INT_BIG_INT_CONCRETE_H
