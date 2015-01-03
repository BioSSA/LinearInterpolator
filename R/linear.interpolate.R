linear.interpolate <- function(x, points, values,
                               fill_value = NA_real_,
                               scale = FALSE, circular = FALSE) {
  x <- as.matrix(x)
  points <- as.matrix(points)
  values <- as.vector(values)
  fill_value <- fill_value[[1]]

  circular <- as.logical(circular)

  d <- ncol(points)

  if (length(values) < nrow(points)) {
    values <- values + numeric(nrow(points))
  }

  stopifnot(length(values) == nrow(points))
  stopifnot(all(is.finite(points)))
  stopifnot(all(is.finite(x))) # TODO Allow NAs in input values

  scale <- rep(scale, d)[seq_len(d)]

  if (is.logical(scale)) {
    scale <- ifelse(scale,
                    apply(points, 2, function(x) diff(range(x))),
                    1)
  }

  stopifnot(length(scale) == d)

  circular <- rep(circular, d)[seq_len(d)]
  if (is.logical(circular)) {
    circular <- ifelse(circular,
                       apply(points, 2, function(x) diff(range(x))),
                       Inf)
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

  x <- as.double(x)
  points <- as.double(points)
  values <- as.double(values)
  scale <- as.double(scale)
  if (!is.na(fill_value) & !is.nan(fill_value)) {
    fill_value <- as.double(fill_value)
  }

  storage.mode(x) <- storage.mode(points) <- storage.mode(values) <- storage.mode(fill_value) <- storage.mode(scale) <- "double"
  storage.mode(d) <- "integer"

  .Call("linear_interpolate_d", d, points, values, x, fill_value, scale)
}
