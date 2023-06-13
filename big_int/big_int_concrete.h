#ifndef BIG_INT_BIG_INT_CONCRETE_H
#define BIG_INT_BIG_INT_CONCRETE_H
#include <iostream>
#include <algorithm>
#include <vector>
#include "abstract_big_int.h"
#include "../allocator_2/abstract_allocator.h"
#include "../logger/logger_builder.h"


class big_int_concrete final: public abstract_big_int
{

    ////------------------------------------ PRIVATE FIELDS ------------------------------------////

private:

    int _sign;
    vector<size_t>* _digits;
    abstract_allocator* _alloc;
    logger* _logger;

private:

    ////-------------------------------- STRING_NUMBER FUNCTIONS FIELD --------------------------------////

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
                power_str(tmp, i);
                result = sum_str(result, tmp);
                tmp = "2";
            }
        }

        return result;
    }

    static void make_equal_length(string& s1, string& s2){
        if(s1.size() < s2.size()){
            size_t difference = s2.size() - s1.size();
            for(size_t i = 0; i < difference; ++i){
                s1.insert(0, "0");
            }
        }else if(s1.size() > s2.size()){
            size_t difference = s1.size() - s2.size();
            for(size_t i = 0; i < difference; ++i){
                s2.insert(0, "0");
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
            result.insert(0, to_string(((s1[i] - '0') + (s2[i] - '0') + tmp) % 10));
            tmp = ((s1[i] - '0') + (s2[i] - '0') + tmp) / 10;
        }
        if(tmp > 0) result = to_string(tmp) + result;

        return result;
    }

    static void multiply_str(string& s, const string& n){
        string copy_s = s;
        reverse_str(copy_s);
        string n_str = n;
        reverse_str(n_str);
        string res;
        char tmp;
        string tmp1;
        string tmp2;
        size_t tmp3;
        if(n == "0" || s == "0"){
            s = "0";
            return;
        }else if(n == "1"){
            return;
        }

        for(size_t i = 0; i < copy_s.size(); ++i){
            tmp = copy_s[i];
            for(size_t k = 0; k < n_str.size(); ++k){
                tmp3 = static_cast<size_t>(n_str[k] - '0') * static_cast<size_t>(tmp - '0');
                if(k != 0) for(int t = 0; t < k; ++t) tmp3 *= 10;
                tmp2 = to_string(tmp3);
                tmp1 = sum_str(tmp1, tmp2);
            }
            for(size_t l = 0; l < i; ++l) tmp1 += "0";
            res = sum_str(res, tmp1);
            tmp1 = "";
        }
        s = res;
    }

    static void power_str(string& s, size_t n){
        if(n == 0){
            s = "1";
            return;
        }else if(n == 1){
            return;
        }

        string tmp = s;
        for(int i = 0; i < n-1; ++i){
            multiply_str(s, tmp);
        }
    }

    static int str_num_comparer(const string& num1, const string& num2){
        size_t num1_size = num1.size();
        size_t num2_size = num2.size();
        if(num1_size > num2_size) return 1;
        else if(num1_size < num2_size) return -1;
        else{
            return num1.compare(num2);
        }
    }

    string sum_in_strings(const string& num1, const string& num2) const {
        string result;

        size_t str_case = 0;
        string copy_num1 = num1;
        string copy_num2 = num2;

        if (num2[0] == '-'){
            copy_num2 = num2.substr(1);
            return subtract_in_strings(copy_num1, copy_num2);
        }else if (num1[0] == '-'){
            str_case = 1;
            copy_num1 = num1.substr(1);
        }

        size_t max_length = (size_t)max(copy_num1.size(), copy_num2.size());
        int borrow = 0;

        if(str_case == 0) {
            for (int i = 0; i < max_length || borrow != 0; i++) {
                int digit1 = (i < copy_num1.length()) ? (copy_num1[copy_num1.length() - i - 1] - '0') : 0;
                int digit2 = (i < copy_num2.length()) ? (copy_num2[copy_num2.length() - i - 1] - '0') : 0;

                int diff = digit1 + digit2 + borrow;

                if (diff > 9) {
                    borrow = diff / 10;
                    diff %= 10;
                }else{
                    borrow = 0;
                }

                result.push_back(static_cast<char>(diff + '0'));
            }

            while (result.length() > 1 && result.back() == '0') {
                result.pop_back();
            }

            reverse(result.begin(), result.end());

            return result;
        }else{
            if (str_num_comparer(copy_num1, copy_num2) > 0){
                result = subtract_in_strings(copy_num1, copy_num2);
                result.insert(0, "-");
                return result;
            }else if (str_num_comparer(copy_num1, copy_num2) == 0) {
                result += '0';
                return result;
            }else{
                string tmp = copy_num1;
                copy_num1 = copy_num2;
                copy_num2 = tmp;
                return subtract_in_strings(copy_num1, copy_num2);
            }
        }
    }

    string subtract_in_strings(const string& num1, const string& num2) const {
        string result;

        size_t str_case = 0;
        string copy_num1 = num1;
        string copy_num2 = num2;

        if (num2[0] == '-'){
            copy_num2 = num2.substr(1);
            return sum_in_strings(copy_num1, copy_num2);
        }else if (num1[0] == '-'){
            str_case = 1;
            copy_num1 = num1.substr(1);
        }

        size_t max_length = (size_t)max(copy_num1.size(), copy_num2.size());
        int borrow = 0;

        if(str_case == 0){
            int sign = 0;
            if(str_num_comparer(copy_num1, copy_num2) < 0){
                sign++;
                string tmp = copy_num1;
                copy_num1 = copy_num2;
                copy_num2 = tmp;
            }else if(str_num_comparer(copy_num1, copy_num2) == 0){
                result += '0';
                return result;
            }

            for (int i = 0; i < max_length; i++){
                int digit1 = (i < copy_num1.length()) ? (copy_num1[copy_num1.length() - i - 1] - '0') : 0;
                int digit2 = (i < copy_num2.length()) ? (copy_num2[copy_num2.length() - i - 1] - '0') : 0;

                int diff = digit1 - digit2 - borrow;

                if (diff < 0){
                    diff += 10;
                    borrow = 1;
                }else{
                    borrow = 0;
                }

                result.push_back(static_cast<char>(diff + '0'));
            }

            while (result.length() > 1 && result.back() == '0') {
                result.pop_back();
            }
            if(sign && result != "0") result.push_back('-');

            reverse(result.begin(), result.end());

        }else{
            result = sum_in_strings(copy_num1, copy_num2);
            result.insert(0, "-");
        }

        return result;
    }

    ////-------------------------------- ADDITIONAL FUNCTIONS FIELD --------------------------------////

    bool fits_into_int(const string& str_num, int sign){
        string str_max_int = to_string((static_cast<int>(1) << ((sizeof(int) << 3) - 1)) - 1);
        string str_min_int = to_string(static_cast<int>(1) << ((sizeof(int) << 3) - 1)).substr(1);

        return (str_num.size() < str_max_int.size() ||
          (str_num.size() == str_max_int.size() && ((!sign && str_num.compare(str_max_int) <= 0) ||
                                                    (sign && str_num.compare(str_min_int) <= 0))));
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

    string convert_to_string() const {
        string result;

        if(_digits == nullptr){
            result = to_string(_sign);
        }else{
            string tmp = sum_str(to_string(static_cast<unsigned int>(0) - 1), "1");

            for(int i = 0; i < _digits->size(); ++i){
                if((*_digits)[i] != 0){
                    power_str(tmp, i);
                    multiply_str(tmp, to_string((*_digits)[i]));
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

    big_int_concrete(const string& string_number = "0", logger* logger = nullptr, abstract_allocator* allocator = nullptr):
            _logger(logger), _alloc(allocator), _sign(0)
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
        _logger = other._logger;
        _alloc = other._alloc;
        _sign = other._sign;
        if(other._digits == nullptr){
            _digits = nullptr;
        }else{
            if(_alloc != nullptr){
                _digits = reinterpret_cast<vector<size_t>*>(_alloc->allocate(sizeof(vector<size_t>)));
                new (_digits) vector<size_t>;
            }else{
                _digits = new vector<size_t>;
            }
            copy(other._digits->begin(), other._digits->end(), back_inserter(*_digits));
        }

        if(_logger != nullptr) _logger->log("CONSTRUCTOR: big int COPIED!", logger::severity::debug);
    }

    big_int_concrete(const char string_number[]):
            big_int_concrete(string_number, nullptr, nullptr){}

    big_int_concrete(big_int_concrete&& other) noexcept {
        _logger = other._logger;
        other._logger = nullptr;
        _alloc = other._alloc;
        other._alloc = nullptr;
        _sign = other._sign;
        other._sign = 0;
        _digits = other._digits;
        other._digits = nullptr;

        if(_logger != nullptr) _logger->log("CONSTRUCTOR: big int MOVED!", logger::severity::debug);
    }

    big_int_concrete& operator = (const big_int_concrete& other){
        if(this != &other){
            _sign = other._sign;
            if(other._digits == nullptr){
                if(_digits != nullptr){
                    if(_alloc != nullptr){
                        _digits->~vector<size_t>();
                        _alloc->deallocate(reinterpret_cast<void*>(_digits));
                    }else{
                        delete _digits;
                    }
                    _digits = nullptr;
                }
            }else{
                if(_digits != nullptr){
                    _digits->clear();
                }else{
                    if(_alloc != nullptr){
                        _digits = reinterpret_cast<vector<size_t>*>(_alloc->allocate(sizeof(vector<size_t>)));
                        new (_digits) vector<size_t>;
                    }else{
                        _digits = new vector<size_t>;
                    }
                }

                copy(other._digits->begin(), other._digits->end(), back_inserter(*_digits));
            }
        }

        if(_logger != nullptr) _logger->log("OPERATOR = : big int ASSIGNED!", logger::severity::debug);
        return *this;
    }

    big_int_concrete& operator = (big_int_concrete&& other) noexcept {
        if(this != &other){
            if(_digits != nullptr){
                if(_alloc != nullptr){
                    _digits->~vector<size_t>();
                    _alloc->deallocate(reinterpret_cast<void*>(_digits));
                }else{
                    delete _digits;
                }
                _digits = nullptr;
            }

            _alloc = other._alloc;
            other._alloc = nullptr;
            _sign = other._sign;
            other._sign = 0;
            _digits = other._digits;
            other._digits = nullptr;
        }

        if(_logger != nullptr) _logger->log("OPERATOR = : big int MOVED!", logger::severity::debug);
        return *this;
    }

    ////------------------------------------ DESTRUCTOR FIELD ------------------------------------////

    ~big_int_concrete() override {
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

    ////------------------------------------ METHOD'S FIELD ------------------------------------////

    friend istream& operator >> (istream& in, big_int_concrete* bigint);

    friend ostream& operator << (ostream& out, const big_int_concrete* bigint);

    friend istream& operator >> (istream& in, big_int_concrete& bigint);

    friend ostream& operator << (ostream& out, const big_int_concrete& bigint);

    abstract_big_int* add(const abstract_big_int* summand) override {
        auto* smnd = dynamic_cast<const big_int_concrete*>(summand);

        string num1 = this->convert_to_string();
        string num2 = smnd->convert_to_string();

        string result = sum_in_strings(num1, num2);
        _sign = 0;

        if(result[0] == '-'){
            _sign = 1;
            result = result.substr(1);
        }

        if(fits_into_int(result, _sign)){
            if(_sign) _sign = stoi('-' + result);
            else _sign = stoi(result);
            if(_digits != nullptr){
                if(_alloc != nullptr){
                    _digits->~vector<size_t>();
                    _alloc->deallocate(reinterpret_cast<void*>(_digits));
                }else{
                    delete _digits;
                }
                _digits = nullptr;
            }
        }else{
            if(_alloc != nullptr){
                _digits = reinterpret_cast<vector<size_t>*>(_alloc->allocate(sizeof(vector<size_t>)));
                new (_digits) vector<size_t>;
            }else{
                _digits = new vector<size_t>;
            }

            convert_to_base(result);
        }

        if(_logger != nullptr) _logger->log("ADD: Done!", logger::severity::debug);
        return this;
    }

    big_int_concrete* operator += (const big_int_concrete* summand){
        return dynamic_cast<big_int_concrete*>(add(summand));
    }

    big_int_concrete& operator += (const big_int_concrete& summand){
        return *(dynamic_cast<big_int_concrete*>(add(&summand)));
    }

    abstract_big_int* sum(const abstract_big_int* summand) const override {
        auto* smnd = dynamic_cast<const big_int_concrete*>(summand);

        string num1 = this->convert_to_string();
        string num2 = smnd->convert_to_string();

        string result = sum_in_strings(num1, num2);

        auto* res_bigint = new big_int_concrete(result);

        if(_logger != nullptr) _logger->log("SUM: Done!", logger::severity::debug);
        return res_bigint;
    }

    big_int_concrete* operator + (const big_int_concrete* summand) const {
        return dynamic_cast<big_int_concrete*>(sum(summand));
    }

    big_int_concrete operator + (const big_int_concrete& summand) const {
        string num1 = this->convert_to_string();
        string num2 = summand.convert_to_string();

        string result = sum_in_strings(num1, num2);

        big_int_concrete res_bigint = result;

        if(_logger != nullptr) _logger->log("SUM: Done!", logger::severity::debug);
        return res_bigint;
    }

    abstract_big_int* subtract(const abstract_big_int* subtrahend) override {
        auto* subtr = dynamic_cast<const big_int_concrete*>(subtrahend);

        string num1 = this->convert_to_string();
        string num2 = subtr->convert_to_string();

        string result = subtract_in_strings(num1, num2);
        _sign = 0;

        if(result[0] == '-'){
            _sign = 1;
            result = result.substr(1);
        }

        if(fits_into_int(result, _sign)){
            if(_sign) _sign = stoi('-' + result);
            else _sign = stoi(result);
            if(_digits != nullptr){
                if(_alloc != nullptr){
                    _digits->~vector<size_t>();
                    _alloc->deallocate(reinterpret_cast<void*>(_digits));
                }else{
                    delete _digits;
                }
                _digits = nullptr;
            }
        }else{
            if(_alloc != nullptr){
                _digits = reinterpret_cast<vector<size_t>*>(_alloc->allocate(sizeof(vector<size_t>)));
                new (_digits) vector<size_t>;
            }else{
                _digits = new vector<size_t>;
            }

            convert_to_base(result);
        }

        if(_logger != nullptr) _logger->log("SUBTRACT: Done!", logger::severity::debug);
        return this;
    }

    big_int_concrete* operator -= (const big_int_concrete* subtrahend){
        return dynamic_cast<big_int_concrete*>(subtract(subtrahend));
    }

    big_int_concrete& operator -= (const big_int_concrete& subtrahend){
        return *(dynamic_cast<big_int_concrete*>(subtract(&subtrahend)));
    }

    abstract_big_int* subtraction(const abstract_big_int* subtrahend) const override {
        auto* subtr = dynamic_cast<const big_int_concrete*>(subtrahend);

        string num1 = this->convert_to_string();
        string num2 = subtr->convert_to_string();

        string result = subtract_in_strings(num1, num2);

        auto* res_bigint = new big_int_concrete(result);

        if(_logger != nullptr) _logger->log("SUBTRACTION: Done!", logger::severity::debug);
        return res_bigint;
    }

    big_int_concrete* operator - (const big_int_concrete* subtrahend) const {
        return dynamic_cast<big_int_concrete*>(subtraction(subtrahend));
    }

    big_int_concrete operator - (const big_int_concrete& subtrahend) const {
        string num1 = this->convert_to_string();
        string num2 = subtrahend.convert_to_string();

        string result = subtract_in_strings(num1, num2);

        big_int_concrete res_bigint = result;

        if(_logger != nullptr) _logger->log("SUBTRACTION: Done!", logger::severity::debug);
        return res_bigint;
    }

    bool lower_than(const abstract_big_int* other) const override {
        auto* o = dynamic_cast<const big_int_concrete*>(other);
        if(_digits == nullptr){
            if(o->_digits == nullptr){
                return _sign < o->_sign;
            }else{
                return !(o->_sign);
            }
        }else{
            if(o->_digits == nullptr){
                return _sign;
            }else{
                if(_sign && !(o->_sign)) return true;
                else if(!_sign && o->_sign) return false;
                else if(_sign && o->_sign){
                    if(_digits->size() != o->_digits->size()){
                        return _digits->size() > o->_digits->size();
                    }else{
                        vector<size_t> v1 = *_digits;
                        reverse(v1.begin(), v1.end());
                        vector<size_t> v2 = *(o->_digits);
                        reverse(v2.begin(), v2.end());
                        return (v1 > v2);
                    }
                }
                else if(!_sign && !(o->_sign)){
                    if(_digits->size() != o->_digits->size()){
                        return _digits->size() < o->_digits->size();
                    }else{
                        vector<size_t> v1 = *_digits;
                        reverse(v1.begin(), v1.end());
                        vector<size_t> v2 = *(o->_digits);
                        reverse(v2.begin(), v2.end());
                        return (v1 < v2);
                    }
                }
            }
        }
        return true;
    }

    bool operator < (const big_int_concrete* other) const {
        return lower_than(other);
    }

    bool operator < (const big_int_concrete& other) const {
        return lower_than(&other);
    }

    bool greater_than(const abstract_big_int* other) const override {
        auto* o = dynamic_cast<const big_int_concrete*>(other);
        if(_digits == nullptr){
            if(o->_digits == nullptr){
                return _sign > o->_sign;
            }else{
                return (o->_sign);
            }
        }else{
            if(o->_digits == nullptr){
                return !_sign;
            }else{
                if(_sign && !(o->_sign)) return false;
                else if(!_sign && o->_sign) return true;
                else if(_sign && o->_sign){
                    if(_digits->size() != o->_digits->size()){
                        return _digits->size() < o->_digits->size();
                    }else{
                        vector<size_t> v1 = *_digits;
                        reverse(v1.begin(), v1.end());
                        vector<size_t> v2 = *(o->_digits);
                        reverse(v2.begin(), v2.end());
                        return (v1 < v2);
                    }
                }
                else if(!_sign && !(o->_sign)){
                    if(_digits->size() != o->_digits->size()){
                        return _digits->size() > o->_digits->size();
                    }else{
                        vector<size_t> v1 = *_digits;
                        reverse(v1.begin(), v1.end());
                        vector<size_t> v2 = *(o->_digits);
                        reverse(v2.begin(), v2.end());
                        return (v1 > v2);
                    }
                }
            }
        }
        return true;
    }

    bool operator > (const big_int_concrete* other) const {
        return greater_than(other);
    }

    bool operator > (const big_int_concrete& other) const {
        return greater_than(&other);
    }

    bool lower_than_or_equal_to(const abstract_big_int* other) const override {
        return (equals(other) || lower_than(other));
    }

    bool operator <= (const big_int_concrete* other) const {
        return lower_than_or_equal_to(other);
    }

    bool operator <= (const big_int_concrete& other) const {
        return lower_than_or_equal_to(&other);
    }

    bool greater_than_or_equal_to(const abstract_big_int* other) const override {
        return (equals(other) || greater_than(other));
    }

    bool operator >= (const big_int_concrete* other) const {
        return greater_than_or_equal_to(other);
    }

    bool operator >= (const big_int_concrete& other) const {
        return greater_than_or_equal_to(&other);
    }

    bool equals(const abstract_big_int* other) const override {
        auto* o = dynamic_cast<const big_int_concrete*>(other);
        if(_digits == nullptr){
            if(o->_digits == nullptr){
                return _sign == o->_sign;
            }else return false;
        }else{
            if(o->_digits == nullptr){
                return false;
            }else{
                return (_sign == o->_sign && *_digits == (*o->_digits));
            }
        }
    }

    bool operator == (const big_int_concrete* other) const {
        return equals(other);
    }

    bool operator == (const big_int_concrete& other) const {
        return equals(&other);
    }

    bool not_equals(const abstract_big_int* other) const override {
        return !equals(other);
    }

    bool operator != (const big_int_concrete* other) const {
        return not_equals(other);
    }

    bool operator != (const big_int_concrete& other) const {
        return not_equals(&other);
    }

};


ostream& operator << (ostream& out, const big_int_concrete* bigint){
    out << bigint->convert_to_string();
    return out;
}

istream& operator >> (istream& in, big_int_concrete* bigint){
    string s;
    in >> s;
    if(s[0] == '-'){
        bigint->_sign = 1;
        s = s.substr(1);
    }else bigint->_sign = 0;

    for(const char& c : s){
        if(!isdigit(c)) throw logic_error("Error: string does not represent a big integer!");
    }

    if(bigint->fits_into_int(s, bigint->_sign)){
        if(bigint->_sign) bigint->_sign = stoi('-' + s);
        else bigint->_sign = stoi(s);
        if(bigint->_digits != nullptr){
            if(bigint->_alloc != nullptr){
                (bigint->_digits)->~vector<size_t>();
                (bigint->_alloc)->deallocate(reinterpret_cast<void*>(bigint->_digits));
            }else{
                delete bigint->_digits;
            }
            bigint->_digits = nullptr;
        }
    }else{
        if(bigint->_digits != nullptr){
            bigint->_digits->clear();
        }else{
            if(bigint->_alloc != nullptr){
                bigint->_digits = reinterpret_cast<vector<size_t>*>((bigint->_alloc)->allocate(sizeof(vector<size_t>)));
                new (bigint->_digits) vector<size_t>;
            }else{
                bigint->_digits = new vector<size_t>;
            }
        }

        bigint->convert_to_base(s);
    }

    if(bigint->_logger != nullptr) bigint->_logger->log("OPERATOR >> : big int CHANGED!", logger::severity::debug);
    return in;
}

ostream& operator << (ostream& out, const big_int_concrete& bigint){
    return operator<<(out, &bigint);
}

istream& operator >> (istream& in, big_int_concrete& bigint){
    return operator>>(in, &bigint);
}

ostream& operator << (ostream& out, const abstract_big_int* bigint){
    return operator<<(out, dynamic_cast<const big_int_concrete*>(bigint));
}

istream& operator >> (istream& in, abstract_big_int* bigint){
    return operator>>(in, dynamic_cast<big_int_concrete*>(bigint));
}


#endif //BIG_INT_BIG_INT_CONCRETE_H