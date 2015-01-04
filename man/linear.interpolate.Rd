\name{linear.interpolate}

\alias{linear.interpolate}

\title{n dimentional interpolation}


\description{
This function makes the Delaunay triangulation and then returns new values recalculated for given points.
}

\usage{
linear.interpolate(x, points, values, fill_value=NA, scale=FALSE, circular=FALSE)
}

\arguments{
  \item{x}{New points represented as matrix. Each row is a n-dimensional point.}
  \item{points}{Initial points represented as matrix. Each row is a n-dimensional point. It's assumed that ncol(x) == ncol(points).}
  \item{values}{Vector. Values for each of initial points. It's assumed that lengt(values) == nrow(points).}
  \item{fill_value}{Optional. Value used to fill in for requested points outside of the convex hull or points with NA value in (at least) one point of it's simplex. If not provided, then the default is nan.}
  \item{scale}{Optional. Either a logical value or a numeric vector of length equal to the number of dimensions. If FALSE, then nothing happens. If TRUE all points will be rescaled to unit cube before performing interpolation. If vector, they'll be rescaled with scale as coefficients. Vector's length schould be equal to dimension of points. This is useful if some of the input dimensions have incommensurable units and differ by many orders of magnitude.}
  \item{circular}{Optional. Either a logical value or a numeric vector of length equal to the number of dimensions. Indicates in which directions data should be replicated in order to support torus topology.}
}

\details{
  \code{linear.interpolate} Returns vector of recalculated values for given points.
}

\examples{
d <- 2
points <- c(0, 0, 2, .5,
            0, 2, 0, .5 )
                               # dots are: (0, 0), (0, 2), (2, 0), (.5, .5)

dim(points) = c(length(points) / d, d)
values <- c(1,  1,  1,  4)

xi <- c(1., .7,
        .1, 1.8 )
dim(xi) <- c(length(xi) / d, d)
                               # dots are: (1., ,1), (.7, 1.8)

linear.interpolate(xi, points, values)
                               # returns (1.6, NA)
}


\author{Anya Yaveyn, Svyatoslav Gryaznov}
