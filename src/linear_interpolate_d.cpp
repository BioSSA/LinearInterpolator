#include "linear_interpolate_d.hpp"

#include <cmath>

#include <algorithm>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "sexp_matrix_iterator.hpp"


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
  dgesv_(&size_int, &b_size, &a[0], &size_int, &ipiv[0],
    &b[0], &size_int, &info);

  // calculate last coordinate
  b[size] = 1 - std::accumulate(b.begin(), b.end() - 1, 0.);

  return b;
}

void make_toroidal(std::size_t size,
                   std::vector<Point> & points,
                   std::vector<double> & values)
{
  /*shifts = [
      np.amax(points[:, i]) - np.amin(points[:, i])
      for i in xrange(points.shape[1])
  ]
  directions = np.array([0, 1, -1])
  all_shifts = itertools.product(*[
      directions * shift for shift in shifts
  ])

  toroidal = np.empty((0,) + points.shape[1:])
  for total_shift in all_shifts:
      toroidal = np.append(toroidal, points + np.array(total_shift), axis=0)*/

  size_t old_values_size = values.size();
  size_t values_size = std::pow(3, size) * old_values_size;
  values.resize(values_size);
  for (size_t i = 1; i < values_size; ++i)
  {
    for (size_t j = 0; j < old_values_size; ++j)
    {
      values[i * old_values_size + j] = values[j];
    }
  }
}

//vector<double>
SEXP linear_interpolate_d(SEXP dimentions,
                          SEXP points,
                          SEXP values,
                          SEXP xi,
                          SEXP fill_value)
{
  auto d = INTEGER(dimentions)[0];

  Delaunay triangulation(d);
  std::map<Vertex_handle, double> vertices_to_values;
  int points_count = length(values);
  for (auto i = 0; i < points_count; ++i)
  {
    Point point(
        d,
        sexp_matrix_iterator(points, points_count, i),
        sexp_matrix_iterator(points, points_count, i + d * points_count));
    auto vertex_handle = triangulation.insert(point);
    vertices_to_values[vertex_handle] = REAL(values)[i];
  }

  auto double_fill_value = REAL(fill_value)[0];

  auto recalc_point =
      [&]
      (Point const & point)
  {
    auto simplex = triangulation.locate(point);
    if (simplex == Delaunay::Simplex_handle())
    {
      return double_fill_value;
    }
    std::vector<Point>  vertices(d + 1);
    std::vector<double> vertex_values(d + 1);
    for (auto i = 0; i <= triangulation.current_dimension(); ++i)
    {
      auto vertex = triangulation.vertex_of_simplex(simplex, i);
      double vertex_val = vertices_to_values[vertex];
      if (ISNAN(vertex_val))
      {
        return double_fill_value;
      }
      vertices[i] = triangulation.associated_point(vertex);
      vertex_values[i] = vertex_val;
    }
    auto barycentric = find_barycentric_coords(triangulation.current_dimension(), point, vertices);
    return std::inner_product(vertex_values.begin(), vertex_values.end(), barycentric.begin(), 0.);
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

