#include"List.h"
#include<string>
#include<iomanip>
#include<fstream>
#include<iostream>
#include"HashTable.h"
using namespace std;
class Doc_Info
{
	string docId;
	int tf;//term frequence
public:
	Doc_Info():docId("") {}//Default Constructor
	Doc_Info(const string& docId) :docId(docId), tf(1) {//Parametrized Constructor
	}
	~Doc_Info() {//Destructor
	}
	string getDocId() {
		return docId;
	}
	int trmFreq() {
		return tf;
	}
	void incrmTermFreq() {//Increment The Term Frequency
		++tf;
	}
};
class  Term_Info {
	string keyTerm;
	list<Doc_Info> docLst;
public:
	Term_Info() :keyTerm("") {//Default Constructor
	}
	Term_Info(const string& keyTerm, const string& fileName) :keyTerm(keyTerm) {//Parametrized Constructor
		docLst.insert(Doc_Info(fileName));
	}
	~Term_Info() {
	}
	//getters
	string getKeyTerm() {
		return keyTerm;
	}
	list<Doc_Info> getDocLst() {
		return docLst;
	}
	void addDocId(const string& docId) {//when same key term exists, then check document list
		for (auto itr = docLst.begin(); itr != docLst.end(); ++itr) {
			if (itr->getDocId() == docId) {//If same Doc Id found than add document id to that keyterm list
				itr->incrmTermFreq();
				return;
			}
		}
		//same document id not found
		docLst.insert(Doc_Info(docId));
	}
	void print() {
		cout << setw(13) << left << keyTerm;

		for (auto itr = docLst.begin(); itr != docLst.end(); ++itr) {
			cout << "  ||  {" << itr->getDocId() << "," << itr->trmFreq() << "}";
		}
		cout << endl;
	}
	friend ostream& operator << (ostream& out, Term_Info& obj) {
		obj.print();
		return out;
	}
};
class Search_Engine
{
	HashTable<string, Term_Info> index;
public:
	Search_Engine(){//Default Constructor
	}
	~Search_Engine() {//Destructor
	}
	void createIndex(const string* fileName, int n) {//Open files to get Data
		ifstream dataIn;
		for (int i = 0; i < n; ++i) {
			dataIn.open(fileName[i]);
			if (!dataIn.fail()) {
				addData(dataIn, fileName[i]);
			}
			else {//File Name Not Correct
				cout << "[WARNING] || " << fileName[i] << " File Not Found" << endl;
				system("pause");
			}
		}
	}
	void printAllData() {
		cout << "Printing All Data\nNumber of Key Term " << index.size() << "\nWord\t{DocumentName.txt,TermFrequency}" << endl;
		index.print();
	}
	void inputSrc() {//search input function
		cin.ignore();
		cin.clear();
		string usrInput;
		string* wrdStr = nullptr;
		cout << "Enter Your Search : " << endl;
		getline(cin, usrInput);
		int wrdCnt = tokenize(usrInput, wrdStr);//calling function to tokenize
		rankAndPrint(wrdStr, wrdCnt);//calling function to rank search according to the conditions(term frequency e.t.c)
		delete[]wrdStr;//clearing up memory
		wrdStr = nullptr;
	}
	void addDoc() {//function to add a new document 
		cin.ignore();
		cin.clear();
		string docName = "";
		cout << "Enter Your Document Name : " << endl;
		getline(cin, docName);
		createIndex(&docName, 1);
	}
private:
	void addData(ifstream& dataIn, const string& fileName) {//extracting whole data from the file word by word
		string keyTerm;
		while (!dataIn.eof()) {//while end of file is not found
			dataIn >> keyTerm;//inputing word
			if (index.find(keyTerm)) {
				index[keyTerm].addDocId(fileName);
			}
			else {
				index.insert(keyTerm, Term_Info(keyTerm, fileName));
				
			}
		}
		dataIn.close();
	}
	int tokenize(const string& srcStr, string*& dstStr) {//convert string into single dimensional word, by removing unnecessary elements
		int wrdCnt = 0, idx = 0, n = 0;
		while (srcStr[idx++] != '\0') {//calculate number of words
			if (srcStr[idx] == ' ') {
				++wrdCnt;
			}
		}
		dstStr = new string[++wrdCnt];
		idx = 0;
		string tmpStr;
		while (srcStr[idx] != '\0') {//store words into string array
			if (srcStr[idx] == ' ') {
				dstStr[n++] = tmpStr;
				tmpStr = "";
			}
			else {
				tmpStr += srcStr[idx];
			}
			++idx;
		}
		dstStr[n] = tmpStr;
		return wrdCnt;
	}
	void rankAndPrint(string*& wrdStr, int wrdCnt) {//function that ranks our search 
		list<list<Doc_Info>> docLst;//List to store the document list 
		//Loop to store Doc_lists
		for (int i = 0; i < wrdCnt; ++i) {
			bool found = false;
			if (index.find(wrdStr[i])) {//if same keyterm found, then store an iterator of that doc list
				docLst.insert(index[wrdStr[i]].getDocLst());
				found = true;
			}
			if (found == false) {//Word Not Found
				cout << "[INFO] || Word  " << wrdStr[i] << " Not Found In DataBase" << endl;
			}
		}
		if (!docLst.empty()) {//If No Similar KeyTerms Found In our DataBase
			struct mySet
			{
				string docId;
				int tf;//term Frequency
				int cnt;
				mySet(string docId, int tf, int cnt) :docId(docId), tf(tf), cnt(cnt) {}
			};
			list<mySet*> rankLst;
			auto docItr = docLst.begin();
			for (int i = 0; i < docLst.size(); ++i, ++docItr) {
				auto docLstItr = docItr->begin();
				for (int j = 0; j < docItr->size(); ++j, ++docLstItr) {
					rankLst.insert(new mySet(docLstItr->getDocId(), docLstItr->trmFreq(), 1));
				}
			}
			//Now taking union and sorting data according to the given conditions
			for (auto iItr = rankLst.begin(); iItr != rankLst.end(); ++iItr) {
				if (*iItr != nullptr) {
					auto tmpItr = iItr;
					for (auto jItr = ++tmpItr; jItr != rankLst.end(); ++jItr) {
						if (*jItr != nullptr && (*iItr)->docId == (*jItr)->docId) {
							++(*iItr)->cnt;
							delete* jItr;
							*jItr = nullptr;
						}
					}
				}
			}
			for (auto iItr = rankLst.begin(); iItr != rankLst.end(); ++iItr) {
				if (*iItr != nullptr) {
					auto tmpItr = iItr;
					for (auto jItr = ++tmpItr; jItr != rankLst.end(); ++jItr) {
						if (*jItr != nullptr) {
							if ((*iItr)->cnt == (*jItr)->cnt) {//if both have same query terms
								if ((*iItr)->tf == (*jItr)->tf) {//if both have same term frequency, swap alphabetically
									if ((*iItr)->docId > (*jItr)->docId) {
										swap(*iItr, *jItr);
									}
								}
								else if ((*iItr)->tf <= (*jItr)->tf) {
									swap(*iItr, *jItr);
								}
							}
							else if ((*iItr)->cnt <= (*jItr)->cnt) {
								swap(*iItr, *jItr);
							}
						}
					}
				}
			}
			//Printing our Ranked Document as well as cleaning up memory
			cout << "DocumentName.txt" << endl;
			for (auto itr = rankLst.begin(); itr != rankLst.end(); ++itr) {
				if (*itr != nullptr) {
					cout << (*itr)->docId << endl;
					delete* itr;
					*itr = nullptr;
				}
			}
		}
	}
};
int main() {
	const int numFile = 4;
	string fileName[numFile] = { "Doc1.txt","Doc2.txt","Doc3.txt","Doc4.txt" };
	Search_Engine srcEng;
	srcEng.createIndex(fileName, numFile);
	srcEng.printAllData();
	char con = '0';
	while (con != '3') {
		cout << "Menu\n1)Input Search\n2)Add Document\n3)Exit\n->";
		cin >> con;
		switch (con)
		{
		case '1':
			srcEng.inputSrc();
			break;
		case '2':
			srcEng.addDoc();
		}
		if (con < '3') {//If condition to wait before clear screen until user press some key
			cin.ignore(2, '\n');
			cin.clear();
		}
	}
	system("pause");
	return 0;
}
