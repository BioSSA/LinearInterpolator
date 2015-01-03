make.toroidal <- function(points, values, circular) {
  d <- ncol(points)

  shifts <- sapply(
    1:d,
    function(x) {
      if (circular[x]) {
        max(points[, x]) - min(points[, x])
      } else {
        0
      }
    }
  )
  directions <- c(0, 1, -1)
  all_shifts <- expand.grid(
    lapply(
      as.list(shifts),
      function(shift) {
        if (shift == 0) {
          0
        } else {
          shift * directions
        }
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

  # TODO Allow to denote topology by period
  # circular <- rep(circular, d)[seq_len(d)]
  torus_enabled <- TRUE
  if (length(circular) == 1) {
    if (circular) {
      circular <- rep(TRUE, d)
    } else {
      torus_enabled <- FALSE
    }
  }

  if (torus_enabled) {
    stopifnot(length(circular) == d)
    tor = make.toroidal(points, values, circular)
    points <- tor$points
    values <- tor$values
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
