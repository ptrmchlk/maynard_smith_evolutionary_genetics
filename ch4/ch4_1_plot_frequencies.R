# Chapter 4 allele frequencies JMS, PM
dd <- read.csv("ch4_allele_frequency.res", header = TRUE)
df <- dd

# 2*dd$A*(1-dd$A)*2*dd$B*(1-dd$B)
# max(dd)
# subset(dd, k == unique(dd$k)[1])
{
  png(
    filename = "ch4_allele_freq.png",
    width = 30, height = 30, units = "cm", res = 600, type = "cairo"
  )
  par(mfrow = c(length(unique(dd$k)), length(unique(dd$h))),
      oma = c(2, 2, 0, 0), mar = c(3, 3, 3, 2))
  for (j in unique(df$k)) {
    for (i in unique(df$h)) {
      plot(NULL,
        type = "n", bty = "n",
        ylim = c(0, 1),
        xlim = c(0, 1),
        axes = FALSE,
        xlab = "", ylab = ""
      )
      box(which = "plot", lwd = 1)
      axis(1, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, padj = -1)
      axis(2, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, padj = 1)
      axis(3, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, labels = FALSE)
      axis(4, lwd = 0.0, lwd.ticks = 1, tcl = -0.4, labels = FALSE)

      mtext("p(A)", 1, line = 1.5)
      mtext("p(B)", 2, line = 1.5)
      abline(a = 0, b = 1, lty = 2, lwd = 0.25)
      abline(h = 0, lty = 2, lwd = 0.25)
      abline(v = 0, lty = 2, lwd = 0.25)
      abline(h = 1, lty = 2, lwd = 0.25)
      abline(v = 1, lty = 2, lwd = 0.25)
      dm <- subset(df, h == i & k == j)
      points(
        x = dm$A[length(dm$A)], y = dm$A[length(dm$B)],
        pch = 16, cex = 0.5
      )
      # text(labels = i, x = dm$A[length(dm$A)], y = dm$A[length(dm$B)], pch = 16, cex = 0.5)
      lines(x = dm$A, y = dm$B, lwd = 1, lty = )
      text(
        labels = paste("h = ", i, "; k = ", j),
        x = 0.95, y = 0.05, xpd = TRUE, adj = c(1, 0), cex = 1.2
      )
    }
  }
  dev.off()
}