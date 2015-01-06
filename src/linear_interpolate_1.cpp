#include "linear_interpolate_1.hpp"
#include <algorithm>
#include <vector>

struct Vertex {
  double x, value;
  bool operator<(const Vertex &b) const {
    return x < b.x;
  }
};

class LinearInterpolator_1 {
public:
  LinearInterpolator_1(const double *points,
                       const double *values,
                       size_t npoints) {
    for (size_t i{0}; i < npoints; ++i) {
      vertices.push_back({points[i], values[i]});
    }

    std::sort(vertices.begin(), vertices.end());
  }

  double linearInterpolation(const double *x) const {
    Vertex p{*x, 0};
    size_t u = std::lower_bound(vertices.begin(), vertices.end(), p) - vertices.begin();
    size_t l = u - 1;
    if (u <= 0 || u >= vertices.size()) {
      return NA_REAL;
    }

    double b = (vertices[u].x - *x) / (vertices[u].x - vertices[l].x);

    return vertices[l].value * b + vertices[u].value * (1 - b);
  }

private:
  std::vector<Vertex> vertices;
};

SEXP linear_interpolate_1(SEXP points,
                          SEXP values,
                          SEXP xi) {
  const int d = 1;

  LinearInterpolator_1 li(REAL(points), REAL(values), length(values));

  int result_length = length(xi) / d;
  SEXP results = PROTECT(allocVector(REALSXP, result_length));

  for (size_t i{0}; i < result_length; ++i) {
    REAL(results)[i] = li.linearInterpolation(REAL(xi) + i*d);
  }

  UNPROTECT(1);
  return results;
}
