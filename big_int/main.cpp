#include <iostream>
#include "big_int_concrete.h"
#include "../allocator_3/allocator_3.h"
#include <random>


void factorial_test_classic_multiplication(size_t iterations){
    big_int_concrete x = "0";
    big_int_concrete y = "1";
    for(int i = 1; i < iterations + 1; ++i){
        cout << "iteration #" << i << " is working..." << endl;
        big_int_concrete z = to_string(i);
        y *= z;
        x += y;
    }
    cout << "Result --> " << x << endl;
}


void factorial_test_karatsuba(size_t iterations){
    big_int_concrete x = "0";
    big_int_concrete y = "1";
    for(int i = 1; i < iterations + 1; ++i){
        cout << "iteration #" << i << " is working..." << endl;
        big_int_concrete z = to_string(i);
        y.multiply_karatsuba(z);
        x += y;
    }
    cout << "Result --> " << x << endl;
}


string random_string_number(size_t min_str_size, size_t max_str_size, bool only_positive = false, int seed = 0){
    string str_res;

    random_device rd;
    mt19937 generator(rd());
    if(seed != 0) generator.seed(seed);
    uniform_int_distribution<size_t> distribution_seed(0, 2000000000);
    size_t seed_len = distribution_seed(generator);
    size_t seed_digits = distribution_seed(generator);

    random_device rd_len;
    mt19937 generator_len(rd());
    generator_len.seed(seed_len);

    random_device rd_digits;
    mt19937 generator_digits(rd());
    generator_digits.seed(seed_digits);

    uniform_int_distribution<size_t> distribution(min_str_size, max_str_size);
    uniform_int_distribution<size_t> distribution_digits(0, 9);
    size_t length = distribution(generator_len);
    if(length == 0) return "0";

    for(size_t i = 0; i < length; ++i){
        str_res += static_cast<char>(distribution_digits(generator_digits) + '0');
    }

    while (str_res.length() > 1 && str_res[0] == '0') {
        str_res = str_res.substr(1);
    }

    if(!only_positive){
        uniform_int_distribution<size_t> distribution_sign(0, 1);
        if(distribution_sign(generator_digits) && str_res != "0") str_res.insert(0, "-");
    }

    return str_res;
}


void bigint_test(size_t iterations = 100, int seed = 0){
    cout << "\n##### BIGINT TEST STARTED #####\n" << endl;

    random_device rd;
    mt19937 generator(rd());
    if(seed != 0) generator.seed(seed);
    uniform_int_distribution<size_t> distribution_action(0, 11);

    uniform_int_distribution<int> distribution_seed(0, 2000000000);

    auto* builder = new logger_builder();
    auto* logger = builder->add_stream("console", logger::severity::debug)->build();
//    auto* my_allocator = new allocator_3(500000, allocator_3::mode::best);

    for(size_t i = 0; i < iterations; ++i){
        cout << "-------------------------------- iteration #" << i+1 << " --------------------------------" << endl;

        size_t action = distribution_action(generator);
        int num1_seed = distribution_seed(generator);
        int num2_seed = distribution_seed(generator);

        big_int_concrete num1 = random_string_number(0, 20, false, num1_seed);
        big_int_concrete num2 = random_string_number(0, 20, false, num2_seed);
        big_int_concrete num3;
        string bool_res;
        switch (action){
            case 0 :
                cout << ">> action '+'" << endl;
                num3 = num1 + num2;
                cout << num1 << " + " << num2 << " = " << num3 << endl;
                break;
            case 1 :
                cout << ">> action '+='" << endl;
                cout << num1 << " += " << num2 << endl;
                num1 += num2;
                cout << "result = " << num1 << endl;
                break;
            case 2 :
                cout << ">> action '-'" << endl;
                num3 = num1 - num2;
                cout << num1 << " - " << num2 << " = " << num3 << endl;
                break;
            case 3 :
                cout << ">> action '-='" << endl;
                cout << num1 << " -= " << num2 << endl;
                num1 -= num2;
                cout << "result = " << num1 << endl;
                break;
            case 4 :
                cout << ">> action '=='" << endl;
                bool_res = num1 == num2 ? "true" : "false";
                cout << num1 << " == " << num2 << " ===>> " << bool_res << endl;
                break;
            case 5 :
                cout << ">> action '!='" << endl;
                bool_res = num1 != num2 ? "true" : "false";
                cout << num1 << " != " << num2 << " ===>> " << bool_res << endl;
                break;
            case 6 :
                cout << ">> action '<'" << endl;
                bool_res = num1 < num2 ? "true" : "false";
                cout << num1 << " < " << num2 << " ===>> " << bool_res << endl;
                break;
            case 7 :
                cout << ">> action '<='" << endl;
                bool_res = num1 <= num2 ? "true" : "false";
                cout << num1 << " <= " << num2 << " ===>> " << bool_res << endl;
                break;
            case 8 :
                cout << ">> action '>'" << endl;
                bool_res = num1 > num2 ? "true" : "false";
                cout << num1 << " > " << num2 << " ===>> " << bool_res << endl;
                break;
            case 9 :
                cout << ">> action '>='" << endl;
                bool_res = num1 >= num2 ? "true" : "false";
                cout << num1 << " >= " << num2 << " ===>> " << bool_res << endl;
                break;
            case 10:
                cout << ">> action '*'" << endl;
                num3 = num1 * num2;
                cout << num1 << " * " << num2 << " = " << num3 << endl;
                break;
            case 11:
                cout << ">> action '*' (karatsuba)" << endl;
                cout << num1;
                num1.multiply_karatsuba(num2);
                cout << " * " << num2 << " = " << num1 << endl;
                break;
            default:
                break;
        }
    }

//    delete my_allocator;
    delete logger;
    delete builder;

    cout << "\n##### BIGINT TEST FINISHED #####" << endl;
}



int main(){

// ------- random test -------------------------------------------------------

//    bigint_test(150, 69696969);

// ------- factorial test ----------------------------------------------------

    factorial_test_classic_multiplication(30);
//    factorial_test_karatsuba(30);

// ------- simple tests ------------------------------------------------------

//    auto* builder = new logger_builder();
//    auto* logger = builder->add_stream("console", logger::severity::debug)->build();
//    auto* allocator = new allocator_3(500000, allocator_3::mode::best);
//
//    auto* bigint = new big_int_concrete("-555", logger, allocator);
//
//    cout << "### entering bi..." << endl;
//    cin >> bigint;
//
//    cout << "### creating bi2..." << endl;
//    auto* bigint2 = new big_int_concrete(*bigint);
//
//    cout << "bigint: " << bigint << " ; bigint2: " << bigint2 << endl;
//
//    cout << "### entering bi2..." << endl;
//    cin >> bigint2;
//
//    cout << "bigint: " << bigint << " ; bigint2: " << bigint2 << endl;
//
//    cout << "### subtraction bi-bi2..." << endl;
//    *bigint -= bigint2;
//
//    cout << "bigint: " << bigint << " ; bigint2: " << bigint2 << endl;
//
//    cout << "### creating bi3 with bi2+bi2..." << endl;
//    auto* bigint3 = *bigint2 + bigint2;
//
//    cout << "bigint: " << bigint << "; bigint2: " << bigint2 << "; bigint3: " << bigint3 << endl;
//
//    cout << "### creating bi4..." << endl;
//    big_int_concrete bigint4;
//    cin >> bigint4;
//
//    cout << "bigint4: " << bigint4 << endl;
//
//    cout << "### creating bi5..." << endl;
//    big_int_concrete bigint5 = bigint4;
//
//    cout << "bigint4: " << bigint4 << " ; bigint5: " << bigint5 << endl;
//
//    cout << "### creating bi6 with bi4+bi5..." << endl;
//    big_int_concrete bigint6 = bigint4 + bigint5;
//
//    cout << "bigint4: " << bigint4 << "; bigint5: " << bigint5 << "; bigint6: " << bigint6 << endl;
//
//    cout << "### bi6 += bi4 += bi6..." << endl;
//    bigint6 += bigint4 += bigint6;
//
//    cout << "bigint4: " << bigint4 << "; bigint5: " << bigint5 << "; bigint6: " << bigint6 << endl;
//
//    cout << "### bi6 = bi5 - bi4..." << endl;
//    bigint6 = bigint5 - bigint4;
//
//    cout << "bigint4: " << bigint4 << "; bigint5: " << bigint5 << "; bigint6: " << bigint6 << endl;
//
//    delete bigint3;
//    delete bigint2;
//    delete bigint;
//
//    delete allocator;
//    delete logger;
//    delete builder;

}