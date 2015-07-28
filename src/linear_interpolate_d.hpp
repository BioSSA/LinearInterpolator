#pragma once

#ifndef DCGAL_USE_GMP
#define DCGAL_USE_GMP
#endif
#ifndef DCGAL_USE_MPFR
#define DCGAL_USE_MPFR
#endif

#include <CGAL/Cartesian_d.h>
#include <CGAL/Delaunay_d.h>

#include <R.h>
#include <Rinternals.h>

extern "C"
{
  //vector<double>
  SEXP linear_interpolate_d(SEXP dimentions,    //unsigned
                            SEXP points,        //vector<Point>
                            SEXP values,        //vector<double>
                            SEXP xi);           //vector<Point>
}
