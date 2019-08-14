#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <stdio.h>

#define MAXLEVELSIZE 4;


//Independent Typedefs
typedef std::vector<std::string> vocab;
typedef std::vector<size_t> vecIndex;
typedef std::map<size_t,size_t> mapIndex;
typedef std::set<size_t> setIndex;
typedef std::vector<vecIndex> listTopicsinDocuments;
typedef std::pair<size_t, size_t> pairIndexNumber;
typedef std::vector<pairIndexNumber> vecTopicsinDocuments ;
typedef std::vector<pairIndexNumber> vecPairIndexNumber;
typedef std::vector<vecTopicsinDocuments> vecDocStates;

struct Dates {
	size_t year = 0;
	size_t month = 0;
	size_t day = 0;

	bool const operator==(const Dates& o) const {
		return year == o.year && month == o.month && day == o.day;
	}

	bool const operator<(const Dates& o) const {
		return year < o.year || (year == o.year && month < o.month) || (year == o.year && month == o.month && day < o.day);
	}
	friend std::ostream& operator<<(std::ostream& os, const Dates& dt)
	{
		os.fill('0');
		os.width(2);
		os << dt.day;
		os << '.';
		os.width(2);
		os << dt.month;
		os << '.';
		os << dt.year;
		return os;
	}

};
typedef std::vector<Dates> vecDates;

template <class T>
class Model_editor
{
public:
	//Typedefinitions dependent on the type T	
	typedef std::pair<size_t, T> pairIndexFileValue;
	typedef std::vector<pairIndexFileValue> Indexmap;
	typedef std::vector<T> vecT;
	//typedef std::pair<size_t, T> pairIndexNumber;
	
	
private:
	std::vector<vecT> Model;

public:
	//Auxiliaries
	T MinValue = 500;
	T MaxValue = 0;
	short date_format = 0; // 0: No format defined, 1: KW (Year,KW), 7: KW(KW,Year), 2: Day of the year, 3: American, 4: European, 5: Just Year and Month, 6X: Just Year, the X for to mark the position of the year

	//Common functions
	const size_t getSize();
	const size_t getSizeVocab();
	short open_docstates(vecDocStates& Docstates, std::string filepath = "");
	short open_vocabulary(vocab&, std::string);
	void DebugMode();

	//Debug
	std::set<size_t> DebugFillListTopicsinDocuments(listTopicsinDocuments&, const size_t&);
	void DebugFillModel(const size_t&);
	void DebugFillModelwithDocuments(const size_t&);

	//Topfunctions
	short readModel(std::string,bool); //Start and Option 2
	short deleteTopic(size_t); //Option 1
	short appendTopic(std::string, size_t, bool, bool); //Option 3
	short saveTopic(std::string, size_t, size_t);// Option 4
	short showValue(size_t, size_t, size_t); //Option 5
	short editTopic(size_t, size_t); //Option 6
	void printTopics(size_t, const std::string, std::string); //Option 7
	void printHDP(std::string filepath_model, std::string filepath_doc_states = "-", std::string filepath_doc_id = "-", size_t topic = std::numeric_limits<size_t>::max()); //Option 8 HDP
	void printDates(std::string filepath_doc_states, std::string filepath_doc_id); //Option 9
	short saveModel(std::string, std::string); //Option 10

	//Auxiliary functions print Topics
		void sort_Topic_data(Indexmap&); //printTopics
		void to_Indexmap(Indexmap&,const size_t&,const size_t&); //printTopics
	
	//Auxiliary functios printHDP
		void sort_Topic_data2(vecPairIndexNumber&); //printHDP
		vecPairIndexNumber sum_Topic_Doc_States(vecDocStates&); //printHDP
		void DocStates_to_ListTopics(listTopicsinDocuments&, const vecDocStates&, const vecPairIndexNumber&); //printHDP
		short printTopicsinDocument(const listTopicsinDocuments&, std::string); //printHDP
		unsigned short getUniqueTopics(std::set<vecIndex>&, const listTopicsinDocuments&); //printHDP
		short printTopicLevelIndex(const std::string&, const std::set<vecIndex>&, const size_t&);
		//short printAncestors(const std::string&, const std::vector<mapIndex>&,const std::vector<vecIndex>&);
		//short printAncestors(const std::string&, const std::vector<mapIndex>&);
		void createHierarchy(std::vector<vecIndex>&, const std::vector<vecIndex>&, const size_t&); //printHDP
		short printHierarchy(std::string, const std::set<vecIndex> &, const size_t&); //printHDP
		void print_Topic_in_Dates(std::string, std::string, const listTopicsinDocuments); //printHDP Add-on
			void Doc_Names_to_Date(vecDates&, const vocab&); //printHDP Add-on
			void setVecDate(vecDates&,const std::vector<size_t*>&);
			void getDateFormat(const std::vector<size_t*>&);
			void getUniqueDates(std::set<Dates>&, const vecDates&);
		
};



template <class T> short Model_editor<T>::open_docstates(vecDocStates& Docstates, std::string filepath)
{
	if (filepath == "-" || (filepath.find("doc.states") == std::string::npos && filepath.find("gamma.dat") == std::string::npos))
	{
		std::cout << "Please type in the filepath to the docstates File\n";
		std::cout << "Filepath: ";
		std::cin >> filepath;
	}
	std::ifstream fs;
	std::string puffer;
	fs.open(filepath);
	if (fs.fail())
	{
		return -1;
	}
	else
	{
		// iStrStream;
		while (std::getline(fs, puffer))
		{
			size_t counter, data;
			float data2;
			data = 0;
			data2 = 0;
			counter = 0;
			vecTopicsinDocuments vecTopinDocs;
			std::istringstream iStrStream;
			iStrStream.str(puffer);
			while (iStrStream)
			{
				if(MaxValue >0)
					iStrStream >> data;
				else
				{
					iStrStream >> data2;
					data = (size_t)data2;
				}
				vecTopinDocs.push_back(pairIndexNumber(counter, data));
				counter++;
			}
			//pop last element, as it goes double into the vector
			vecTopinDocs.pop_back();
			Docstates.push_back(vecTopinDocs);
		}
		return 1;
	}
}

template <class T> short Model_editor<T>::open_vocabulary(vocab& Vocabulary, std::string filepath)
{
	std::ifstream file;
	
	file.open(filepath.c_str());
	size_t id = 0;
	std::string data = "";
	if (file.fail())
		return -1;
	else
	{
		// vocab_data = new vec_vocab;
		data.resize(0);
		while (std::getline(file, data))
		{
			//sprintf(data,"%s",buffer);
			Vocabulary.push_back(data);
			data = "";
		}
		if (Vocabulary.size() == 0)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}

}


template <class T> short Model_editor<T>::readModel(std::string filepath, bool MinMax)
{
	std::fstream fs;
	fs.open(filepath, std::ios::in);
	if (fs.fail()) return -1;
	//fs >> *(Model + (i++)) >> std::endl;
	T i(0);
	std::string puffer;
	while (std::getline(fs, puffer))
	{
		std::istringstream istr;
		std::vector<T> vecRow;
		istr.str(puffer);
		while (istr)
		{
			try
			{
				istr >> i;
				if (MinMax)
				{
					/*if (i < 0)
					{
						if (MinValue > 0) MinValue = i;
						else if (i > MinValue)	MinValue = i;
						if (MaxValue == 0) MaxValue = i;
						else if (i < MaxValue) MaxValue = i;
						
					}*/
					if (i < MinValue) MinValue = i;
					else if (MaxValue < i) MaxValue = i;
				}
				vecRow.push_back(i);
			}
			catch (const std::exception&)
			{
				std::cout << "Datei enthaelt nicht lesbare Werte. Das Programm wird beendet\n";
				return 0;
			}
		}
		vecRow.pop_back();
		if (getSize() == 0) Model.push_back(vecRow);
		else
		{
			int intDifSize = getSizeVocab() - vecRow.size();
			if (intDifSize < 0)
			{
				vecRow.resize(getSizeVocab());
			}
			else
			{
				while (intDifSize > 0)
				{
					vecRow.push_back(MinValue);
					intDifSize--;
				}
			}
			Model.push_back(vecRow);
		}
		puffer = "";
	}
	std::cout << "read it\n";
	return 1;
}
template <class T> short Model_editor<T>::saveModel(std::string filepath, std::string new_name)
{
	std::string directory;
	if (filepath.find("/") != std::string::npos)
	{
		directory = filepath.substr(0, filepath.rfind("/") + 1);
	}
	else
		directory = "";
	std::ofstream fs;
	if (new_name != "")
	{
		if (MaxValue < 0)
			fs.open(directory + new_name + ".beta");
		else
			fs.open(directory + new_name + ".topics");
	}
	else
		fs.open(filepath);
	if (fs.fail()) return -1;
	size_t i(0);
	//std::vector<T>::iterator itTopic;
	typename vecT::iterator itTopic;
  //itVecT itTopic;
	for (i=0;i<getSize();i++)
	{
		fs << Model[i][0];
		for (itTopic = Model[i].begin()+1; itTopic != Model[i].end(); itTopic++)
		{
			fs << " ";
			fs << *itTopic ;
		}
		fs << std::endl;
	}
	if (fs.is_open()) fs.close();
	return 1;
}
template <class T> short Model_editor<T>::appendTopic(std::string filepath, size_t topic_number, bool append, bool multi)
{
	vecT vecModifiedTopic;
	std::ifstream fs;
	fs.open(filepath);
	if (fs.fail()) return -1;
	T i(0);
	size_t number_topics = 1;
	std::string puffer;
	std::vector<vecT> vecMultipleTopics;
	while (std::getline(fs, puffer))
	{
		
		if (multi)
		{
			number_topics = 1;
			std::istringstream istr;
			istr.str(puffer);
			while (istr)
			{
				istr >> i;
				if (vecMultipleTopics.size() < number_topics)
				{
					vecMultipleTopics.push_back({ i });
				}
				else
				{
					vecMultipleTopics[number_topics - 1].push_back(i);
				}
				number_topics++;
			}
			number_topics--;
			vecMultipleTopics.pop_back();
			number_topics--;

		}
		else
		{
			std::istringstream istr;
			istr.str(puffer);
			istr >> i;
			vecModifiedTopic.push_back(i);
		}
	}
	size_t counter = 0;
	if (getSize() < number_topics)
	{
		while (counter < number_topics)
		{
			if (multi)
			{
				vecModifiedTopic = vecMultipleTopics[counter];
			}
			Model.push_back(vecModifiedTopic);
			counter++;
		}
	}
	else
	{
		while (counter < number_topics)
		{
			if (multi)
			{
				vecModifiedTopic = vecMultipleTopics[counter];
			}
			int intDifSize = getSizeVocab() - vecModifiedTopic.size();
			if (intDifSize < 0)
			{
				vecModifiedTopic.resize(getSizeVocab());
			}
			else
			{
				while (intDifSize > 0)
				{
					vecModifiedTopic.push_back(MinValue);
					intDifSize--;
				}
			}
			if (append) Model.insert((Model.begin() + topic_number), 1, vecModifiedTopic);
			else *(Model.begin() + topic_number) = vecModifiedTopic;
			counter++;
		}
	}
	return 1;
}
template <class T> short Model_editor<T>::saveTopic(std::string filepath, size_t topic_number_start, size_t topic_number_end)
{
	size_t topic;
	std::ofstream fs;
	fs.open(filepath);
	if (fs.fail()) return -1;
	for (size_t word = 0; word < getSizeVocab(); word++)
	{
		for (topic = topic_number_start; topic <= topic_number_end; topic++)
		{
			//vecT vectopic = Model[topic];
			/*std::vector<T>::iterator itTopic;
			for (itTopic = topic.begin(); itTopic != topic.end(); itTopic++)
			{
				fs << *itTopic << std::endl;
			}*/
			fs << Model[topic][word];
			if ((topic + 1) <= topic_number_end)
			{
				fs << " ";
			}
			else
			{
				fs << std::endl;
			}
		}
	}
	if (fs.is_open()) fs.close();
	return 1;
}

template <class T> short Model_editor<T>::deleteTopic(size_t TopicNumber)
{
	Model.erase(Model.begin() + TopicNumber);
	return 1;
}
template <class T> short Model_editor<T>::editTopic(size_t TopicNumber, size_t Entry)
{
	T New_Value;
	std::cout << "New Value: ";
	std::cin >> New_Value;
	Model[TopicNumber][Entry] = New_Value;
	return 1;
}
template <class T> const size_t Model_editor<T>::getSize()
{
	return Model.size();
}
template <class T> const  size_t Model_editor<T>::getSizeVocab()
{
	return Model[0].size();
}
template <class T> short Model_editor<T>::showValue(size_t Topic, size_t begin, size_t end)
{
	typename std::vector<T>::iterator itVec = Model[Topic].begin();
	std::cout << std::endl;
	for (size_t i = begin; i <= end; i++)
	{
		std::cout << i << ": " << *(itVec + i) << std::endl;
	}
	std::cout << std::endl;
	return 1;
}

template <class T> void Model_editor<T>::to_Indexmap(Indexmap& result,const size_t& topic,const size_t& NumberTopWords)
{	
	size_t Index = 0;
	typename std::vector<T>::iterator itTopic;
	Indexmap Indexmap_temp;
	for (itTopic = Model[topic].begin(); itTopic != Model[topic].end(); itTopic++)
	{
		Indexmap_temp.push_back(pairIndexFileValue(Index, *itTopic));
		Index++;
	}
	sort_Topic_data(Indexmap_temp);
	Indexmap_temp.resize(NumberTopWords);
	result = Indexmap_temp;
}

template <class T> void Model_editor<T>::sort_Topic_data(Indexmap& Index)
{
	
	//if (MaxValue < 0)
	//{
	std::sort(Index.begin(), Index.end(), [](pairIndexFileValue &a, pairIndexFileValue &b) {
			return a.second > b.second; });
	//}
	/*else
	{
		std::sort(Index.begin(), Index.end(), [](pairIndexFileValue &a, pairIndexFileValue &b) {
			return a.second < b.second; });
	}*/
}

template <class T> void Model_editor<T>::sort_Topic_data2(vecPairIndexNumber& Index)
{
	
	//if (MaxValue > 0)
	//{
	std::sort(Index.begin(), Index.end(), [](pairIndexNumber& a, pairIndexNumber& b) {
			return a.second > b.second; });
	/*}
	else
	{
		std::sort(Index.begin(), Index.end(), [](pairIndexNumber& a, pairIndexNumber& b) {
			return a.second < b.second; });
	}*/
}

template <class T> void Model_editor<T>::printTopics(size_t number_topwords,const std::string filepath_Model,std::string filepath_vocab)
{
	std::string filename;
	std::vector<Indexmap> vctIndexmap;
	size_t topicindex = 0;
	for (topicindex = 0; topicindex < getSize(); topicindex++)
	{
		Indexmap Indexmap_;

		to_Indexmap(Indexmap_, topicindex, number_topwords);
		//sort_Topic_data(Indexmap_);
		vctIndexmap.push_back(Indexmap_);
	}
	if (filepath_vocab == "-")
	{
		std::cout << "Please type in the path to the vocabulary file:\n";
		std::cin >> filepath_vocab;
	}
	std::string directory = "";
	if (filepath_Model.rfind("/") != std::string::npos)
	{
		directory = filepath_Model.substr(0, filepath_Model.rfind("/")+1);
	}
	vocab vocab_data;
	if (open_vocabulary(vocab_data, filepath_vocab) > 0)
	{
		filename = directory + "final_cluster.txt";
		std::ofstream file;
		file.open(filename);
		if (file.fail())
		{
			std::cout << "Couldn't open the topic file for writing. Abort";
		}
		else
		{
			size_t topic = 0;
			size_t levels = 0;
			file.fill(' ');
			for (levels = 0; levels <= number_topwords; levels++)
			{
				for (topic = 0; topic < getSize(); topic++)
				{
					//if (Model[topic].size() > 1 || Model[topic].size() >= number_topwords) This doesn't make sense
					if (Model[topic].size() > 1)
					{
						if (levels == 0)
						{
							//Print headers
							size_t number = topic;
							unsigned short digits = 0;
							while (number) {
								number /= 10;
								digits++;
							}
							if (topic == 0)
								file.width(49 - digits);
							else
								file.width(50 - digits);
							file << "Topic_" << topic;
						}
						else
						{
							//Print terms
							if (levels < Model[topic].size())
							{
								typename std::vector<std::vector<pairIndexFileValue>>::iterator itVctIndexmap;
								itVctIndexmap = vctIndexmap.begin();
								typename std::vector<pairIndexFileValue>::iterator itIndexmap;
								Indexmap Indexmap_ = *(itVctIndexmap + topic);
								itIndexmap = Indexmap_.begin();
								pairIndexFileValue pairIndex;
								for (unsigned short i = 0; i < levels - 1; i++)
								{
									itIndexmap++;
								}
								pairIndex = *(itIndexmap);
								file.width(50);
								if (pairIndex.second != MinValue && pairIndex.first <= vocab_data.size())
									file << vocab_data[pairIndex.first];
								else
									file << " ";
							}
						}
					}
				}
				file << std::endl;
			}
		}
	}
	else
		std::cout << "Wrong Path to Vocabulary, no Data found";
}


template <class T> vecPairIndexNumber Model_editor<T>::sum_Topic_Doc_States(vecDocStates& VecDocStates_)
{
	vecPairIndexNumber vecResults_;
	size_t counter = 0;
	size_t data = 0;
	vecDocStates::iterator itDocuments;
	//First level is document level, second level are the topics
	//We are iterating through all documents and increase the counter for each topic
	for (itDocuments = VecDocStates_.begin(); itDocuments != VecDocStates_.end(); itDocuments++)
	{
		//vecTopics_ = *itDocuments;
		counter = 0;
		vecPairIndexNumber::iterator itTopics_;
		for (itTopics_ = itDocuments->begin(); itTopics_ != itDocuments->end(); itTopics_++)
		{
			if (vecResults_.size() < getSize())
			{
				vecResults_.push_back(pairIndexNumber(itTopics_->first,itTopics_->second));
			}
			else
			{
				data = vecResults_[counter].second;
				vecResults_[counter].second = data + itTopics_->second;
			}
			counter++;
		}
	}
	sort_Topic_data2(vecResults_);
	return vecResults_;
}

template <class T> void Model_editor<T>::DocStates_to_ListTopics(listTopicsinDocuments& Result_, const vecDocStates& DocStates_, const vecPairIndexNumber& SumTopics_)
{
	vecIndex vecIndexTopicinDoc;
	size_t DocIndex = 0;
	//vecDocStates<T>::iterator itDocuments;
	for (vecDocStates::const_iterator itDocuments = DocStates_.begin();itDocuments != DocStates_.end();++itDocuments)
	{
		vecPairIndexNumber::const_iterator itSumTopics;
		for (itSumTopics = SumTopics_.begin();itSumTopics != SumTopics_.end();itSumTopics++)
		{
			if ((*itDocuments)[itSumTopics->first].second >0)
			{
				if (Result_.size() <= DocIndex)
				{
					vecIndex puffer;
					puffer.push_back(itSumTopics->first);
					Result_.push_back(puffer);
				}
				else
					Result_[DocIndex].push_back(itSumTopics->first);
			}
		}
		DocIndex++;
	}
}

template <class T> short Model_editor<T>::printTopicsinDocument(const listTopicsinDocuments& Topics_in_Document_, std::string directory_)
{
	std::ofstream file;
	if (directory_.find("/") != std::string::npos)
	{
		directory_ = directory_.substr(0, directory_.rfind("/") + 1);
	}
	else
		directory_ = "";
	file.open(directory_ + "TopicsinDocuments.txt");
	if (file.fail())
	{
		return -1;
	}
	else
	{
		size_t IndexDocument = 0;
		for (listTopicsinDocuments::const_iterator itTopicsinDocument = Topics_in_Document_.begin(); itTopicsinDocument != Topics_in_Document_.end(); itTopicsinDocument++)
		{
			//file << IndexDocument << ":";
			file << itTopicsinDocument->at(0);
			for (vecIndex::const_iterator itIndex = itTopicsinDocument->begin()+1; itIndex != itTopicsinDocument->end(); itIndex++)
			{
				file << " " << *itIndex;
			}
			file << std::endl;
			IndexDocument++;
		}
		return 0;
	}
}

template <class T> unsigned short Model_editor<T>::getUniqueTopics(std::set<vecIndex>& setUniqueTopics, const listTopicsinDocuments& listTopinDoc_)
{
	size_t sum_doc_level = 1, round_ =0;
	unsigned short document_level = 0;
	unsigned short highest_written_level = 0;
	unsigned short number_topics = 0;

	listTopicsinDocuments::const_iterator itListTopinDocs; //iterator that goes through the documents

	//Look inside the Document
	for (itListTopinDocs = listTopinDoc_.begin(); itListTopinDocs != listTopinDoc_.end(); itListTopinDocs++)
	{
		vecIndex TopicLevelIndex_;
		for (document_level = 0; document_level < itListTopinDocs->size(); document_level++)
		{
			TopicLevelIndex_.push_back((*itListTopinDocs)[document_level]);
		}
		if (TopicLevelIndex_.size() > highest_written_level)
			highest_written_level = TopicLevelIndex_.size();
		setUniqueTopics.insert(TopicLevelIndex_);

	}
	
	return highest_written_level-1; //-1 as size() counts from one
}


//template <class T> short Model_editor<T>::printAncestors(const std::string& filepath_model, const std::vector<mapIndex>& Ancestors_, const std::vector<vecIndex>& Hierarchie_)
/*template <class T> short Model_editor<T>::printAncestors(const std::string& filepath_model, const std::vector<mapIndex>& Ancestors_)
{
	std::ofstream file;
	size_t best_topic, max_value_l = 0;
	std::string directory_ = "";
	if (filepath_model.find("/") != std::string::npos)
	{
		directory_ = filepath_model.substr(0, filepath_model.rfind("/") + 1);
	}
	else
		directory_ = "";
	file.open(directory_ + "Ancestors.txt");
	if (file.fail())
	{
		return -1;
	}
	else
	{
		
		for (size_t topic = 0; topic < getSize(); topic++)
		{
			max_value_l = 0;
			for (mapIndex::const_iterator itAncestors = Ancestors_[topic].begin(); itAncestors != Ancestors_[topic].end(); itAncestors++)
			{
				if ((*itAncestors).second > max_value_l)
				{
					max_value_l = (*itAncestors).second;
					best_topic = (*itAncestors).first;
				}
				file << (*itAncestors).first << ":" << (*itAncestors).second << " ";
			}

			if (topic + 1 != getSize())
			{
				if (max_value_l)
					file << "Highest Probability: "<<best_topic << ":" << max_value_l << std::endl;
				else
					file << std::endl;
			}

		}
	
	}
}*/

template <class T> short Model_editor<T>::printTopicLevelIndex(const std::string& filepath_model, const std::set<vecIndex>& UniqueTopics_, const size_t& levels)
{
	std::ofstream file;
	std::string directory_ = "";
	if (filepath_model.find("/") != std::string::npos)
	{
		directory_ = filepath_model.substr(0, filepath_model.rfind("/") + 1);
	}
	else
		directory_ = "";
	file.open(directory_ + "TopicLevelIndex.txt");
	if (file.fail())
	{
		return -1;
	}
	else
	{
		typename std::set<vecIndex>::iterator itTopics;
    for (size_t level = 1; level <= levels; level++)
		{
			//file << "Level " << level << ":" << std::endl << std::endl;
			for (itTopics = UniqueTopics_.begin(); itTopics != UniqueTopics_.end(); itTopics++)
			{
				if (itTopics->size() == level)
				{
					for (typename vecIndex::const_iterator itVecs = itTopics->begin(); itVecs != itTopics->end();)
					{
						file << *itVecs;
						itVecs++;
						if (itVecs != itTopics->end())
						{
							file << " ";
						}
						else
						{
							file << std::endl;
						}
						
					}
					//file << std::endl;
				}
			}
			//file << std::endl;
		}
	}
	return 0;
}

template <class T> void Model_editor<T>::createHierarchy(std::vector<vecIndex>& Hierarchy_, const std::vector<vecIndex>& TopicLevelIndex_,const size_t& Levels_)
{
	size_t value = 0;
	size_t topic = 0;
	unsigned short level = 0;
	unsigned short final_level = 0;
	for (topic = 0; topic < getSize(); topic++)
	{
		value = 0;
		final_level = 0;
		for (level = 0; level <= Levels_; level++)
		{
			if (TopicLevelIndex_[level][topic] > value)
			{
				final_level = level;
				value = TopicLevelIndex_[level][topic];
				break; //We are looking for the first level, where the topic appears

			}
		}
		if (value)
		{
			if (Hierarchy_.size() <= final_level)
			{
				while (Hierarchy_.size() <= final_level)
				{
					//vecIndex vecTopic;
					Hierarchy_.push_back({});
				}
			}
			Hierarchy_[final_level].push_back(topic);
		}
	}
}

template <class T> short Model_editor<T>::printHierarchy(std::string directory_,const std::set<vecIndex>& UniqueTopics,const size_t& topic_of_interest_)
{
	size_t max_levels = MAXLEVELSIZE;
	std::cout << "Creating new hierarchie." << std::endl << "The maximum number of new topics would be " << UniqueTopics.size() << " Topics" << std::endl;
	std::cout << "We are printing all combined topics for topic number " << topic_of_interest_ << std::endl;
	std::ofstream file;
	if (directory_.find("/") != std::string::npos)
	{
		directory_ = directory_.substr(0, directory_.rfind("/")+1);
	}
	else
		directory_ = "";
	file.open(directory_ + std::to_string(topic_of_interest_) +"_final-test.topics");
	if (file.fail())
	{
		return -1;
	}
	else
	{
		//for (size_t level = 1; level <= max_levels; level++)
		//{
			std::set<vecIndex>::const_iterator itTopics;
			for (itTopics = UniqueTopics.begin(); itTopics != UniqueTopics.end(); itTopics++)
			{
				//if (itTopics->size() == level)
				//{
				bool print = false;
				size_t counter = 0;
				for (size_t topic = 0; topic < itTopics->size(); topic++)
				{
					if ((*itTopics)[topic] == topic_of_interest_)
						print = true;
				}
				if (print)
				{
					size_t topic = 0;
					for (size_t word = 0; word < getSizeVocab(); word++)
					{
						T entry = 0;
						topic = 0;
						for (; topic < itTopics->size(); topic++)
						{
							entry += Model[(*itTopics)[topic]][word];
						}
						if (word == 0)
							file << entry;
						else
						{
							file << " " << entry;
						}
						/*if (word == (getSizeVocab() - 1))
							file << std::endl;*/
					}
					if ((topic+1) != itTopics->size())
						file << std::endl;
					counter++;
					if (counter % 100 == 0)
					{
						std::cout << "Topic " << counter << std::endl;
					}
				}
			}
				
	}
		//}
	return 0;
}


template <class T> void Model_editor<T>::getDateFormat(const std::vector<size_t*>& vecDate_ )
{
	//Dates l_Date;
	//size_t l_max_value = 0;
	size_t l_max_value[3] = { 0 };
	//size_t max_value_all = 0, min_value_all = 0;
	unsigned short number_max = 4, number_min = 4, number_middle =4;
	unsigned short counter = 0;
	//bool year = false, month = false, day = false;
	if (!date_format)
	{
		for (std::vector<size_t*>::const_iterator itDates = vecDate_.begin(); itDates != vecDate_.end(); itDates++)
		{
			size_t* ptrDate_ = *itDates;
			size_t Date_[3];
			Date_[0] = *(ptrDate_);
			Date_[1] = *(ptrDate_ + 1);
			Date_[2] = *(ptrDate_ + 2);
			for (counter = 0; counter < 3; counter++)
			{
				if (l_max_value[counter] < Date_[counter])
					l_max_value[counter] = Date_[counter];
			}
			/*if (!l_max_value[0] && !l_max_value[1] && !l_max_value[2])
			{
				rerun = false;
			}
			else*/
			if (l_max_value[0] > l_max_value[1])
			{
				if (l_max_value[0] > l_max_value[2])
					number_max = 0;
				else
					number_max = 2;
				if (l_max_value[1] > l_max_value[2])
					number_min = 2;
				else
					number_min = 1;
			}
			else
			{
				if (l_max_value[1] > l_max_value[2])
					number_max = 1;
				else
					number_max = 2;
				if (l_max_value[0] > l_max_value[2])
					number_min = 2;
				else
					number_min = 0;
			}
			if (number_min == 0 || number_max == 0)
			{
				if (number_max == 2 || number_min == 2)
				{
					number_middle = 1;
				}
				else
					number_middle = 2;
			}
			else
				number_middle = 0;
		}
		if (l_max_value[number_max] > l_max_value[number_middle]) //We have a value
		{
			if (l_max_value[number_middle] > 12 && l_max_value[number_middle] < 32) //We maybe have a day, a month and a year?
			{
				if (l_max_value[number_min] <= 12 && l_max_value[number_min] > 1 && number_min == 1) //Yes
				{
					date_format = 4; //European
				}
				else if (l_max_value[number_min] <= 12 && l_max_value[number_min] > 1 && number_min == 0)//Yes
				{
					date_format = 3; //American
				}

			}
			else if (l_max_value[number_middle] > 0 && l_max_value[number_min] == 0 && l_max_value[number_middle] < 367) //We have a second number, we don't know whether just months or day of year or calendary
			{
				if (l_max_value[number_middle] > 53) //value higher than possible calendary weeks in a year, therefor it should be a day of the year format
				{
					date_format = 2;
				}
				else if (l_max_value[number_middle] > 12) //Value higher than possible months in a year, therefor it should be a calendary week
				{
					if (number_middle < number_max)
						date_format = 1;
					else
						date_format = 7;
				}
				else
				{
					date_format = 5;
				}
			}
			else //use the found value as a year only
			{
				if (number_max == 0)
					date_format = 60;
				if (number_max == 1)
					date_format = 61;
				if (number_max == 2)
					date_format = 62;
			}
				
		}
		

	}
}

bool schaltjahr(size_t intJahr)
{
	if ((intJahr % 400) == 0)
		return true;
	else if ((intJahr % 100) == 0)
		return false;
	else if ((intJahr % 4) == 0)
		return true;

	return false;
}

unsigned short getTageimMonat(unsigned short usMonat, int intJahr)
{
	unsigned short Tage[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (schaltjahr(intJahr))
	{
		Tage[1] = 29;
	}
	return Tage[usMonat];
}

size_t* TagJahrtoTagMonat(unsigned short usTag, size_t intJahr)
{
	unsigned short lokalTag = usTag;
	unsigned short i = 0;
	while (lokalTag > getTageimMonat(i, intJahr))
	{
		lokalTag -= getTageimMonat(i, intJahr);
		i++;
	}
	size_t* ergebnis = new size_t[2];
	ergebnis[0] = lokalTag;
	ergebnis[1] = i + 1;
	return ergebnis;
}
unsigned short getWochentag(unsigned short usTag, unsigned short usMonat, unsigned short usJahr)
{
	// "Woche" ist dabei definiert als [Mo, ..., So].
	//Berechne die Verschiebung an Wochentagen pro Monat
	unsigned short arrusMonatsOffset[12] = { 0,3,3,6,1,4,6,2,5,0,3,5 };

	unsigned short usTagesziffer = (usTag % 7);
	unsigned short usMonatsziffer = arrusMonatsOffset[usMonat - 1];
	unsigned short usJahresziffer = ((usJahr % 100) + ((usJahr % 100) / 4)) % 7;
	unsigned short usJahrhundertziffer = (3 - ((usJahr / 100) % 4)) * 2;

	if ((usMonat <= 2) && (schaltjahr(usJahr)))
		usTagesziffer += 6;

	unsigned short ergebnis = (usTagesziffer + usMonatsziffer + usJahresziffer + usJahrhundertziffer) % 7;
	return ergebnis;
	//return (usTagesziffer + usMonatsziffer + usJahresziffer + usJahrhundertziffer) % 7;
}

void kw_to_date(size_t* rawDate)
{
	size_t* stJahr = (rawDate+1);
	size_t usKW = *rawDate;
	//returns the monday of a week
	unsigned short usTagJahr = 0;
	unsigned short uslokalWochentag = 0;
	unsigned short usErsterJanuar = 0;

	usErsterJanuar = getWochentag(1, 1, *stJahr);

	size_t* data = NULL;
	// Der erste Donnerstag im neuen Jahr liegt immer in der KW 1.
	if (usErsterJanuar > 4)
	{
		usTagJahr = 7 * usKW - 5 - usErsterJanuar + 7;
	}
	else if (usErsterJanuar > 2 && usKW == 1)
	{
		if (schaltjahr(*stJahr - 1))
		{
			usTagJahr = 366 - usErsterJanuar + 2;
		}
		else
		{
			usTagJahr = 365 - usErsterJanuar + 2;
		}
		*stJahr -= 1;
	}
	else
	{
		usTagJahr = 7 * usKW - 5 - usErsterJanuar;
	}
	data = TagJahrtoTagMonat(usTagJahr, *stJahr);
	//intTag = new int;
	//intMonat = new int;
	*(rawDate + 2) = *stJahr;
	*(rawDate) = *(data + 0);
	*(rawDate+1) = *(data + 1);
	//http://www.a-m-i.de/tips/datetime/datetime.php
}

template <class T> void Model_editor<T>::setVecDate(vecDates& VecDate_,const std::vector<size_t*>& Date_)
{
	std::vector<size_t*>::const_iterator itDatesraw;
	for (itDatesraw = Date_.begin(); itDatesraw != Date_.end(); itDatesraw++)
	{
		Dates l_Date;
		size_t* ptrDate;
		ptrDate = *itDatesraw;
		if (date_format == 1 || date_format ==7)
		{
			if (date_format == 7)
			{
				size_t* ptrDate2 = new size_t[3]{ 0 };
				*(ptrDate2 + 1) = *ptrDate;
				*(ptrDate2) = *(ptrDate + 1);
				ptrDate = ptrDate2;
			}
			kw_to_date(ptrDate);
		}
		else if (date_format == 2)
		{
			size_t* data;
			if (*ptrDate > 366)
			{
				data = TagJahrtoTagMonat(*(ptrDate + 1), *ptrDate);
				*(ptrDate + 2) = *ptrDate;
				*(ptrDate + 1) = *(data + 1);
				*ptrDate = *data;
			}
			else
			{
				data = TagJahrtoTagMonat(*ptrDate, *(ptrDate + 1));
				*(ptrDate + 2) = *(ptrDate+1);
				*(ptrDate + 1) = *(data + 1);
				*ptrDate = *data;
			}
		}
		else if (date_format == 3)
		{
			size_t puffer;
			puffer = *ptrDate;
			*ptrDate = *(ptrDate + 1);
			*(ptrDate + 1) = puffer;
		}
		else if (date_format == 5)
		{
			if (*ptrDate > 12)
			{
				*(ptrDate + 2) = *ptrDate;
				*ptrDate = 1;
			}
			else
			{
				*(ptrDate + 2) = *(ptrDate + 1);
				*(ptrDate + 1) = *ptrDate;
				*ptrDate = 1;
			}
		}
		else if (date_format == 60)
		{
			*(ptrDate + 2) = *(ptrDate);
			*(ptrDate + 1) = 1;
			*ptrDate = 1;
		}
		else if (date_format == 61)
		{
			*(ptrDate + 2) = *(ptrDate+1);
			*(ptrDate + 1) = 1;
			*ptrDate = 1;
		}
		else if (date_format == 62)
		{
			*(ptrDate + 1) = 1;
			*ptrDate = 1;
		}
		l_Date.day = *ptrDate;
		l_Date.month = *(ptrDate+1);
		l_Date.year = *(ptrDate+2);
		VecDate_.push_back(l_Date);
	}
	

}

template <class T> void Model_editor<T>::Doc_Names_to_Date(vecDates& Document_IDs_as_Date,const vocab& Document_Names)
{
	vocab::const_iterator itDocument_Names;
	std::vector<size_t*> vecDates;
	size_t* ptrDates = new size_t[3]{ 0 };

	for (itDocument_Names = Document_Names.begin(); itDocument_Names != Document_Names.end(); itDocument_Names++)
	{
		ptrDates = new size_t[3]{ 0 };

		unsigned short counter = 0;
		std::streamoff position = 0, old_position = 0;
		std::stringstream strstr;
		std::string puffer = *(itDocument_Names);
		size_t firstPosition = puffer.find_first_of("123456789");
		if (firstPosition != std::string::npos)
		{
			strstr << puffer.substr(firstPosition);
			while (counter < 3)
			{
				strstr >> *(ptrDates + counter);
				old_position = position;
				position = strstr.tellg();
				strstr.seekg(position + 1);
				if (position != old_position && position > 0)
				{
					counter++;
				}
				else
					break;
			}
			vecDates.push_back(ptrDates);
		}
	}
	if (vecDates.size())
	{
		//As we don't want to dictate a special format, we have to analyze the dates found
		getDateFormat(vecDates);
		if (date_format)
		{
			std::cout << "Using Date Format " << date_format << "." << std::endl;
			setVecDate(Document_IDs_as_Date, vecDates);
		}
		else
			std::cout << "Couldn't read the date format" << std::endl;
	}
	else
		std::cout << "No Numbers found" << std::endl;
}

template <class T> void Model_editor<T>::getUniqueDates(std::set<Dates>& setDates,const vecDates& Document_Dates)
{
	vecDates::const_iterator itDates;
	for (itDates = Document_Dates.begin(); itDates != Document_Dates.end(); itDates++)
		setDates.insert(*itDates);
}

template <class T> void Model_editor<T>::print_Topic_in_Dates(std::string filepath_model ,std::string filepath_doc_id_, const listTopicsinDocuments listTopinDoc_)
{
	vocab Document_Names; //Don't get confused because of the vocab. This function was build in at a very late point of development. As the document_id and the vocab has the same struture, we are reusing the vocabulary function
	vecDates Document_IDs_as_Date;

	if (open_vocabulary(Document_Names, filepath_doc_id_) > 0)
	{
		if (Document_Names.size() == listTopinDoc_.size())
		{
			Doc_Names_to_Date(Document_IDs_as_Date,Document_Names);
			if (Document_IDs_as_Date.size() > 0)
			{
				//We have all Informations we need, now we need to print
				std::ofstream file_topics, file_documents; //One file showing all Topics, one file showing all Documents
				std::set<Dates> setUniqueDates;
				std::string directory;
				if (filepath_model.find("/") != std::string::npos)
				{
					directory = filepath_model.substr(0, filepath_model.rfind("/") + 1);
				}
				else
					directory = "";
				file_topics.open(directory + "DateTopic.txt");
				file_documents.open(directory + "DateDocument.txt");
				getUniqueDates(setUniqueDates, Document_IDs_as_Date);
					for (std::set<Dates>::const_iterator itUniqueDates = setUniqueDates.begin(); itUniqueDates != setUniqueDates.end(); itUniqueDates++)
					{
						file_documents << *itUniqueDates;
						file_topics << *itUniqueDates;
						size_t* Topics = new size_t[getSize()]{ 0 };
						for (size_t counter = 0; counter < Document_IDs_as_Date.size(); counter++)
						{
							if (Document_IDs_as_Date[counter] == *itUniqueDates)
							{
								for (vecIndex::const_iterator itTopicsinDocuments = listTopinDoc_[counter].begin(); itTopicsinDocuments != listTopinDoc_[counter].end(); itTopicsinDocuments++)
									* (Topics + (*itTopicsinDocuments)) = *(Topics + (*itTopicsinDocuments)) + 1;
								file_documents << " " << counter;
								//if (counter + 1 == Document_IDs_as_Date.size())
								//{
									//for (size_t counter2 = 0; counter2 < Document_IDs_as_Date.size(); counter2++)

								//}
							}
						}
						for (size_t counter2 = 0; counter2 < getSize(); counter2++)
						{
							if (*(Topics + counter2))
								file_topics << " " << counter2 << ":" << *(Topics + counter2);
						}
						file_documents << std::endl;
						file_topics << std::endl;
					}
			}
			else
				std::cout << "No Dates found. Abort." << std::endl;
		}
		else
			std::cout << "Number of Documents doesn't match with the model. Wrong document_id file" << std::endl;
	}
}


template <class T> void Model_editor<T>::printHDP(std::string filepath_model,std::string filepath_doc_states,std::string filepath_doc_id, size_t topic)
{
	//Declaration of the variables
	vecDocStates vecDocStates_;
	vecPairIndexNumber vecSumTopics_;
	listTopicsinDocuments listTopinDoc_;
	std::set<vecIndex> setUniqueTopics;
	std::vector<mapIndex> Ancestors;
	std::vector<vecIndex> Hierarchy;
	size_t levels;

	//Doing the magic
	if (open_docstates(vecDocStates_, filepath_doc_states) > 0)
	{

		if (vecDocStates_[0].size() != getSize())
		{
			std::cout << "Number of topics differ. Abort" << std::endl << std::endl;
		}
		else
		{
			vecSumTopics_ = sum_Topic_Doc_States(vecDocStates_); //Results will be sorted automatic
			DocStates_to_ListTopics(listTopinDoc_, vecDocStates_, vecSumTopics_);
			printTopicsinDocument(listTopinDoc_, filepath_doc_states);
			levels = getUniqueTopics(setUniqueTopics, listTopinDoc_);
			printTopicLevelIndex(filepath_model, setUniqueTopics, levels);
			//if (MaxValue > 0) printAncestors(filepath_model, Ancestors);
			//createHierarchy(Hierarchy, TopicLevelIndex, levels);
			if (MaxValue > 0 && topic != std::numeric_limits<size_t>::max()) printHierarchy(filepath_doc_states, setUniqueTopics,topic);
			if (filepath_doc_id != "-")
				print_Topic_in_Dates(filepath_model, filepath_doc_id, listTopinDoc_);
		}
	}
	else
		std::cout << "Couldn't find " << filepath_doc_states << "! Please try again." << std::endl;
}

template <class T> void Model_editor<T>::printDates(std::string filepath_doc_states, std::string filepath_doc_id)
{
	vecDocStates vecDocStates_;
	listTopicsinDocuments listTopinDoc_;
	vecPairIndexNumber vecSumTopics_;
	open_docstates(vecDocStates_, filepath_doc_states);
	if (vecDocStates_[0].size() != getSize())
	{
		std::cout << "Number of topics differ. Abort" << std::endl << std::endl;
	}
	else
	{
		vecSumTopics_ = sum_Topic_Doc_States(vecDocStates_);
		DocStates_to_ListTopics(listTopinDoc_, vecDocStates_, vecSumTopics_);
		print_Topic_in_Dates(filepath_doc_states, filepath_doc_id, listTopinDoc_);
	}

}

template <class T> std::set<size_t> Model_editor<T>::DebugFillListTopicsinDocuments(listTopicsinDocuments& listTopinDoc_,const size_t& NumberDocuments)
{
	std::set<size_t> uniqueTopics;
	//size_t NumberDocuments = Document_Names.size();
	for (size_t counter = 0; counter < NumberDocuments; counter++)
	{
		vecIndex puffer;
		size_t randNumber_Topics = rand() % 8 + 1;
		for (size_t counter2 = 0; counter2 < randNumber_Topics; counter2++)
		{
			size_t rand_Topic = rand() % 99;
			puffer.push_back(rand_Topic);
			uniqueTopics.insert(rand_Topic);
		}
		listTopinDoc_.push_back(puffer);

	}
	return uniqueTopics;
}

template <class T> void Model_editor<T>::DebugFillModel(const size_t& NumberTopics)
{
	vecT vecModel;
	for (size_t counter = 0; counter < NumberTopics; counter++)
	{
		Model.push_back(vecModel);
	}
}
template <class T> void Model_editor<T>::DebugFillModelwithDocuments(const size_t& NumberDocuments)
{
	size_t value;
	for (size_t counter = 0; counter < getSize(); counter++)
	{
		for (size_t counter2 = 0; counter2 < NumberDocuments; counter2++)
		{
			value = rand() % 100;
			Model[counter].push_back(value);
		}
	}
}
template <class T> void Model_editor<T>::DebugMode()
{
	std::cout << "You are in the Debug Mode of TopModHis" << std::endl << std::endl;
	unsigned short choice = 0;
	do
	{
		std::cout << "Please select the routine you want to check:" << std::endl;
		std::cout << "1: Doc_Names_to_Date" << std::endl;
		std::cout << "2: Print_Topic_in_dates" << std::endl;
		std::cout << "3: PrintHDP" << std::endl;
		std::cout << "4: Print Topics" << std::endl;
		std::cout << "0: Exit" << std::endl;
		std::cout << std::endl;
		std::cout << "Your choice: ";
		std::cin >> choice;

		switch (choice)
		{
			case 1:
			{
				std::cout << "We need a document with filenames. Please tell the path to a testfile:" << std::endl;
				std::string filepath;
				std::cout << "Filepath: ";
				std::cin >> filepath;
				vocab Document_Names; //Don't get confused because of the vocab. This function was build in at a very late point of development. As the document_id and the vocab has the same struture, we are reusing the vocabulary function
				vecDates Document_IDs_as_Date;
				date_format = 0;
				if (open_vocabulary(Document_Names, filepath) > 0)
				{
					Doc_Names_to_Date(Document_IDs_as_Date, Document_Names);
				}
				else
				{
					std::cout << "Something is wrong with the file, no informations found. Please try again" <<std::endl;
					std::cout << "Press Enter to continue";
					std::cin.get();
				}
				break;
			};
			case 2:
			{
				listTopicsinDocuments listTopinDoc_;
				std::cout << "We need a document with filenames. Please tell the path to a testfile:" << std::endl;
				std::string filepath;
				std::cout << "Filepath: ";
				std::cin >> filepath;
				std::set<size_t> uniqueTopics;
				vocab Document_Names; //Don't get confused because of the vocab. This function was build in at a very late point of development. As the document_id and the vocab has the same struture, we are reusing the vocabulary function
				vecDates Document_IDs_as_Date;
				date_format = 0;
				if (open_vocabulary(Document_Names, filepath) > 0)
				{
					
					uniqueTopics = DebugFillListTopicsinDocuments(listTopinDoc_, Document_Names.size());
					DebugFillModel(uniqueTopics.size());
					print_Topic_in_Dates(filepath, filepath, listTopinDoc_);
				}
				else
				{
					std::cout << "Something is wrong with the file, no informations found. Please try again" << std::endl;
					std::cout << "Press Enter to continue";
					std::cin.get();
				}
				break;
				
			}
			case 3:
			{
				std::string filepath_doc_states, filepath_doc_id;
				size_t NumberTopics;
				std::cout << "Please type in the filepath to the .doc.states or *-gamma.dat file \n";
				std::cout << "Path: ";
				std::cin >> filepath_doc_states;
				std::cout << "If you want to print the Dates, please type in the path to the Document_ID file,else type in - \n";
				std::cout << "Path: ";
				std::cin >> filepath_doc_id;
				if (!getSize())
				{
					std::cout << "How many topics shall be prepared? \n";
					std::cout << "Number topics: ";
					std::cin >> NumberTopics;
					DebugFillModel(NumberTopics);
				}
				if (!getSizeVocab())
				{
					std::cout << "How many Documents shall be in the topics? \n";
					std::cout << "Number Documents: ";
					std::cin >> NumberTopics;
					DebugFillModelwithDocuments(NumberTopics);
				}
				printHDP(filepath_doc_states, filepath_doc_states, filepath_doc_id);
				break;
			}
			case 4:
			{
				std::string filepath_vocab;
				size_t NumberTopics;
				std::cout << "Please type in the filepath to the Vocabulary file \n";
				std::cout << "Path: ";
				std::cin >> filepath_vocab;
				if (!getSize())
				{
					std::cout << "How many topics shall be prepared? \n";
					std::cout << "Number topics: ";
					std::cin >> NumberTopics;
					DebugFillModel(NumberTopics);
				}
				if (!getSizeVocab())
				{
					std::cout << "How many Documents shall be in the topics? \n";
					std::cout << "Number Documents: ";
					std::cin >> NumberTopics;
					DebugFillModelwithDocuments(NumberTopics);
				}
				printTopics(10, "", filepath_vocab);
				break;
			}
		
		}
		for(short i = 0; i<10 ;i++)
			std::cout << std::endl;
	} while (choice);
}

