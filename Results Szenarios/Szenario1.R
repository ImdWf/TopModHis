number_documents <- read.csv("E:/ProgrammeMasterarbeit/Skripte/Daten/Ergebnis_all-Vorkommen.txt", header=FALSE)

lhood <- NULL
pfad = sprintf("Result_Szenario1_1/final-lda-lhood.dat")
lhood <- read.table(pfad)
likelihoods = lhood/number_documents[,1]
print(sprintf("Result 6 Topics 1_1: %f",sum(likelihoods)/dim(lhood)[1]))

pfad = sprintf("Result_Szenario1_2/final-lda-lhood.dat")
lhood <- read.table(pfad)
likelihoods = lhood/number_documents[,1]
print(sprintf("Result 6 Topics 1_2: %f",sum(likelihoods)/dim(lhood)[1]))

pfad = sprintf("Result_Szenario1_2_11/final-lda-lhood.dat")
lhood <- read.table(pfad)
likelihoods = lhood/number_documents[,1]
print(sprintf("Result 11 Topics 1_2: %f",sum(likelihoods)/dim(lhood)[1]))


topics_size = c(7,11,13)
for(topic_c in topics_size)
{
pfad = sprintf("Result_Szenario1_1_%d/final-lda-lhood.dat",topic_c)
lhood <- read.table(pfad)
likelihoods = lhood/number_documents[,1]
print(sprintf("Result %d: %f",topic_c,sum(likelihoods)/dim(lhood)[1]))
}
# number_topics = 6
number_topics = 7
# number_topics = 11
# number_topics = 13
read.csv("TopicsinDocuments.txt", sep=" ", header=FALSE, col.names = paste0("V",seq_len(number_topics)), fill = TRUE) -> daten
read.csv("E:/ProgrammeMasterarbeit/Skripte/Daten/Dokumente_sortiert_all_unix.txt", header =FALSE) -> Dokumente
if (number_topics == 11)
{
topics <- read.table("../Topics11.txt", sep=".", encoding="UTF-8")
} else
{
topics <- read.table("../Topics.txt", sep=".", encoding="UTF-8")
}


summe_topics <- NULL
daten_topics <-NULL
documents_topics <-NULL
for(i in 1:dim(daten)[2])
{
   documents_topics[[i]] <- which(Dokumente[,3] %in% topics[i,1])
   daten_topics[[i]] = daten[documents_topics[[i]],]
     summe_x <- NULL
     for(x in 1:dim(daten)[2])
     {
     summe_j <- NULL
     for (j in 1:dim(daten)[2])
     {
        summe_j[j] <- length(which(daten_topics[[i]][,j] %in% (x-1)))
     }
     summe_x[[x]] = sum(summe_j)
     }
   summe_topics[[i]] <- summe_x
}

for (i in 1:dim(daten)[2])
{
  print(sprintf("%s: %d Documents total",topics[i,1],dim(daten_topics[[i]])[1]))
  print(sort(summe_topics[[i]]))
  print(sort(summe_topics[[i]]/dim(daten_topics[[i]])[1], index.return = TRUE))
}

die_zeit = which(Dokumente[,3] %in% "die zeit")
daten_die_zeit <- daten[die_zeit,]

summe <- NULL
for (i in 1:dim(daten)[2])
{
  summe_j <- 0
  for (j in 1:dim(daten)[2])
  {
    summe_j <- summe_j+length(which(daten_die_zeit[,j] %in% (i-1)))
  }
  summe[i] = summe_j
}
sort(summe)
sort(summe/dim(daten_die_zeit)[1], index.return = TRUE)


unzugeordnet = which(Dokumente[,3] %in% "unzugeordnet")
daten_unzugeordnet <- daten[unzugeordnet,]
summe2 <- NULL
for (i in 1:dim(daten)[2])
{
  summe_j <- 0
  for (j in 1:dim(daten)[2])
  {
    summe_j <- summe_j+length(which(daten_unzugeordnet[,j] %in% (i-1)))
  }
  summe2[i] = summe_j
}
sort(summe2)
sort(summe2/dim(daten_unzugeordnet)[1], index.return = TRUE)

summe <- NULL
for (i in 1:dim(daten)[2])
{
    summe[i] <- length(which(daten_die_zeit[,1] %in% (i-1)))
}
sort(summe)
sort(summe/dim(daten_die_zeit)[1], index.return = TRUE)

summe2 <- NULL
for (i in 1:dim(daten)[2])
{
    summe2[i] <- length(which(daten_unzugeordnet[,1] %in% (i-1)))
}
sort(summe2, index.return = TRUE)
