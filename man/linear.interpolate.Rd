\name{linear.interpolate}

\alias{linear.interpolate}

\title{n dimentional interpolation}


\description{
This function makes the Delaunay triangulation and then returns new values recalculated for given points.
}

\usage{
linear.interpolate(x, points, values)
linear.interpolate(x, points, values, fill_value=NA)
}

\arguments{
  \item{x}{New points represented as matrix. Each row is a n-dimentional point.}
  \item{points}{Initial points represented as matrix. Each row is a n-dimentional point. It's assumed that ncol(x) == ncol(points).}
  \item{values}{Vector. Values for each of initial points. It's assumed that lengt(values) == nrow(points).}
  \item{fill_value}{Optional. Vector. Values for each of initial points. It's assumed that lengt(values) == nrow(points).}
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
                               # dots are: (1., ,1), (.7, 1.8)

linear.interpolate(xi, points, values)
                               # returns (1.6, NA)
}


\author{Anya Yaveyn, Svyatoslav Gryaznov}
