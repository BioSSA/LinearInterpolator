#pragma once

#ifndef DCGAL_USE_GMP
#define DCGAL_USE_GMP
#endif
#ifndef DCGAL_USE_MPFR
#define DCGAL_USE_MPFR
#endif

#include <cstddef>

#include <CGAL/Cartesian_d.h>
#include <CGAL/Delaunay_d.h>

#include <R.h>
#include <Rinternals.h>
#include <R_ext/BLAS.h>
#include <R_ext/Lapack.h>

typedef CGAL::Cartesian_d<double> R;
typedef CGAL::Delaunay_d<R>       Delaunay;
typedef Delaunay::Point_d         Point;
typedef Delaunay::Vertex_handle   Vertex_handle;

extern "C"
{
  //vector<double>
  SEXP linear_interpolate_d(SEXP dimentions,  //unsigned
                            SEXP points,      //vector<Point>
                            SEXP values,      //vector<double>
                            SEXP xi,          //vector<Point>
                            SEXP fill_value); //double

  std::vector<double> find_barycentric_coords(std::size_t size,
                                              Point const & point,
                                              std::vector<Point> const & simplex);
}
