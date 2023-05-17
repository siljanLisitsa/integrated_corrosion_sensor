
data <- read.csv('D:\\PuTTY\\demo0.csv') 
data

x <- data$Time * 0.001
y1 <- data$Temperature
y2 <- data$Humidity
y3 <- data$Resistance

x


df <- data.frame(x, y1, y2)
df


p <- ggplot(df, mapping = aes(x, y1))
p <- p + scale_color_manual(values = c("Temperature" = '#CC79A7','Humidity' = '#3A86ff'))
p <- p + geom_point(df, mapping = aes(y = y2), colour = "#3A86ff") 

p <- p + geom_point(df, mapping = aes(y = y1), col = "#CC79A7") 

p <- p + labs(
  title = "Temperature and Humidity over Time",
  x = "Time (s)", 
  y = NULL
)
p + guides(col = guide_legend(nrow = 3))
p





