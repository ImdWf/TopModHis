# -*- coding: utf-8 -*-

from datetime import datetime
import sys

class Meta_POS_to_LDA(object):
  '''A class, that transforms a meta enriched output of an pos tagger into the LDA-C Format
  The input has to look like this:
  word_x,[Some Metadata]
  ...
  
  The output will be:
  [#UniqueWords] {Word_x_id: #Word_x}...
  
  The easiest way to use this class:
  obj_meptl = Meta_POS_to_LDA()
  erg = obj_meptl("Some Text File")'''
  
  def __init__(self):
    self.output = []
    self.raw_data = []
    self.list_sorted_data = []
    self.list_sorted_unique_articles = []
    self.list_sorted_articles = []
    self.int_number_articles = 0
    self.tuple_word_id = {}
  
  def get_raw_data(self, path_to_data,enc_file="utf8"):
    if "," in path_to_data:
      pathes = path_to_data.split(",")
      # import pdb; pdb.set_trace()
      for path in pathes:
        try:
          file = open(path, encoding=enc_file)
          self.raw_data += file.readlines()
          file.close()
        except:
          return 0
    else:
      try:
        file = open(path_to_data, encoding=enc_file)
        self.raw_data = file.readlines()
        file.close()
      except:
        return 0
    return 1

  def transform_raw_data_to_list_sorted_data(self):
    data = []
    if self.raw_data:
      for datapoint in self.raw_data: data.append(datapoint.split(',',1))
      # import pdb; pdb.set_trace()
      # self.list_sorted_data = sorted(data, key = lambda x: (int(x[1].split(',')[1]),x[1].split(',')[2],int(x[1].split(',')[3])))
      self.list_sorted_data = sorted(data, key = lambda x: (int(x[1].split(',')[0]),int(x[1].split(',')[1]),x[1].split(',')[2],int(x[1].split(',')[3])))
  
  def set_self_list_sorted_articles(self):
    if self.list_sorted_data:
      for element in self.list_sorted_data: self.list_sorted_articles.append(element[1])
      
  def set_self_list_sorted_unique_articles(self):
    if self.list_sorted_articles:
      # import pdb; pdb.set_trace()
      # self.list_sorted_unique_articles = sorted(set(self.list_sorted_articles), key = lambda x: (int(x.split(',')[1]),x.split(',')[2],int(x.split(',')[3])))
      self.list_sorted_unique_articles = sorted(set(self.list_sorted_articles), key = lambda x: (int(x.split(',')[0]),int(x.split(',')[1]),x.split(',')[2],int(x.split(',')[3])))
    
  def set_tuple_word_id(self):
    list_all_words = []
    if self.list_sorted_data:
      for data in self.list_sorted_data:
        list_all_words.append(data[0])
      list_sorted_unique_words = sorted(set(list_all_words))
      for i in range(0,len(list_sorted_unique_words)):
        self.tuple_word_id[list_sorted_unique_words[i]] = i

  def set_tuple_word_id_by_reading_vocab(self, path_to_vocabulary, update_vocabulary):
    print(u"Open Vocabulary file %s ..."  % path_to_vocabulary)
    try:
      file = open(path_to_vocabulary,"r")
    except:
      print(u"Konnte Datei %s nicht lesen, erstelle Word ID über die alternative Funktion\n" % path_to_vocabulary)
      self.set_tuple_word_id()
    else:
      list_all_words = []
      print(u"Reading from file ... ")
      for line in file.readlines():
        list_all_words.append(line.split(":")[1].rstrip())
      file.close()
      i = 0
      if self.list_sorted_data and update_vocabulary:
        print(u"Reading completed, now checking for new words\n")
        for data in self.list_sorted_data:
          if i % (len(self.list_sorted_data) // 10)  == 0 : print(u"Checking %s from %s \n" %(i, len(self.list_sorted_data)))
          if not data[0] in list_all_words:
            print(u"Appending new word &s \n" % data[0])
            list_all_words.append(data[0])
          i +=1
      for i in range(0,len(list_all_words)):
        self.tuple_word_id[list_all_words[i]] = i
      print(u"Reading completed\n")
        
  def replace_words(self):
    if self.tuple_word_id != {} and self.list_sorted_data:
      for i in range(0, len(self.list_sorted_data)):
        self.list_sorted_data[i][0] = self.tuple_word_id[self.list_sorted_data[i][0]]
    file = open('word_id.txt', 'w')
    for i in self.tuple_word_id:
      file.write(u"%s:%s\n" % (self.tuple_word_id[i], i))
    file.close()

  
  def set_self_int_number_articles(self):
    self.int_number_articles = len(self.list_sorted_unique_articles)
  
  def get_list_words_from_list_sorted_data_by_article_index(self, article_index):
    if self.list_sorted_data != [] and self.list_sorted_articles != [] and self.list_sorted_unique_articles != [] and self.int_number_articles >0:
      if article_index == self.int_number_articles-1: 
        return self.list_sorted_data[self.list_sorted_articles.index(self.list_sorted_unique_articles[article_index]):]
      else: 
        return self.list_sorted_data[self.list_sorted_articles.index(self.list_sorted_unique_articles[article_index]):self.list_sorted_articles.index(self.list_sorted_unique_articles[article_index+1])]
    else:
      print(u"Bin hier")
      return []
  
  
  def calculate_output_for_single_article(self, article_index):
    # if self.int_number_articles !=0 and article_index % (self.int_number_articles*0.1) == 0: print(article_index)
    if self.int_number_articles !=0 and article_index % (self.int_number_articles // 10)  == 0: print("%s/%s\n" % (article_index,self.int_number_articles))
    # if self.int_number_articles !=0 and article_index % (self.int_number_articles*0.01) == 0: print("%s/%s\n" % (article_index,self.int_number_articles))
    list_data = self.get_list_words_from_list_sorted_data_by_article_index(article_index)
    list_words = []
    str_word_sum_words = u""
    for data in list_data:
      list_words.append(data[0])
    int_unique_words = len(set(list_words))
    list_sorted_unique_words = sorted(set(list_words))
    for word in list_sorted_unique_words:
      i = 0
      while word in list_words:
        i+=1
        list_words.pop(list_words.index(word))
      if str_word_sum_words != "": str_word_sum_words += u" "
      str_word_sum_words += u"%s:%s" %(word,i)
    return u"%s %s" % (int_unique_words,str_word_sum_words)
    
  def __call__(self, path_to_file, path_to_vocabulary, update_vocabulary):
    t1 = datetime.now()
    print( "%s\n" % t1)
    self.get_raw_data(path_to_file)
    self.transform_raw_data_to_list_sorted_data()
    self.set_self_list_sorted_articles()
    self.set_self_list_sorted_unique_articles()
    if path_to_vocabulary:
      self.set_tuple_word_id_by_reading_vocab(path_to_vocabulary, update_vocabulary)
    else:
      self.set_tuple_word_id()
    self.replace_words()
    self.set_self_int_number_articles()
    str_erg = u""
    for i in range(0,self.int_number_articles):
      erg = self.calculate_output_for_single_article(i)
      if erg == "0 ": import pdb; pdb.set_trace()
      str_erg += u"%s\n" %erg
    file1 = open("Wortliste_LDA.txt","w")
    file1.write(str_erg)
    file1.close()
    file2 = open("Dokumente_sortiert.txt", "w")
    for article in self.list_sorted_unique_articles: file2.write(u"%s" %article)
    file2.close()
    t2 = datetime.now()
    print( "%s \n Diff: %s" % (t2, t2-t1))
    return 1
    
objPOS_to_LDA = Meta_POS_to_LDA()
if len(sys.argv) == 2:
  erg_text = objPOS_to_LDA(sys.argv[1],"", False)
elif len(sys.argv) == 3:
  erg_text = objPOS_to_LDA(sys.argv[1],sys.argv[2], False)
elif len(sys.argv) == 4:
  erg_text = objPOS_to_LDA(sys.argv[1],sys.argv[2], sys.argv[3])
else:
  erg_text = objPOS_to_LDA("1971-Nomen.csv,1973-Nomen.csv,1975-Nomen.csv,1978-Nomen.csv,1987-Nomen.csv", "", False)

#  erg_text = objPOS_to_LDA("1969-Nomen.csv")
# erg_test = test("1969-Nomen.csv,1970-Nomen.csv,1971-Nomen.csv,1972-Nomen.csv,1973-Nomen.csv,1974-Nomen.csv")
#erg_test = test("1969-Nomen.csv,1970-Nomen.csv,1971-Nomen.csv,1972-Nomen.csv,1973-Nomen.csv,1974-Nomen.csv,1975-Nomen.csv,1976-Nomen.csv,1977-Nomen.csv,1978-Nomen.csv,1979-Nomen.csv")
# erg_test = test("1969-Nomen.csv,1970-Nomen.csv,1971-Nomen.csv,1972-Nomen.csv,1973-Nomen.csv,1974-Nomen.csv,1975-Nomen.csv,1976-Nomen.csv,1977-Nomen.csv,1978-Nomen.csv,1979-Nomen.csv,1980-Nomen.csv,1981-Nomen.csv,1982-Nomen.csv,1983-Nomen.csv,1984-Nomen.csv,1985-Nomen.csv,1986-Nomen.csv,1987-Nomen.csv,1988-Nomen.csv,1989-Nomen.csv")
