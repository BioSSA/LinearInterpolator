#include "linear_interpolate_d.hpp"
#include "find_barycentric_coords.hpp"

#include <algorithm>
#include <map>
#include <vector>

typedef CGAL::Cartesian_d<double> R;
typedef CGAL::Delaunay_d<R>       Delaunay;
typedef Delaunay::Point_d         Point;
typedef Delaunay::Vertex_handle   Vertex_handle;


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

  double linearInterpolation(const double *x) {
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
    REAL(results)[i] = li.linearInterpolation(REAL(xi) + i*d);
  }

  UNPROTECT(1);
  return results;
}
