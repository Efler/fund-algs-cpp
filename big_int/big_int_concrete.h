#ifndef BIG_INT_BIG_INT_CONCRETE_H
#define BIG_INT_BIG_INT_CONCRETE_H
#include <iostream>
#include <algorithm>
#include <vector>
#include "abstract_big_int.h"
#include "../allocator_2/abstract_allocator.h"
#include "../logger/logger_builder.h"


class big_int_concrete final//: public abstract_big_int ////TODO: TURN BACK!!
{

    ////------------------------------------ PRIVATE FIELDS ------------------------------------////

private:

    int _sign;
    vector<size_t>* _digits;
    abstract_allocator* _alloc;
    logger* _logger;

    ////-------------------------------- ADDITIONAL FUNCTIONS FIELD --------------------------------////

private:

    static bool fits_into_int(const string& str_num, int sign){
        string str_max_int = to_string((static_cast<int>(1) << ((sizeof(int) << 3) - 1)) - 1);
        string str_min_int = to_string(static_cast<int>(1) << ((sizeof(int) << 3) - 1)).substr(1);

        return (str_num.size() < str_max_int.size() ||
          (str_num.size() == str_max_int.size() && ((!sign && str_num.compare(str_max_int) <= 0) ||
                                                    (sign && str_num.compare(str_min_int) <= 0))));
    }

    static void reverse_str(string& s){
        string tmp;
        for(size_t i = s.size(); i > 0; --i) tmp += s[i-1];
        s = tmp;
    }

    static string str_base10_to_str_base2(string str_num){
        string tmp;
        string result;
        string divide_res;
        if(str_num == "0") return str_num;

        while(!str_num.empty()){
            tmp += str_num.substr(0, 1);
            if(str_num.size() == 1) str_num = "";
            else str_num = str_num.substr(1);

            if(!divide_res.empty() || (stoi(tmp) / 2) != 0) divide_res += to_string(stoi(tmp) / 2);
            tmp = to_string(stoi(tmp) % 2);
            if(tmp == "0") tmp = "";

            if(str_num.empty()){
                if(tmp.empty()) result += '0';
                else result += tmp;

                if(divide_res == "1"){
                    result += divide_res;
                    break;
                }else{
                    str_num = divide_res;
                    tmp = "";
                    divide_res = "";
                }
            }
        }

        reverse_str(result);
        return result;
    }

    static string str_base2_to_str_base10(string str_num){
        string result;
        string tmp = "2";
        if(str_num == "0") return str_num;

        reverse_str(str_num);
        for(size_t i = 0; i < str_num.size(); ++i){
            if(str_num[i] == '1'){
                exponentiate_str(tmp, i);
                result = sum_str(result, tmp);
                tmp = "2";
            }
        }

        return result;
    }

    static void delete_zeros(string& s){
        while(s[0] == '0'){
            s = s.substr(1);
        }
    }

    static void make_equal_length(string& s1, string& s2){
        if(s1.size() < s2.size()){
            size_t difference = s2.size() - s1.size();
            for(size_t i = 0; i < difference; ++i){
                s1 = "0" + s1;
            }
        }else if(s1.size() > s2.size()){
            size_t difference = s1.size() - s2.size();
            for(size_t i = 0; i < difference; ++i){
                s2 = "0" + s2;
            }
        }
    }

    static string sum_str(string s1, string s2){
        string result;
        int tmp = 0;

        make_equal_length(s1, s2);
        reverse_str(s1);
        reverse_str(s2);

        for(int i = 0; i < s1.size(); ++i){
            result = to_string(((s1[i] - '0') + (s2[i] - '0') + tmp) % 10) + result;
            tmp = ((s1[i] - '0') + (s2[i] - '0') + tmp) / 10;
        }
        if(tmp > 0) result = to_string(tmp) + result;

        return result;
    }

    static void multiply_str(string& s, size_t n){
        string copy_s = s;
        if(n == 0 || s == "0"){
            s = "0";
            return;
        }else if(n == 1){
            return;
        }

        for(int i = 0; i < n-1; ++i){
            s = sum_str(s, copy_s);
            cout << "lol " << s << endl;
        }
    }

    static void exponentiate_str(string& s, size_t n){
        if(n == 0){
            s = "1";
            return;
        }

        size_t tmp = stoul(s);
        for(int i = 0; i < n-1; ++i){
            multiply_str(s, tmp);
        }
    }

    void convert_to_base(string str_num){
        string tmp;
        string result;
        string divide_res;
        if(str_num == "0"){
            _digits->push_back(0);
            return;
        }

        while(!str_num.empty()){
            tmp = str_base10_to_str_base2(str_num);
            string tmp1 = tmp.substr(0, tmp.size() - (8 * sizeof(int)));
            if(tmp1.empty()){
                tmp1 = "0";
            }else {
                tmp1 = str_base2_to_str_base10(tmp1);
            }
            string tmp2 = tmp.substr(tmp.size() - (8 * sizeof(int)));
            tmp2 = str_base2_to_str_base10(tmp2);

            _digits->push_back(static_cast<size_t>(stoul(tmp2)));

            if(str_base10_to_str_base2(tmp1).size() < ((8 * sizeof(int)) + 1)){
                if(tmp1 != "0"){
                    _digits->push_back(static_cast<size_t>(stoul(tmp1)));
                }
                break;
            }else{
                str_num = tmp1;
            }
        }
    }

    string convert_to_string(){
        string result;

        if(_digits == nullptr){
            result = to_string(_sign);
        }else{
            string tmp = sum_str(to_string(static_cast<unsigned int>(0) - 1), "1");

            for(int i = 0; i < _digits->size(); ++i){
                if((*_digits)[i] != 0){
                    exponentiate_str(tmp, i);
                    multiply_str(tmp, (*_digits)[i]);
                    result = sum_str(result, tmp);

                    tmp = sum_str(to_string(static_cast<unsigned int>(0) - 1), "1");
                }
            }
            if(_sign) result = '-' + result;
        }

        return result;
    }

    ////------------------------------------ CONSTRUCTORS FIELD ------------------------------------////

public:

    big_int_concrete(const string& string_number, abstract_allocator* allocator = nullptr, logger* logger = nullptr):
            _alloc(allocator), _logger(logger), _sign(0)
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
                _digits = reinterpret_cast<vector<size_t>*>(_alloc->allocate(sizeof(vector<size_t>)));
                new (_digits) vector<size_t>;
            }else{
                _digits = new vector<size_t>;
            }

            convert_to_base(str_num);
        }

        if(_logger != nullptr) _logger->log("CONSTRUCTOR: big int CREATED!", logger::severity::debug);
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

    ~big_int_concrete() /*override*/ { ////TODO: TURN BACK!!
        cout << "destructor check: "; cout << convert_to_string() << endl;

        if(_digits != nullptr){
            if(_alloc != nullptr){
                _digits->~vector<size_t>();
                _alloc->deallocate(reinterpret_cast<void*>(_digits));
            }else{
                delete _digits;
            }
        }

        if(_logger != nullptr) _logger->log("DESTRUCTOR: big integer DELETED!", logger::severity::debug);
    }

};


#endif //BIG_INT_BIG_INT_CONCRETE_H
