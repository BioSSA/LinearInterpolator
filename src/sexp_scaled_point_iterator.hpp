#pragma once

#include <iterator>

#include <R.h>
#include <Rinternals.h>

class sexp_scaled_point_iterator
   : public std::iterator<std::input_iterator_tag, double>
{

private:

    double * vector;
    size_t dimention;
    double * scale_coeffs;

public:

    sexp_scaled_point_iterator(const SEXP & val, size_t d, size_t begin_index)
        : vector(REAL(val) + begin_index)
        , dimention(d)
    { }

    sexp_scaled_point_iterator(const SEXP & val, size_t d, size_t begin_index, const SEXP scale_coeffs)
        : vector(REAL(val) + begin_index)
        , dimention(d)
        , scale_coeffs(REAL(scale_coeffs))
    { }

    sexp_scaled_point_iterator (const sexp_scaled_point_iterator& x) = default;

    double operator*() const
    {
        return (*vector) * (*scale_coeffs);
    }

    sexp_scaled_point_iterator& operator++()
    {
        vector += dimention;
        ++scale_coeffs;
        return *this;
    }

    sexp_scaled_point_iterator operator++(int)
    {
        sexp_scaled_point_iterator temp(*this);
        vector += dimention;
        ++scale_coeffs;
        return temp;
    }

    friend bool operator==(sexp_scaled_point_iterator const & a, sexp_scaled_point_iterator const & b)
    {
        return (a.vector == b.vector)
            && (a.dimention == b.dimention);
    }

    friend bool operator!=(sexp_scaled_point_iterator const & a, sexp_scaled_point_iterator const & b)
    {
        return !(a == b);
    }
};
