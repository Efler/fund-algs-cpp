#ifndef BIG_INT_BIG_INT_CONCRETE_H
#define BIG_INT_BIG_INT_CONCRETE_H
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
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
        while(str_num[0] == '0' && str_num.size() > 1){
            str_num = str_num.substr(1);
        }
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
        while(str_num[0] == '0' && str_num.size() > 1){
            str_num = str_num.substr(1);
        }
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

    bool is_negative() const {
        if(_digits == nullptr){
            return _sign < 0;
        }else{
            return _sign;
        }
    }

    static int deque_comparer(const deque<size_t>& a, const deque<size_t>& b){
        if(a.size() < b.size()) return -1;
        else if(a.size() > b.size()) return 1;
        else{
            auto a1 = a;
            auto b1 = b;

            reverse(a1.begin(), a1.end());
            reverse(b1.begin(), b1.end());
            if(a1 > b1) return 1;
            else if(a1 < b1) return -1;
            else return 0;
        }
    }

    static void normalize_deque(string str_num, deque<size_t>& deque){
        string tmp;
        string result;
        string divide_res;
        if(str_num == "0"){
            deque.push_back(0);
            return;
        }

        while(!str_num.empty()){
            tmp = str_base10_to_str_base2(str_num);
            string tmp1;
            if(tmp.size() < (8 * sizeof(int))) tmp1 = "0";
            else{
                tmp1 = tmp.substr(0, tmp.size() - (8 * sizeof(int)));
                if(tmp1.empty()){
                    tmp1 = "0";
                }else {
                    tmp1 = str_base2_to_str_base10(tmp1);
                }
            }
            string tmp2;
            if(tmp.size() < (8 * sizeof(int))){
                tmp2 = tmp;
                tmp2 = str_base2_to_str_base10(tmp2);
            }else{
                tmp2 = tmp.substr(tmp.size() - (8 * sizeof(int)));
                tmp2 = str_base2_to_str_base10(tmp2);
            }
            deque.push_back(static_cast<size_t>(stoul(tmp2)));

            if(str_base10_to_str_base2(tmp1).size() < ((8 * sizeof(int)) + 1)){
                if(tmp1 != "0"){
                    deque.push_back(static_cast<size_t>(stoul(tmp1)));
                }
                break;
            }else{
                str_num = tmp1;
            }
        }
    }

    static void normalize(string& res, string& check) {
        string tmp = str_base10_to_str_base2(res);
        string tmp1;
        if(tmp.size() < (8 * sizeof(int))) tmp1 = "0";
        else{
            tmp1 = tmp.substr(0, tmp.size() - (8 * sizeof(int)));
            if(tmp1.empty()){
                tmp1 = "0";
            }else {
                tmp1 = str_base2_to_str_base10(tmp1);
            }
        }
        string tmp2;
        if(tmp.size() < (8 * sizeof(int))){
            tmp2 = tmp;
            tmp2 = str_base2_to_str_base10(tmp2);
        }else{
            tmp2 = tmp.substr(tmp.size() - (8 * sizeof(int)));
            tmp2 = str_base2_to_str_base10(tmp2);
        }
        res = tmp2;
        check = tmp1;
    }

    deque<size_t> sum_in_deque(const deque<size_t>& a, const deque<size_t>& b) const {
        deque<size_t> result;
        string borrow = "0";
        size_t max_len = std::max(a.size(), b.size());
        for(size_t i = 0; i < max_len; ++i){
            string a_digit = i < a.size() ? to_string(a[i]) : "0";
            string b_digit = i < b.size() ? to_string(b[i]) : "0";

            string res = sum_in_strings(sum_in_strings(a_digit, b_digit), borrow);
            normalize(res, borrow);

            result.push_back(static_cast<size_t>(stoul(res)));
        }
        if(borrow != "0"){
            result.push_back(static_cast<size_t>(stoul(borrow)));
        }
        return result;
    }

    deque<size_t> subtract_in_deque(const deque<size_t>& a, const deque<size_t>& b) const {
        deque<size_t> result;
        if(deque_comparer(a, b) > 0){
            string borrow = "0";
            for(int i = 0; i < a.size(); ++i){
                string a_digit = i < a.size() ? to_string(a[i]) : "0";
                string b_digit = i < b.size() ? to_string(b[i]) : "0";

                string res = subtract_in_strings(subtract_in_strings(a_digit, b_digit), borrow);
                if(res[0] == '-'){
                    borrow = "1";
                    res = res.substr(1);
                }else{
                    borrow = "0";
                }

                result.push_back(static_cast<size_t>(stoul(res)));

            }

            return result;
        }else if(deque_comparer(a, b) < 0){
            string borrow = "0";
            for(int i = 0; i < b.size(); ++i){
                string b_digit = i < b.size() ? to_string(b[i]) : "0";
                string a_digit = i < a.size() ? to_string(a[i]) : "0";

                string res = subtract_in_strings(subtract_in_strings(b_digit, a_digit), borrow);
                if(res[0] == '-'){
                    borrow = "1";
                    res = res.substr(1);
                }else{
                    borrow = "0";
                }

                result.push_back(static_cast<size_t>(stoul(res)));
            }

            return result;
        }else{
            result.push_back(0);
            return result;
        }
    }

    deque<size_t> karatsuba(deque<size_t>& f1, deque<size_t>& s1) const {
        size_t n = std::max(f1.size(), s1.size());
        if(f1.size() < s1.size()){
            while(f1.size() != s1.size()) f1.push_back(0);
        }else if(f1.size() > s1.size()){
            while(s1.size() != f1.size()) s1.push_back(0);
        }

        if(n != 1){
            size_t m = n/2;

            deque<size_t> f2;
            for(size_t i = m; i < n; ++i){
                f2.push_back(f1[i]);
            }
            for(size_t j = 0; j < m; ++j){
                f1.pop_back();
            }

            deque<size_t> s2;
            for(size_t i = m; i < n; ++i){
                s2.push_back(s1[i]);
            }
            for(size_t j = 0; j < m; ++j){
                s1.pop_back();
            }

            int flag = 0;
            if(deque_comparer(f2, f1) < 0) flag++;
            deque<size_t> z1 = std::move(subtract_in_deque(f2, f1));
            if(deque_comparer(s2, s1) < 0) flag++;
            deque<size_t> z2 = std::move(subtract_in_deque(s2, s1));
            deque<size_t> x = std::move(karatsuba(f2, s2));
            deque<size_t> y = std::move(karatsuba(f1, s1));
            int flag1 = flag % 2;
            deque<size_t> z = std::move(karatsuba(z1, z2));

            deque<size_t> tmp = sum_in_deque(x, y);
            int flag2 = 0;

            if(flag1){
                z = sum_in_deque(tmp, z);
            }else{
                if(deque_comparer(tmp, z) < 0) flag2++;
                z = subtract_in_deque(tmp, z);
            }

            for(int i = 0; i < n; ++i) x.push_front(0);
            for(int i = 0; i < m; ++i) z.push_front(0);

            z1 = sum_in_deque(x, y);
            if(flag2){
                z = subtract_in_deque(z1, z);
            }else{
                z = sum_in_deque(z1, z);
            }

            return z;
        }else{
            string a = to_string(f1[0]);
            string b = to_string(s1[0]);
            string borrow;
            multiply_str(a, b);
            deque<size_t> z;
            normalize_deque(a, z);

            return z;
        }
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

    abstract_big_int* multiply(const abstract_big_int* right_multiplier) override {
        auto* right_mult = dynamic_cast<const big_int_concrete*>(right_multiplier);

        string bi1 = this->convert_to_string();
        string bi2 = right_mult->convert_to_string();
        size_t negative_result = 0;

        if( (!(this->is_negative()) && (right_mult->is_negative())) || ((this->is_negative()) && !(right_mult->is_negative())) ){
            negative_result++;
            if(this->is_negative()) bi1 = bi1.substr(1);
            else bi2 = bi2.substr(1);
        }else if(this->is_negative() && right_mult->is_negative()){
            bi1 = bi1.substr(1);
            bi2 = bi2.substr(1);
        }

        multiply_str(bi1, bi2);
        if(negative_result && bi1 != "0") bi1.insert(0, "-");

        _sign = 0;

        if(bi1[0] == '-'){
            _sign = 1;
            bi1 = bi1.substr(1);
        }

        if(fits_into_int(bi1, _sign)){
            if(_sign) _sign = stoi('-' + bi1);
            else _sign = stoi(bi1);
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

            convert_to_base(bi1);
        }

        if(_logger != nullptr) _logger->log("MULTIPLY: Done!", logger::severity::debug);
        return this;
    }

    big_int_concrete* operator *= (const big_int_concrete* right_multiplier){
        return dynamic_cast<big_int_concrete*>(multiply(right_multiplier));
    }

    big_int_concrete& operator *= (const big_int_concrete& right_multiplier){
        return *(dynamic_cast<big_int_concrete*>(multiply(&right_multiplier)));
    }

    abstract_big_int* multiplication(const abstract_big_int* right_multiplier) const override {
        auto* right_mult = dynamic_cast<const big_int_concrete*>(right_multiplier);
        string bi1 = this->convert_to_string();
        string bi2 = right_mult->convert_to_string();
        size_t negative_result = 0;

        if( (!(this->is_negative()) && (right_mult->is_negative())) || ((this->is_negative()) && !(right_mult->is_negative())) ){
            negative_result++;
            if(this->is_negative()) bi1 = bi1.substr(1);
            else bi2 = bi2.substr(1);
        }else if(this->is_negative() && right_mult->is_negative()){
            bi1 = bi1.substr(1);
            bi2 = bi2.substr(1);
        }

        multiply_str(bi1, bi2);

        if(negative_result && bi1 != "0") bi1.insert(0, "-");

        auto* res_bigint = new big_int_concrete(bi1);

        if(_logger != nullptr) _logger->log("MULTIPLICATION: Done!", logger::severity::debug);

        return res_bigint;
    }

    big_int_concrete* operator * (const big_int_concrete* right_multiplier) const {
        return dynamic_cast<big_int_concrete*>(multiplication(right_multiplier));
    }

    big_int_concrete operator * (const big_int_concrete& right_multiplier) const {
        string bi1 = this->convert_to_string();
        string bi2 = right_multiplier.convert_to_string();
        size_t negative_result = 0;

        if( (!(this->is_negative()) && (right_multiplier.is_negative())) || ((this->is_negative()) && !(right_multiplier.is_negative())) ){
            negative_result++;
            if(this->is_negative()) bi1 = bi1.substr(1);
            else bi2 = bi2.substr(1);
        }else if(this->is_negative() && right_multiplier.is_negative()){
            bi1 = bi1.substr(1);
            bi2 = bi2.substr(1);
        }

        multiply_str(bi1, bi2);
        if(negative_result && bi1 != "0") bi1.insert(0, "-");

        big_int_concrete res_bigint = bi1;

        if(_logger != nullptr) _logger->log("MULTIPLICATION: Done!", logger::severity::debug);

        return res_bigint;
    }

    big_int_concrete* multiply_karatsuba(const big_int_concrete* right_multiplier){
        deque<size_t> v1;
        deque<size_t> v2;
        size_t negative_result = 0;

        if( (!(is_negative()) && (right_multiplier->is_negative())) || ((is_negative()) && !(right_multiplier->is_negative())) ){
            negative_result++;
        }

        if(_digits == nullptr){
            size_t digit;
            if(_sign < 0) digit = -_sign;
            else digit = _sign;
            v1.push_back(digit);
        }else{
            copy(_digits->begin(), _digits->end(), back_inserter(v1));
        }

        if(right_multiplier->_digits == nullptr){
            size_t digit;
            if(right_multiplier->_sign < 0) digit = -(right_multiplier->_sign);
            else digit = right_multiplier->_sign;
            v2.push_back(digit);
        }else{
            copy(right_multiplier->_digits->begin(), right_multiplier->_digits->end(), back_inserter(v2));
        }

        if(v1.size() < v2.size()){
            while(v1.size() != v2.size()) v1.push_back(0);
        }else if(v1.size() > v2.size()){
            while(v2.size() != v1.size()) v2.push_back(0);
        }

        if(v1.size() % 2){
            v1.push_back(0);
            v2.push_back(0);
        }

        deque<size_t> result = karatsuba(v1, v2);

        if(result.size() == 1){
            string res_str = to_string(result[0]);
            if(fits_into_int(res_str, (int)negative_result)){
                if(negative_result) _sign = static_cast<int>(-result[0]);
                else _sign = static_cast<int>(result[0]);
                if(_digits != nullptr){
                    if(_alloc != nullptr){
                        _digits->~vector<size_t>();
                        _alloc->deallocate(reinterpret_cast<void*>(_digits));
                    }else{
                        delete _digits;
                    }
                    _digits = nullptr;
                }
                return this;
            }
        }
        if(negative_result) _sign = 1;
        else _sign = 0;
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

        copy(result.begin(), result.end(), back_inserter(*_digits));
        return this;
    }

    big_int_concrete& multiply_karatsuba(const big_int_concrete& right_multiplier){
        deque<size_t> v1;
        deque<size_t> v2;
        size_t negative_result = 0;

        if( (!(is_negative()) && (right_multiplier.is_negative())) || ((is_negative()) && !(right_multiplier.is_negative())) ){
            negative_result++;
        }

        if(_digits == nullptr){
            size_t digit;
            if(_sign < 0) digit = -_sign;
            else digit = _sign;
            v1.push_back(digit);
        }else{
            copy(_digits->begin(), _digits->end(), back_inserter(v1));
        }

        if(right_multiplier._digits == nullptr){
            size_t digit;
            if(right_multiplier._sign < 0) digit = -(right_multiplier._sign);
            else digit = right_multiplier._sign;
            v2.push_back(digit);
        }else{
            copy(right_multiplier._digits->begin(), right_multiplier._digits->end(), back_inserter(v2));
        }

        if(v1.size() < v2.size()){
            while(v1.size() != v2.size()) v1.push_back(0);
        }else if(v1.size() > v2.size()){
            while(v2.size() != v1.size()) v2.push_back(0);
        }

        if(v1.size() % 2){
            v1.push_back(0);
            v2.push_back(0);
        }

        deque<size_t> result = karatsuba(v1, v2);
        while(result[result.size() - 1] == 0 && result.size() != 1) result.pop_back();

        if(result.size() == 1){
            string res_str = to_string(result[0]);
            if(fits_into_int(res_str, (int)negative_result)){
                if(negative_result) _sign = static_cast<int>(-result[0]);
                else _sign = static_cast<int>(result[0]);
                if(_digits != nullptr){
                    if(_alloc != nullptr){
                        _digits->~vector<size_t>();
                        _alloc->deallocate(reinterpret_cast<void*>(_digits));
                    }else{
                        delete _digits;
                    }
                    _digits = nullptr;
                }
                return *this;
            }
        }
        if(negative_result) _sign = 1;
        else _sign = 0;
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

        copy(result.begin(), result.end(), back_inserter(*_digits));
        return *this;
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