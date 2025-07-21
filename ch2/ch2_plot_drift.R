# Plotting drift, PM
df <- read.csv("drift_results.res", header = TRUE)

png(filename = "drift.png",
    width = 12.5, height = 10, units = "cm", res = 600, type = "cairo")
  plot(NULL,
    type = "n", bty = "n",
    ylim = c(0, 1),
    xlim = c(1, 50 * ceiling(max(df$ngen) / 50)),
    axes = FALSE,
    xlab = "", ylab = ""
  )

  box(which = "plot", lwd = 1)
  axis(1, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, padj = -1)
  axis(2, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, padj = 1)
  axis(3, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, labels = FALSE)
  axis(4, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, labels = FALSE)
  mtext("Generations", 1, line = 1.5)
  mtext("Fraction of fixed type", 2, line = 1.5)
  abline(h = 1, lty = 2, lwd = 0.5)
  abline(h = 0, lty = 2 , lwd = 0.5)
  for (i in unique(df$nrun)) {
    dm <- as.matrix(df[df$nrun == i, ])[, -c(1, 2)]
    xx <- df[df$nrun == i, ]$ngen
    fixedType <- dm[nrow(dm), 1]
    all(dm[nrow(dm), ] == fixedType)
    fractionType <- apply(dm, 1, function(x) sum(x == fixedType) / ncol(dm))
    if (i == 1) {
      lines(y = fractionType, x = xx, lwd = 1.25, col = 1)
    } else {
      lines(y = fractionType, x = xx, lwd = 0.75, col = adjustcolor(1, 0.5))
    }
  }

dev.off()
