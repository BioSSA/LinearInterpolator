#include "linear_interpolate_3.hpp"
#include <R_ext/BLAS.h>
#include <R_ext/Lapack.h>

#include <vector>
typedef CGAL::Cartesian<double> K;

template < class GT, class Vb = CGAL::Triangulation_vertex_base_3<GT> >
class My_vertex_base : public Vb {
public:
  typedef typename Vb::Vertex_handle Vertex_handle;
  typedef typename Vb::Cell_handle Cell_handle;
  typedef typename Vb::Point Point;
  template < class TDS2 >
    struct Rebind_TDS {
      typedef typename Vb::template Rebind_TDS<TDS2>::Other Vb2;
      typedef My_vertex_base<GT, Vb2> Other;
    };
  My_vertex_base() {}
  My_vertex_base(const Point& p)
    : Vb(p) {}
  My_vertex_base(const Point& p, Cell_handle c)
    : Vb(p, c) {}

  double value;
};


typedef CGAL::Triangulation_data_structure_3<My_vertex_base<K> > Tds;
typedef CGAL::Delaunay_triangulation_3<K, Tds, CGAL::Fast_location> Delaunay;

typedef Delaunay::Cell_handle Cell_handle;
typedef Delaunay::Vertex_handle Vertex_handle;
typedef Delaunay::Locate_type Locate_type;
typedef Delaunay::Point Point;



static std::vector<double> find_barycentric_coords(std::size_t size,
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




class LinearInterpolator_3 {
public:
  LinearInterpolator_3(const double *points,
                       const double *values,
                       size_t npoints) {
    for (size_t i{0}; i < npoints; ++i) {
      Point p{points[3*i], points[3*i + 1], points[3*i + 2]};
      Vertex_handle vertex = t.insert(p);
      vertex->value = values[i];
    }
  }

  double nnInterpolation(const double *x) const {
    Point p{x[0], x[1], x[2]};
    Vertex_handle vertex = t.nearest_vertex(p);
    return vertex->value;
  }

  double linearInterpolation(const double *x) const {
    Point p{x[0], x[1], x[2]};
    Locate_type lt;
    int li, lj;

    Cell_handle c = t.locate(p, lt, li, lj);
    if (lt == Locate_type::OUTSIDE_CONVEX_HULL) {
      return NA_REAL;
    }

    std::vector<Point> points;
    std::vector<double> values;
    for (int i = 0; i < 4; ++i) {
      points.push_back(c->vertex(i)->point());
      values.push_back(c->vertex(i)->value);
    }

    auto bc = find_barycentric_coords(3, p, points);

    return std::inner_product(values.begin(), values.end(), bc.begin(), 0.0);
  }

private:
  Delaunay t;
};

SEXP linear_interpolate_3(SEXP points,
                          SEXP values,
                          SEXP xi) {
  int d = 3;

  LinearInterpolator_3 li(REAL(points), REAL(values), length(values));

  int result_length = length(xi) / d;
  SEXP results = PROTECT(allocVector(REALSXP, result_length));

  for (size_t i{0}; i < result_length; ++i) {
    REAL(results)[i] = li.linearInterpolation(REAL(xi) + i*d);
  }

  UNPROTECT(1);
  return results;
}
