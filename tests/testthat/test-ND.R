context("ND")

test_that("5 D", {
  d <- 5

  points <- matrix(
    c(
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 1,
      0, 0, 0, 1, 0,
      0, 0, 1, 0, 0,
      0, 1, 0, 0, 0,
      1, 0, 0, 0, 0
    ),
    ncol = d,
    byrow=TRUE
  )

  values <- c(
    1,
    2,
    3,
    4,
    5,
    6
  )

  xi <- matrix(
    c(
      .1, .1, .1, .1, .1,
      .2, .2, .2, .2, .1,
      .5, .5, .5, .5, .5
    ),
    ncol = d,
    byrow=TRUE
  )

  expect_equal(linear.interpolate(xi, points, values), as.numeric(c(2.5, 3.9, NA)), tolerance = 1e-5)
})
