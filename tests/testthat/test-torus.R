context("2D")

test_that("One line", {
  d <- 2

  points <- matrix(
    c(
      0, .4,
      0, 2,
      2, 0,
      .5, .5
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
      .2, .3,
      1.7, .8
    ),
    ncol = d,
    byrow=TRUE
  )

  expect_equal(linear.interpolate(xi, points, values, circular=c(FALSE, TRUE)), as.numeric(c(2.2, 1.6)), tolerance = 1e-5)
})
