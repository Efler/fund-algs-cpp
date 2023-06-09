#include <iostream>
#include "big_int_concrete.h"


int main(){
    auto* builder = new logger_builder();
    auto* logger = builder->add_stream("console", logger::severity::debug)->build();

    auto* bigint = new big_int_concrete("4000000000", nullptr, logger);

    delete bigint;
    delete logger;
    delete builder;
}