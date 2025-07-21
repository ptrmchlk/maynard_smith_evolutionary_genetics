# Plotting the weasel, PM
df <- read.csv("weasel_results.res")

round_up <- function(x) {
  5 * ceiling(x / 5)
}

png(
  filename = "weasel.png",
  width = 10, height = 10, units = "cm", res = 600, type = "cairo"
)
plot(NULL,
  type = "n", bty = "n",
  ylim = c(0, round_up(max(df$fitness))),
  xlim = c(1, round_up(max(df$ngen))),
  axes = FALSE,
  xlab = "", ylab = ""
)
box(which = "plot", lwd = 1)
axis(1, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, padj = -1)
axis(2, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, padj = 1)
axis(3, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, labels = FALSE)
axis(4, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, labels = FALSE)
mtext("Generations", 1, line = 1.5)
mtext("Fitness", 2, line = 1.5)
abline(h = 19, lty = 2, lwd = 0.5)
abline(h = 0, lty = 2 , lwd = 0.5)

for (i in unique(df$nrun)) {
  lines(
    y = df[df$nrun == i, ]$fitness, x = df[df$nrun == i, ]$ngen,
    lwd = 1, col = adjustcolor(1, 0.5), lty = "dotted"
  )
}
lines(y = df[df$nrun == 1, ]$fitness, x = df[df$nrun == 1, ]$ngen, lwd = 2)

dev.off()
