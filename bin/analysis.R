library(ggplot2)

print.evolution <- function(log_file, pdf_file) {
    log <- read.csv(log_file, header=T)

    log <- cbind(
        Generation=log$Generation,
        rbind(
            cbind(data.frame(Type="Average"), data.frame(Fitness=log$Average)),
            cbind(data.frame(Type="Max"), data.frame(Fitness=log$Max))
        )
    )

    pdf(pdf_file)
    print(ggplot(data=log, aes(x=Generation, y=Fitness, colour=Type)) + geom_line())
    dev.off()   
}
