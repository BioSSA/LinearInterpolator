#include <vector>
#include <R_ext/BLAS.h>
#include <R_ext/Lapack.h>

template<typename TPoint>
std::vector<double> find_barycentric_coords(std::size_t size,
                                            TPoint const &point,
                                            std::vector<TPoint> const &simplex) {
  std::vector<double> a(size * size);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      a[i*size+j] = *(simplex[i].cartesian_begin()+j) - *(simplex[size].cartesian_begin()+j);
    }
  }

  std::vector<double> b(size + 1);
  for (size_t i = 0; i < size; ++i) {
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

  if (info != 0) {
    error("Bad matrix");
  }

  // calculate last coordinate
  b[size] = 1 - std::accumulate(b.begin(), b.end() - 1, 0.);

  return b;
}
