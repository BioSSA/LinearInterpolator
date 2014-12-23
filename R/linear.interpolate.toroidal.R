make.toroidal <- function(points, values) {
  d <- ncol(points)

  shifts <- sapply(
    1:d,
    function(x) { max(points[, x]) - min(points[, x]) }
  )
  directions <- c(0, 1, -1)
  all_shifts <- expand.grid(
    lapply(
      as.list(shifts),
      function(shift) {
        shift * directions
      }
    )
  )

  toroidal_points <- c()
  for (i in 1:nrow(all_shifts)) {
    shift <- unlist(all_shifts[i, ])
    toroidal_points <- rbind(toroidal_points, sweep(points, d, shift))
  }

  toroidal_values <- rep(values, nrow(all_shifts))

  list(points = toroidal_points, values = toroidal_values)
}

linear.interpolate.toroidal <- function(x, points, values, fill_value=NA, scale=FALSE) {
  tor = make.toroidal(points, values)
  linear.interpolate(x, tor$points, tor$values, fill_value, scale)
}
