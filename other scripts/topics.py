#! /usr/bin/python

# usage: python topics.py <beta file> <vocab file> <num words>
#
# <beta file> is output from the lda-c code
# <vocab file> is a list of words, one per line
# <num words> is the number of words to print from each topic

import sys

def print_topics(beta_file, vocab_file, nwords = 25):
    
    # get the vocabulary
    file = open(vocab_file, 'r')
    vocab = file.readlines()
    # vocab = map(lambda x: x.split()[0], vocab)
    vocab = list(map(lambda x: x.strip(), vocab))
    # import pdb; pdb.set_trace()
    file.close()

    # for each line in the beta file

    indices = list(range(len(vocab)))
    topic_no = 0
    file = open(beta_file, 'r')
    topics = []
    for topic in file:
        # print('topic %03d' % topic_no)
        topic = list(map(float, topic.split()))
        # import pdb; pdb.set_trace()
        # indices.sort(lambda x,y: -cmp(topic[x], topic[y]))
        topics.append([])
        indices.sort(key = lambda x: topic[x], reverse = True)
        for i in range(nwords):
            topics[topic_no].append(vocab[indices[i]])
            # print('   %s' % vocab[indices[i]])
        topic_no += 1
        # print('\n')
    file.close()
    
    file = open(result_file, 'w')
    for tn in range(0,topic_no):
      topic_name = u"Topic %03d" % tn
      file.write(str(u"{0:50s}").format(topic_name))
    file.write("\n")
    for i in range(0,nwords):
      for tn in range(0,topic_no):
        file.write(str(u"{0:50s}").format(topics[tn][i]))
      file.write("\n")
    file.close()
    

if (__name__ == '__main__'):

    if (len(sys.argv) != 5):
       print('usage: python topics.py <beta-file> <vocab-file> <result_file> <num words>\n')
       sys.exit(1)

    beta_file = sys.argv[1]
    vocab_file = sys.argv[2]
    nwords = int(sys.argv[4])
    result_file = sys.argv[3]
    print_topics(beta_file, vocab_file, nwords)
