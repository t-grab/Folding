library(ggplot2)

print.evolution <- function(log_file, png_file) {
    log <- read.csv(log_file, header=T)

    log <- cbind(
        Generation=log$Generation,
        rbind(
            cbind(data.frame(Type="Average"), data.frame(Fitness=log$Average)),
            cbind(data.frame(Type="Max"), data.frame(Fitness=log$Max))
        )
    )

    if (ncol(log) > 4) {
        diversity <- cbind(
            Generation=log$Generation,
            rbind(
                cbind(data.frame(Type="Mean"), data.frame(Diversity=log$Diversity)),
                cbind(data.frame(Type="Standard deviation", data.frame(Diversity=log[,ncol(log)])))
            )
        )
        
        png(paste0(png_file, "_diversity"))
        print(ggplot(data=diversity, aes(x=Generation, y=Diversity, colour=Type)) + geom_line())
        dev.off()
    }

    png(png_file)
    print(ggplot(data=log, aes(x=Generation, y=Fitness, colour=Type)) + geom_line())
    dev.off()   
}
