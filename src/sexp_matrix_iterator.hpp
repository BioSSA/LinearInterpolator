#pragma once

#include <iterator>

#include <R.h>
#include <Rinternals.h>

extern "C"
{

class sexp_scaled_point_iterator
   : public std::iterator<std::input_iterator_tag, double>
{

private:

    double * vector;
    size_t dimention;

public:

    sexp_scaled_point_iterator(const SEXP & val, size_t d)
        : vector(REAL(val))
    { }

    sexp_scaled_point_iterator(const SEXP & val, size_t d, size_t begin_index)
        : vector(REAL(val) + begin_index)
        , dimention(d)
    { }

    sexp_scaled_point_iterator (const sexp_scaled_point_iterator& x) = default;

    double operator*() const
    {
        return *vector;
    }

    double * operator->() const
    {
        return vector;
    }

    sexp_scaled_point_iterator& operator++()
    {
        vector += dimention;
        return *this;
    }

    sexp_scaled_point_iterator operator++(int)
    {
        sexp_scaled_point_iterator temp(*this);
        vector += dimention;
        return temp;
    }

    friend bool operator==(sexp_scaled_point_iterator const & a, sexp_scaled_point_iterator const & b)
    {
        return (a.vector == b.vector) && (a.dimention == b.dimention);
    }

    friend bool operator!=(sexp_scaled_point_iterator const & a, sexp_scaled_point_iterator const & b)
    {
        return !(a == b);
    }
};

} // extern "C";
