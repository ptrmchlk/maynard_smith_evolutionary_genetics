# Age sitribution from chapter 3 JMS, by PM
df <- read.csv("ch3_age_popsize.res")
ddf <- read.csv("ch3_age_distribution.res", header = FALSE)
dd <- df[, 1]

png(
  filename = "ch3_age_distribution.png",
  width = 20, height = 10, units = "cm", res = 600, type = "cairo"
)
par(mfrow = c(1, 2))

plot(NULL,
  type = "n", bty = "n",
  ylim = c(0, 5 * ceiling(max(dd) / 5)),
  xlim = c(1, length(dd)),
  axes = FALSE,
  xlab = "", ylab = ""
)
box(which = "plot", lwd = 1)
axis(1, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, padj = -1)
axis(2, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, padj = 1)
axis(3, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, labels = FALSE)
axis(4, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, labels = FALSE)
mtext("Generations", 1, line = 1.5)
mtext("N", 2, line = 1.5)
abline(h = 0, lty = 2, lwd = 0.5)
abline(v = 0, lty = 2, lwd = 0.5)

lines(y = dd, x = seq(1, length(dd)), lwd = 1)

plot(NULL,
  type = "n", bty = "n",
  ylim = c(0, max(ddf)),
  xlim = c(1, nrow(ddf)),
  axes = FALSE,
  xlab = "", ylab = ""
)
box(which = "plot", lwd = 1)
axis(1, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, padj = -1)
axis(2, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, padj = 1)

mtext("Generations", 1, line = 1.5)
mtext("Fraction of age", 2, line = 1.5)
mtext("age", 4, line = 0.75)

#abline(h = 0, lty = 2, lwd = 0.5)
for (i in seq(1, length(ddf))) {
  lines(y = ddf[, i], x = seq(1, nrow(ddf)), lwd = 1)

  if (i < 5) {
    text(
      labels = i - 1,
      y = ddf[nrow(ddf), i], x = nrow(ddf) + 80, xpd = NA
    )
  }
}
dev.off()
