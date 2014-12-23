context("Shpxvat")

test_that("Big test", {
  skip_on_cran()
  
  d <- 2
  n <- 100000
  m <- 1000
  
  points <- matrix(
    runif(d * n, 0, 100),
    ncol = d,
    byrow=TRUE
  )
  
  values <- runif(n, -10, 10)
  
  xi <- matrix(
    runif(d * m, -5, 105),
    ncol = d,
    byrow=TRUE
  )
  
  expect_equal(length(linear.interpolate(xi, points, values)), m)
})