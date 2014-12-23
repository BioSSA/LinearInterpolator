#source(system.file("extdata", "common.test.methods.R", package = "Rssa"))
context("First")

test_that("Simpe test1", function() {
  d <- 2

  points <- c(
    c(0, 0),
    c(0, 2),
    c(2, 0),
    c(0.5, 0.5)
  )
  dim(points) <- c(d, length(points) / d)
  points <- t(points)

  values <- c(
    1,
    1,
    1,
    4
  )

  xi <- c(
    c(0.4, 0.1),
    c(0.7, 1.8)
  )
  dim(xi) <- c(d, length(xi) / d)
  xi <- t(xi)

  res <- linear.interpolate(xi, points, values)

  expect_that(res, c(1.6, NA))
})
