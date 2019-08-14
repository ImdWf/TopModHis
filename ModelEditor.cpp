#include <stdio.h>
#include "ModelEditor.h"


template <class T>
void Menu(T& obMe, std::string &filepath)
{
	short bMenu(1);
	do
	{
		bMenu= 1;
		size_t originalNumberTopics(obMe.getSize());
		std::cout << "Please choose:\n";
		std::cout << "1 = delete topic\n";
		std::cout << "2 = add topic\n";
		std::cout << "3 = append another model\n";
		std::cout << "4 = save topic to edit in external file\n";
		std::cout << "5 = show a range of values of a topic\n";
		std::cout << "6 = edit single value in a topic\n";
		std::cout << "7 = Print Topics\n";
		if (obMe.MaxValue > 0) std::cout << "8 = Print Hierarchy\n";
		else std::cout << "8 = Print assignment document to topics\n";
		std::cout << "9 = Print Dates\n";
		std::cout << "10 = exit and save\n";
		std::cout << "0 = exit\n";
		std::cout << "Your choice: ";
		std::cin >> bMenu;
		switch (bMenu)
		{
			case 1:
			{
				size_t topic;
				std::cout << "Which topic shall be deleted? (0 - " << obMe.getSize() - 1 << ") ";
				std::cin >> topic;
				if (topic < obMe.getSize()) obMe.deleteTopic(topic);
				else std::cout << "\n\nInvalid value\n\n";
				break; 
			};
			case 2:
			{
				std::string filepath;
				size_t position;
				bool append(false), multi(false);
				char charAppend;
				std::cout << "Please write the filepath.";
				if (obMe.getSize() > 0)
				{
					std::cout << " The topic have to in the size of " << obMe.getSizeVocab() << "!\n";
					std::cout << "To long topics will be cutted, to short topics will be filled with 0.\n";
				}
				else 
					std::cout << std::endl;
				std::cout << "Each line is a word\n";
				std::cout << "Filepath: ";
				std::cin >> filepath;
				std::cout << "At which position shall the topic be added?\n  0= Begin und " << obMe.getSize() << " = End\n";
				std::cin >> position;
				std::cout << "Do you wish to overwrite the topic?  [Y/N] ";
				std::cin >> charAppend;
				if (charAppend == 'N' || charAppend == 'n') append = true;
				std::cout << "Is there more than one topic included?  [Y/N] ";
				std::cin >> charAppend;
				if (charAppend == 'Y' || charAppend == 'y') multi = true;
				obMe.appendTopic(filepath, position, append, multi);
				break;
			};
			case 3:
			{
				std::string filepath2model;
				std::cout << "Please type in the filepath to the " << filepath.substr(filepath.rfind('.'),filepath.size()) <<" file \n";
				std::cout << "Path: ";
				std::cin >> filepath2model;
				if (filepath2model.find(filepath.substr(filepath.rfind('.'), filepath.size())) != std::string::npos)
				{
					std::cout << "Start reading model... \n";
					obMe.readModel(filepath2model,false);
				}
				else
				{
					std::cout << "Wrong filename, back to main menu \n\n";
				}
				break;
			}
			case 4:
			{
				size_t topicnumber_start,topicnumber_end;
				std::string filepath;
				std::cout << "Which topic do you want to export? (0 - " << obMe.getSize() - 1 << ")\n";
				std::cout << "Topic No. ";
				std::cin >> topicnumber_start;
				std::cout << "Do you want to export a range? If yes, type in the end of the range, else just repeat the first topic" << std::endl;
				std::cout << topicnumber_start << " - " << obMe.getSize() - 1 << ": \n";
				std::cin >> topicnumber_end;
				if (topicnumber_start <= topicnumber_end)
				{
					std::cout << "Please write a filepath for the export: \n";
					std::cout << "Filepath: ";
					std::cin >> filepath;
					obMe.saveTopic(filepath, topicnumber_start, topicnumber_end);
				}
				else
					std::cout << "The end has to be bigger than the start!. Abort";
				break;
			};
			case 5:
			{
				size_t topicnumber, begin, end;
				//auto value;
				std::cout << "Which topic do you want to edit? (0 - " << obMe.getSize() - 1 << ")\n";
				std::cout << "Topic No. ";
				std::cin >> topicnumber;
				std::cout << "Start? (0 - " << obMe.getSizeVocab() - 1 << ")\n";
				std::cout << "Start: ";
				std::cin >> begin;
				std::cout << "End? ("<< begin <<" - " << obMe.getSizeVocab() - 1 << ")\n";
				std::cout << "End: ";
				std::cin >> end;
				obMe.showValue(topicnumber, begin, end);

				break;
			}
			case 6:
			{
				size_t topicnumber, vocabnumber;
				//auto value;
				std::cout << "Which topic do you want to edit? (0 - " << obMe.getSize() - 1 << ")\n";
				std::cout << "Topic No. ";
				std::cin >> topicnumber;
				std::cout << "Which word do you want to edit? (0 - " << obMe.getSizeVocab() - 1 << ")\n";
				std::cout << "Word No. ";
				std::cin >> vocabnumber;
				obMe.editTopic(topicnumber, vocabnumber);
				break;
			};
			case 7:
			{
				size_t topwords = 0;
				std::cout << "How many topwords shall be printed? \n";
				std::cin >> topwords;
				obMe.printTopics(topwords, filepath, "-");
				break;
			};
			case 8:
			{
				
				std::string filepath_doc_states, filepath_doc_id;
				size_t topic;
				if (obMe.MaxValue > 0) std::cout << "Please type in the filepath to the .doc.states file \n";
				else std::cout << "Please type in the filepath to the *-gamma.dat file \n";
				std::cout << "Path: ";
				std::cin >> filepath_doc_states;
				std::cout << "If you want to print the Dates, please type in the path to the Document_ID file,else type in - \n";
				std::cout << "Path: ";
				std::cin >> filepath_doc_id;
				std::cout << "If you want to print all combined topics for a specific topic, please type in the number, else type in " << std::numeric_limits<size_t>::max() << "\n";
				std::cout << "Topic: ";
				std::cin >> topic;
				obMe.printHDP(filepath,filepath_doc_states,filepath_doc_id, topic);
				break;
			}
			case 9:
			{
				std::string filepath_doc_states, filepath_doc_id;
				if (obMe.MaxValue>0 ) std::cout << "Please type in the filepath to the .doc.states file \n";
				else std::cout << "Please type in the filepath to the -gamma.dat file \n";
				std::cout << "Path: ";
				std::cin >> filepath_doc_states;
				std::cout << "Please type in the filepath to the Document_id file \n";
				std::cout << "Path: ";
				std::cin >> filepath_doc_id;
				obMe.printDates(filepath_doc_states,filepath_doc_id);
				break;
			}
			case 10:
			{
				std::string new_name = "";
				char charYN;
				std::cout << "You are sure to quit? [Y/N]";
				std::cin >> charYN;
				if (charYN == 'Y' || charYN == 'y') bMenu = 0;
				std::cout << "You want to overwrite the old Model? [Y/N]";
				std::cin >> charYN;
				if (charYN == 'N' || charYN == 'n')
				{
					std::cout << "Please write a name for the new Model without the ending .beta or .topics: \n";
					std::cout << "Name: ";
					std::cin >> new_name;
				}
				obMe.saveModel(filepath,new_name);
				break;
			};
			case 42:
			{
				obMe.DebugMode();
				bMenu = 0;
				break;
			}
		}
		for (short i = 0; i < 5; i++)
			std::cout << std::endl;
	} while (bMenu > 0);
}

template <class T>
void printInformations(T& obMe)
{
	std::cout << "The model includes " << obMe.getSize() << " topics\n";
	if (obMe.getSize()>0) std::cout << "The model includes " << obMe.getSizeVocab() << " terms\n";
	std::cout << "The minimum got set to " << obMe.MinValue << ". \n";
	std::cout << "The maximum got set to " << obMe.MaxValue << ". \n";
}

int main (int argc, char* argv[])
{
 // std::cout << "Hallo Welt" ;
	std::string filepath;
	if (argc < 2)
	{
		
		std::cout << "No filepath given\n";
		std::cout << "Please type in the filepath to the .beta or .topics file \n";
		std::cout << "Path: ";
		std::cin >> filepath;
	}
	else
	{
		filepath = argv[1];
	}
	std::cout << "Reading Model at " << filepath << std::endl;
	if (filepath.find(".topics") != std::string::npos)
	{
		Model_editor<size_t> obMe;
		if (obMe.readModel(filepath,true))
		{
			printInformations(obMe);
			if (argc > 4)
			{
				std::string puffer = argv[2];
				if (puffer.find("doc.states") != std::string::npos)
				{
					std::cout << "Printing HDP_Model with " << puffer << std::endl;
					std::cout << "Printing Dates" << std::endl;
					obMe.printHDP(filepath, argv[2], argv[3], atoi(argv[4]));
				}
			}
			else if (argc > 3)
			{
				std::string puffer = argv[2];
				if (puffer.find(".") == std::string::npos)
				{
					size_t NumberWords = atoi(argv[2]);

					std::cout << "Printing Topic with " << NumberWords << " Topwords" << std::endl;

					obMe.printTopics(NumberWords, filepath, argv[3]);
				}
				else
				{
					if (puffer.find("doc.states") != std::string::npos)
					{
						std::cout << "Printing HDP_Model with " << puffer << std::endl;
						std::cout << "Printing Dates" << std::endl;
						obMe.printHDP(filepath, argv[2], argv[3]);
					}
				}
			}
			else if (argc > 2)
			{
				std::string filepath_doc_states = argv[2];
				if (filepath_doc_states.find("doc.states") != std::string::npos)
				{
					std::cout << "Printing HDP_Model with " << filepath_doc_states << std::endl;
					obMe.printHDP(filepath, argv[2]);
				}
			}
			else
				Menu(obMe, filepath);
		}
	}
	else if (filepath.find(".beta") != std::string::npos)
	{
		Model_editor<float> obMe;
		if (obMe.readModel(filepath,true))
		{
			printInformations(obMe);
			if (argc > 3)
			{
				std::string puffer = argv[2];
				if (puffer.find(".") == std::string::npos)
				{
					size_t NumberWords = atoi(argv[2]);

					std::cout << "Printing Topic with " << NumberWords << " Topwords" << std::endl;

					obMe.printTopics(NumberWords, filepath, argv[3]);
				}
				else
				{
					if (puffer.find("gamma.dat") != std::string::npos)
					{
						std::cout << "Printing Dates" << std::endl;
						obMe.printHDP(filepath,argv[2], argv[3]);
					}
				}
			}
			else
				Menu(obMe, filepath);
		}
	}
	else if (filepath == "42")
	{
		Model_editor<size_t> obMe;
		obMe.MaxValue = 10000;
		obMe.MinValue = 0;
		obMe.DebugMode();
	}
	else if (filepath == "42,42")
	{
		Model_editor<float> obMe;
		obMe.MaxValue = -1;
		obMe.MinValue = -1000;
		obMe.DebugMode();
	}
	else
	{ 
		std::cout << "Kein gueltiger Pfad";
		return -2; 
	}
  return 0;
}