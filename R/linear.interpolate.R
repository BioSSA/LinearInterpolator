linear.interpolate <- function(x, points, values,
                               scale = FALSE, circular = FALSE) {
  x <- as.matrix(x)
  points <- as.matrix(points)
  values <- as.vector(values)

  d <- ncol(points)

  values <- rep_len(values, nrow(points)) # TODO Add recycling warning
  scale <- rep_len(scale, d)
  circular <- rep_len(circular, d)

  stopifnot(all(is.finite(points)))
  stopifnot(all(is.finite(x)))

  ranges <- apply(points, 2, function(x) diff(range(x)))

  if (is.logical(scale)) {
    scale <- ifelse(scale, ranges, 1)
  }

  if (is.logical(circular)) {
    circular <- ifelse(circular, ranges, Inf)
  }

  for (j in which(circular < Inf)) {
    per <- circular[j]
    x[, j] <- x[, j] %% per
    points[, j] <- points[, j] %% per
    points.p <- points.m <- points
    points.p[, j] <- points.p[, j] + per
    points.m[, j] <- points.m[, j] - per
    points <- rbind(points.m, points, points.p)

    values <- rep(values, 3)
  }

  x <- scale(x, center = FALSE, scale = scale)
  points <- scale(points, center = FALSE, scale = scale)

  shrink <- function(x, center = colMeans(x), lambda = 0.9999) {
    x <- as.matrix(x)
    (x - rep(center, each = nrow(x))) * lambda + rep(center, each = nrow(x))
  }

  x <- shrink(x, center = colMeans(points))

  x <- as.double(t(x))
  points <- as.double(t(points))
  values <- as.double(values)
  scale <- as.double(scale)

  storage.mode(x) <- storage.mode(points) <- storage.mode(values) <- "double"
  storage.mode(d) <- "integer"

  if (d == 3)
    .Call("linear_interpolate_3", points, values, x)
  else if (d == 1)
    .Call("linear_interpolate_1", points, values, x)
  else
    .Call("linear_interpolate_d", d, points, values, x)
}
