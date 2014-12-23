context("5D")

test_that("Simple", {
  d <- 5
  
  points <- matrix(
    c(
      0, 0, 0, 0, 1,
      0, 0, 0, 2, 0,
      0, 0, 3, 0, 0,
      0, 4, 0, 0, 0,
      5, 0, 0, 0, 0,
      1, 0, 1, 0, 1,
      0, 2, 0, 3, 0
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
    6,
    7
    
  )
  
  xi <- matrix(
    c(
      .3, .7, 0, 0, .1,
      1, 1, .5, 0, 3,
      .5, .5, .5, .5, .5
    ),
    ncol = d,
    byrow=TRUE
  )
  
  expect_equal(linear.interpolate(xi, points, values), as.numeric(c(NA, NA, 3.296875)), tolerance = 1e-5)
})