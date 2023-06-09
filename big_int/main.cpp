#include <iostream>
#include "big_int_concrete.h"


bool fits_into_int(const string& str_num, int sign){
    string str_max_int = to_string(static_cast<unsigned int>(0) - 1);
    string str_min_int = str_max_int.substr(0, str_max_int.size() - 1) + (to_string(stoi(str_max_int.substr(str_max_int.size() - 1, 1)) + 1));

    cout << str_max_int << endl << str_min_int << endl;

    return (str_num.size() < str_max_int.size() ||
            (str_num.size() == str_max_int.size() && ((!sign && str_num.compare(str_max_int) <= 0) ||
                                                      (sign && str_num.compare(str_min_int) <= 0))));
}


int main(){
    cout << "Oh, hi there!" << endl;
    unsigned int a = static_cast<unsigned int>(0) - 1;
    cout << a << endl;
//    string s1 = "2333";
//    cout << s1.compare("2344") << endl;

    cout << fits_into_int("4294967296", 1) << endl;
}