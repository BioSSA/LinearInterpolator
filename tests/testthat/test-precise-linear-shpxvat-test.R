context("ND")

library(LinearInterpolator)

get_linear <- function(d, rand = runif, args = list(min=1, max=100)) {
  const <- do.call(rand, c(args, n=1))
  coeffs <- do.call(rand, c(args, n=d))
  
  function(x) {
    stopifnot(length(x) == length(coeffs))
    sum(x * coeffs) + const
  }
}

get_rect_points <- function(d, n, rand = runif, args = list(min=0, max=10), corners=FALSE) {
  points_count <- n
  points <- matrix(
    do.call(rand, c(args, n=points_count*d)),
    ncol = d,
    byrow = TRUE
  )
  if (!corners) {
    return(points)
  }
  corners <- as.matrix(expand.grid(lapply(1:d, function(i) c(args$min, args$max))))
  rbind(points, corners)
}

get_values <- function(points, func) {
  sapply(
    1:nrow(points),
    function(i) {
      func(points[i,])
    }
  )
}

get_test <- function(d, n, m, cran=TRUE) {
  if (!cran) {
    skip_on_cran()
  }

  func <- get_linear(d)
  
  points <- get_rect_points(d, n, corners=TRUE)
  values <- get_values(points, func)
  
  xi <- get_rect_points(d, m)
  expected <- get_values(xi, func)
  
  expect_equal(linear.interpolate(xi, points, values), expected, tolerance = 1e-7)
}

test_that("Random 2D", get_test(2, 1000000, 1000000, cran=FALSE))
test_that("Random 3D", get_test(3, 10000, 10000))
test_that("Random 4D", get_test(4, 1000, 1000))
test_that("Random 5D", get_test(5, 100, 100))

