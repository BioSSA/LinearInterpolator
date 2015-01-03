context("3D")

test_that("Simple", {
  d <- 3

  points <- matrix(
    c(
      0, 0, 0,
      0, 0, 1,
      0, 1, 0,
      1, 0, 0
    ),
    ncol = d,
    byrow=TRUE
  )

  values <- c(
    1,
    2,
    3,
    4
  )

  xi <- matrix(
    c(
      .3, .3, .3,
      -.1, .1, .1
    ),
    ncol = d,
    byrow=TRUE
  )

  expect_equal(linear.interpolate(xi, points, values), c(2.8, NA), tolerance = 1e-5)
})

test_that("One plane", {
  d <- 3

  points <- matrix(
    c(
      0, 0, 1,
      0, 1, 0,
      1, 0, 0,
      .1, .1, .8
    ),
    ncol = d,
    byrow=TRUE
  )

  values <- c(
    1,
    2,
    3,
    4
  )

  xi <- matrix(
    c(
      0.8, 0.8, .08,
      1.1, 0, 0
    ),
    ncol = d,
    byrow=TRUE
  )

  expect_equal(linear.interpolate(xi, points, values), as.numeric(c(NA, NA)))
})
