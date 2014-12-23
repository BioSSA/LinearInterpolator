context("2D")

test_that("Initial", {
  d <- 2

  points <- matrix(
    c(
      0, 0,
      0, 2,
      2, 0,
      0.5, 0.5
    ),
    ncol = d,
    byrow=TRUE
  )

  values <- c(
    1,
    1,
    1,
    4
  )

  xi <- matrix(
    c(
      0.3, 0.2,
      1.1, 1.1
    ),
    ncol = d,
    byrow=TRUE
  )

  expect_equal(linear.interpolate(xi, points, values), c(2.2, NA), tolerance = 1e-5)
})

test_that("Simple 1", {
  d <- 2  
  
  points <- matrix(
    c(
      3, 0,
      0, 13,
      0, 0
    ),
    ncol = d,
    byrow=TRUE
  )
  
  values <- c(
    1,
    2,
    3
  )
  
  xi <- matrix(
    c(
      .3, .2,
      .1, 12.,
      1.4, 6.5,
      2.,  1.
    ),
    ncol = d,
    byrow=TRUE
  )
  
  expect_equal(linear.interpolate(xi, points, values), c(2.784615, 2.010256, 1.566667, 1.589744), tolerance = 1e-5)
})


test_that("Simple 1", {
  d <- 2

  points <- matrix(
    c(
      2, 3,
      10, 2.5,
      7, 42
    ),
    ncol = d,
    byrow=TRUE
  )

  values <- c( 
    .5,
    87,
    21
  )

  xi <- matrix(
    c(
      3, 3,
      7, 41,
      8, 16
    ),
    ncol = d,
    byrow=TRUE
  )
  
  expect_equal(linear.interpolate(xi, points, values), c(11.25914, 21.85374, 53.95628), tolerance = 1e-5)
})
