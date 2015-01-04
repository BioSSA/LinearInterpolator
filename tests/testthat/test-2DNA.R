context("2D")

test_that("Outsider", {
  d <- 2

  points <- matrix(
    c(
      0, 0,
      0, 1,
      1, 0
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

  expect_equal(linear.interpolate(xi, points, values), c(1.70, NA, NA, 2.48), tolerance = 1e-5)
})

test_that("Skipped value", {
  d <- 2

  points <- matrix(
    c(
      0, 0,
      0, 1,
      1, 0,
      2, 1
    ),
    ncol = d,
    byrow=TRUE
  )

  values <- c(
    NA,
    2,
    3,
    4
  )

  xi <- matrix(
    c(
      .2,  .3,
      .49, .5,
      .5,  .51
    ),
    ncol = d,
    byrow=TRUE
  )

  expect_equal(linear.interpolate(xi, points, values), c(NA, NA, 2.5), tolerance = 1e-5)
})

test_that("Fill value", {
  d <- 2

  points <- matrix(
    c(
      0, 0,
      0, 1,
      1, 0
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
      -.01, .1,
      .49, .49,
      1, .51,
      .49, .52,
      7, 7
    ),
    ncol = d,
    byrow=TRUE
  )

  expect_equal(linear.interpolate(xi, points, values), c(NA, 2.47, NA, NA, NA), tolerance = 1e-5)
})
