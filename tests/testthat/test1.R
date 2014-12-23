#source(system.file("extdata", "common.test.methods.R", package = "Rssa"))
context("First")

test_that("Simple test 1", function() {
  d <- 2

  points <- matrix(
    c(
      c(0, 0),
      c(0, 2),
      c(2, 0),
      c(0.5, 0.5)
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
      c(0.3, 0.2)
    ),
    ncol = d,
    byrow=TRUE
  )

  expect_that(linear.interpolate(xi, points, values), c(1.6, NA))
})
