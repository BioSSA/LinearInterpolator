#pragma once

#include <R.h>
#include <Rinternals.h>

extern "C"
{
  SEXP linear_interpolate_1(SEXP points,
                            SEXP values,
                            SEXP xi);
}

