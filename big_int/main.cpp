#include <iostream>
#include "big_int_concrete.h"


int main(){

    auto* builder = new logger_builder();
    auto* logger = builder->add_stream("console", logger::severity::debug)->build();

    auto* bigint = new big_int_concrete("-555", logger);

    cout << bigint << endl;

    cin >> bigint;

    cout << bigint << endl;

    auto* bigint2 = new big_int_concrete(*bigint);

    cout << bigint2 << endl;

    auto* bigint3 = new big_int_concrete(std::move(*bigint2));

    cout << bigint2 << " " << bigint3 << endl;


    delete bigint3;
    delete bigint2;
    delete bigint;
    delete logger;
    delete builder;

}