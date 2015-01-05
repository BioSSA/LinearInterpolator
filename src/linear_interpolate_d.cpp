#include "linear_interpolate_d.hpp"

#include <cmath>

#include <algorithm>
#include <map>
#include <numeric>
#include <limits>
#include <string>
#include <tuple>
#include <vector>


std::vector<double> find_barycentric_coords(std::size_t size,
                                            Point const & point,
                                            std::vector<Point> const & simplex)
{
  std::vector<double> a(size * size);
  for (size_t i = 0; i < size; ++i)
  {
    for (size_t j = 0; j < size; ++j)
    {
      a[i*size+j] = *(simplex[i].cartesian_begin()+j) - *(simplex[size].cartesian_begin()+j);
    }
  }

  std::vector<double> b(size + 1);
  for (size_t i = 0; i < size; ++i)
  {
    b[i] = *(point.cartesian_begin()+i) - *(simplex[size].cartesian_begin()+i);
  }

  // additional variables needed for LAPACK
  std::vector<int> ipiv(size);
  int info;
  int size_int = size;
  int b_size = 1;

  // solve a*x = b
  // result will be in b
  F77_NAME(dgesv)(&size_int, &b_size, &a[0], &size_int, &ipiv[0],
    &b[0], &size_int, &info);

  if (info != 0)
  {
    error("Bad matrix");
  }

  // calculate last coordinate
  b[size] = 1 - std::accumulate(b.begin(), b.end() - 1, 0.);

  return b;
}

class LinearInterpolator_d {
public:
  LinearInterpolator_d(int d,
                       const double *points,
                       const double *values,
                       size_t npoints) : d{d}, triangulation{d}, npoints{npoints} {
    for (size_t i{0}; i < npoints; ++i) {
      Point point(d, points + i*d, points + (i + 1)*d);
      Vertex_handle vertex_handle = triangulation.insert(point);
      vertices_to_values[vertex_handle] = values[i];
    }
  }

  double operator()(const double *x) {
    const double double_fill_value = NA_REAL;

    Point point(d, x, x + d);
    auto simplex = triangulation.locate(point);
    if (simplex == Delaunay::Simplex_handle()) {
      return double_fill_value;
    }
    std::vector<Point>  vertices(d + 1);
    std::vector<double> vertex_values(d + 1);
    for (size_t i{0}; i <= triangulation.current_dimension(); ++i) {
      auto vertex = triangulation.vertex_of_simplex(simplex, i);
      double vertex_val = vertices_to_values[vertex];
      if (ISNAN(vertex_val)) {
        return double_fill_value;
      }
      vertices[i] = triangulation.associated_point(vertex);
      vertex_values[i] = vertex_val;
    }
    auto barycentric = find_barycentric_coords(triangulation.current_dimension(), point, vertices);
    return std::inner_product(vertex_values.begin(), vertex_values.end(), barycentric.begin(), 0.);
  }

private:
  const int d;
  size_t npoints;
  Delaunay triangulation;
  std::map<Vertex_handle, double> vertices_to_values;
};

SEXP linear_interpolate_d(SEXP dimentions,
                          SEXP points,
                          SEXP values,
                          SEXP xi) {
  int d = INTEGER(dimentions)[0];

  LinearInterpolator_d li(d, REAL(points), REAL(values), length(values));

  int result_length = length(xi) / d;
  SEXP results = PROTECT(allocVector(REALSXP, result_length));

  for (size_t i{0}; i < result_length; ++i) {
    REAL(results)[i] = li(REAL(xi) + i*d);
  }

  UNPROTECT(1);
  return results;
}
