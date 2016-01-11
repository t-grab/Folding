library(ggplot2)
source("analysis.R")

args <- commandArgs(TRUE)
print.evolution(args[1], args[2])