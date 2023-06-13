#ifndef BIG_INT_ABSTRACT_BIG_INT_H
#define BIG_INT_ABSTRACT_BIG_INT_H
#include <iostream>


class abstract_big_int
{
public:

    virtual abstract_big_int* add(const abstract_big_int* summand) = 0;

    virtual abstract_big_int* operator += (const abstract_big_int* summand){
        return add(summand);
    }

    virtual abstract_big_int* sum(const abstract_big_int* summand) const = 0;

    virtual abstract_big_int* operator + (const abstract_big_int* summand) const {
        return sum(summand);
    }

    virtual abstract_big_int* subtract(const abstract_big_int* subtrahend) = 0;

    virtual abstract_big_int* operator -= (const abstract_big_int* subtrahend){
        return subtract(subtrahend);
    }

    virtual abstract_big_int* subtraction(const abstract_big_int* subtrahend) const = 0;

    virtual abstract_big_int* operator - (const abstract_big_int* subtrahend) const {
        return subtraction(subtrahend);
    }

    virtual bool lower_than(const abstract_big_int* other) const = 0;

    virtual bool operator < (const abstract_big_int* other) const {
        return lower_than(other);
    }

    virtual bool greater_than(const abstract_big_int* other) const = 0;

    virtual bool operator > (const abstract_big_int* other) const {
        return greater_than(other);
    }

    virtual bool lower_than_or_equal_to(const abstract_big_int* other) const = 0;

    virtual bool operator <= (const abstract_big_int* other) const {
        return lower_than_or_equal_to(other);
    }

    virtual bool greater_than_or_equal_to(const abstract_big_int* other) const = 0;

    virtual bool operator >= (const abstract_big_int* other) const {
        return greater_than_or_equal_to(other);
    }

    virtual bool equals(const abstract_big_int* other) const = 0;

    virtual bool operator == (const abstract_big_int* other) const {
        return equals(other);
    }

    virtual bool not_equals(const abstract_big_int* other) const = 0;

    virtual bool operator != (const abstract_big_int* other) const {
        return not_equals(other);
    }

    friend std::istream& operator >> (std::istream& in, abstract_big_int* bigint);

    friend std::ostream& operator << (std::ostream& out, const abstract_big_int* bigint);

    virtual ~abstract_big_int() = default;

};


#endif //BIG_INT_ABSTRACT_BIG_INT_H
