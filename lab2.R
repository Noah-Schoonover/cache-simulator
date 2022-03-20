configs <- c(
	"2way_wa",
	"2way_nwa",
	"4way_fifo",
	"4way_rand",
	"small_dm",
	"medium_dm",
	"large_dm",
	"mega")

traces <- c(
	"gcc",
	"gzip",
	"mcf",
	"swim",
	"twolf")

make_plots <- function(conf, thp, amal) {

	dev.new(width = 400, height = 200, unit = "px", noRStudioGD = TRUE)
	par(mfrow=c(1,2))

	barplot(thp,
		names.arg=traces,
		ylim=c(0,100),
		xlab="Trace",
		ylab="Hit Rate %",
		main=paste(conf, " Total Hit Rates"))

	barplot(amal,
		names.arg=traces,
		xlab="Trace",
		ylab="Average Memory Access Latency",
		main=paste(conf, " Average Memory Access Latency"))
}

#===============================
# 2way-wa.conf

conf <- "2way-wa"
thp <- c(98.76, 66.83, 75.24, 97.85, 99.66)
amal <- c(1.85, 23.89, 18.09, 2.48, 1.24)
make_plots(conf, thp, amal);

#===============================
# 2way-nwa.conf

conf <- "2way-nwa"
thp <- c(93.83, 66.71, 1.05, 92.61, 98.86)
amal <- c(5.26, 23.97, 69.27, 6.10, 1.79)
make_plots(conf, thp, amal);

#===============================
# 4way-fifo.conf

conf <- "4way-fifo"
thp <- c(98.72, 66.83, 75.24, 97.82, 99.65)
amal <- c(1.62, 17.26, 13.13, 2.07, 1.17)
make_plots(conf, thp, amal);

#===============================
# 4way-rand.conf

conf <- "4way-rand"
thp <- c(98.71, 66.83, 75.24, 97.79, 99.65)
amal <- c(1.63, 17.26, 13.13, 2.08, 1.17)
make_plots(conf, thp, amal);

#===============================
# small-dm.conf

conf <- "small-dm"
thp <- c(4.01, 17.32, 49.50, 5.11, 1.85)
amal <- c(93.85, 66.70, 1.02, 91.61, 98.27)
make_plots(conf, thp, amal);

#===============================
# medium-dm.conf

conf <- "medium-dm"
thp <- c(95.36, 66.70, 1.04, 92.92, 98.72)
amal <- c(3.27, 17.31, 49.49, 4.47, 1.63)
make_plots(conf, thp, amal);

#===============================
# large-dm.conf

conf <- "large-dm"
thp <- c(95.83, 66.71, 1.04, 93.43, 98.84)
amal <- c(3.04, 17.31, 49.49, 4.22, 1.57)
make_plots(conf, thp, amal);

#===============================
# mega.conf

conf <- "mega"
thp <- c(99.35, 66.85, 87.61, 98.86, 99.80)
amal <- c(1.65, 33.82, 13.26, 2.13, 1.20)
make_plots(conf, thp, amal);
