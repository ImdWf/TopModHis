read.csv("TopicLevelIndex.txt",header =FALSE, sep=" ", col.names = paste0("V",seq_len(25))) ->data
topic_six = NULL
for (i in 1:dim(data)[2])
{
  topic_six = append(topic_six,which(data[,i] == 6))
}
topic_six_sorted = sort(topic_six)

lots <- c(502,1484,1802,1995,2333)
data[topic_six_sorted[lots],]

unique_topics <- NULL
all_topics <- NULL
for(i in lots)
{
  puffer <- data[topic_six_sorted[i],]
  for(x in puffer)
  {
    all_topics <- append(all_topics,x)
  }
}

unique_topics = unique(all_topics)
sorted_unique_topics = sort(unique_topics)

parts <- read.csv("big_paket.txt", header = FALSE, sep=" ")
for (i in which(sorted_unique_topics == 79):length(sorted_unique_topics))
{
  filename <- sprintf("%d.txt", sorted_unique_topics[i])
  parts <- cbind(parts, read.csv(filename,header = FALSE, sep=" "))
}
order_topics <- c(6:65,79,93,94,103,167,274,383)

sum_all <-NULL
for(i in 1:dim(data[topic_six_sorted[lots],])[1])
{
  sum_single <- NULL
  for(j in 1:dim(data[topic_six_sorted[lots],])[2])
  {
    current_topic <- data[topic_six_sorted[lots],][i,j]
    
    if(!is.na(current_topic))
    {
    if (is.null(sum_single))
    {
      sum_single <- parts[which(order_topics == current_topic)]
    }else
    {
    sum_single <- sum_single+ parts[which(order_topics == current_topic)]
    }
    }
  }
  
  sum_all[i] <- sum_single
}
