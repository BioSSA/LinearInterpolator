linear.interpolate <- function(x, points, values, fill_value=NA, scale=FALSE) {
  #stopifnot(load.python())

  x <- as.matrix(x)
  points <- as.matrix(points)
  values <- as.vector(values)
  fill_value <- as.vector(fill_value)

  if (length(values) < nrow(points)) {
    values <- values + numeric(nrow(points))
  }

  stopifnot(length(values) == nrow(points))
  stopifnot(length(fill_value) == 1)

  d <- ncol(points)

  if (is.logical(scale)) {
    if (scale) {
      scale <- sapply(1:d, function(x) { 1. / (max(points[, x]) - min(points[, x]))  })
    } else {
      scale <- rep(1, d)
    }
  } else {
    scale <- as.vector(scale)
  }

  stopifnot(length(scale) == ncol(points))

  x <- as.double(x)
  points <- as.double(points)
  values <- as.double(values)
  scale <- as.double(scale)
  if (!is.na(fill_value) & !is.nan(fill_value)) {
    fill_value <- as.double(fill_value)
  }

  storage.mode(x) <- storage.mode(points) <- storage.mode(values) <- storage.mode(fill_value) <- storage.mode(scale) <- "double"
  storage.mode(d) <- "integer"

  res <- .Call(linear_interpolate_d, d, points, values, x, fill_value, scale)

  res <- lapply(res, function(x) if (is.null(x)) NA_real_ else as.double(x))
  res <- unlist(res)
  res <- as.double(res)

  res[is.nan(res)] <- NA_real_

  res
}
