#include <algorithm>
//#include <cassert>
#include <cmath>
//#include <limits>
#include <map>
//#include <memory>
#include <numeric>
#include <string>
#include <vector>

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

#include "sexp_matrix_iterator.h"

typedef CGAL::Cartesian_d<double> R;
typedef CGAL::Delaunay_d<R>       Delaunay;
typedef Delaunay::Point_d         Point;
typedef Delaunay::Vertex_handle   Vertex_handle;

extern "C"
{
  //vector<double>
SEXP interpolate_d(
    SEXP dimentions, //unsigned
    SEXP points,     //vector<Point>
    SEXP values,     //vector<double>
    SEXP xi,         //vector<Point>
    SEXP fill_value); //double
}

std::vector<double> find_barycentric(
    unsigned curr_d,
    Point const & point,
    std::vector<Point> const & vertices)
{
  return std::vector<double>(curr_d + 1, 1. / (curr_d + 1));
  //todo: find barycentric coords
}

//vector<double>
SEXP interpolate_d(
    SEXP dimentions, //unsigned
    SEXP points,     //vector<Point>
    SEXP values,     //vector<double>
    SEXP xi,         //vector<Point>
    SEXP fill_value) //double
{
  auto d = INTEGER(dimentions)[0];

  Delaunay triangulation(d);
  std::map<Vertex_handle, double> vertices2vals;
  int points_count = length(values);
  for (auto i = 0; i < points_count; ++i)
  {
    // implementing funny custom SEXP iterator can be avoided by using Rcpp package ^.^
    Point point(
          d,
          sexp_matrix_iterator(points, points_count, i),
          sexp_matrix_iterator(points, points_count, i + d * points_count));
    auto vertex_handle = triangulation.insert(point);
    vertices2vals[vertex_handle] = REAL(values)[i];
  }

  auto double_fill_value = REAL(fill_value)[0];

  auto recalc_point =
      [&]
      (Point const & point)
  {
    auto simplex = triangulation.locate(point);
    if (simplex == Delaunay::Simplex_handle())
    {
      //return double_fill_value;
      return 17.;
    }
    std::vector<Point>  vertices;
    std::vector<double> vertex_vals;
    vertices.reserve(d + 1);
    vertex_vals.reserve(d + 1);
    for (auto i = 0; i <= triangulation.current_dimension(); ++i)
    {
      auto vertex = triangulation.vertex_of_simplex(simplex, i);
      double vertex_val = vertices2vals[vertex];
      if (ISNAN(vertex_val))
      {
        return double_fill_value;
      }
      vertices.push_back(triangulation.associated_point(vertex));
      vertex_vals.push_back(vertex_val);
    }
    auto barycentric = find_barycentric(triangulation.current_dimension(), point, vertices);
    return std::inner_product(vertex_vals.begin(), vertex_vals.end(), barycentric.begin(), 0.);
  };

  auto result_length = length(xi) / d;
  SEXP results = PROTECT(allocVector(REALSXP, result_length));
  for(int i = 0; i < result_length; ++i)
  {
    Point point(
        d,
        sexp_matrix_iterator(xi, result_length, i),
        sexp_matrix_iterator(xi, result_length, i + result_length * d));
    REAL(results)[i] = recalc_point(point);
  }
  UNPROTECT(1);
  return results;
}

