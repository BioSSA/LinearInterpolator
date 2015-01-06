#pragma once

#ifndef DCGAL_USE_GMP
#define DCGAL_USE_GMP
#endif
#ifndef DCGAL_USE_MPFR
#define DCGAL_USE_MPFR
#endif

#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_vertex_base_3.h>

#include <CGAL/Cartesian.h>
#include <CGAL/Triangulation_3.h>

#include <R.h>
#include <Rinternals.h>

extern "C" {
  SEXP linear_interpolate_3(SEXP points,
                            SEXP values,
                            SEXP xi);
}
