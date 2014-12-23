context("2D")

test_that("One line", {
  d <- 2
  
  points <- matrix(
    c(
      0, 0,
      0, 13,
      0, 1
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
      .2, .3,
      1, 1,
      .5, .51,
      .49, .5
    ),
    ncol = d,
    byrow=TRUE
  )
  
  expect_equal(linear.interpolate(xi, points, values), as.numeric(c(NA, NA, NA, NA)), tolerance = 1e-5)
})

test_that("One circle", {
  d <- 2
  
  points <- matrix(
    c(
      0, 0,
      2, 0,
      .5, -1.5,
      .5, .5
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
      0, 3
    ),
    ncol = d,
    byrow=TRUE
  )
  
  expect_equal(length(linear.interpolate(xi, points, values)), 1)
})
