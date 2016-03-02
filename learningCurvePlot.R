pdf("learningCurve.pdf")
learning=read.table("trainErrorperEpoch.csv")
learningTs=ts(learning)
plot.ts(learningTs, xlab="Epochs", ylab="Training Error")

