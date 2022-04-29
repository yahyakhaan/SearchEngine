#include <iostream>
#include <string>
#include <conio.h>
#include <iomanip>
#include <fstream>
using namespace std;

template<class T>
class Node //Node class
{ 
	template<typename T>friend class List;
private:
	T data;
	Node<T>* next;
	Node<T>* prev;
};

template <typename T>
class List //doubly linked list class
{

	Node<T>* head;
	Node<T>* tail;
	int	count;
public:
	class Iterator //Iterator class
	{
		Node<T>* itrCrnt;
	public:
		Iterator()
		{
			itrCrnt=nullptr;
		}
		Iterator(Node<T>* objItr)
		{
			itrCrnt=objItr;
		}
		Iterator(const Iterator& objItr)
		{
			itrCrnt=objItr.itrCrnt;
		}
		~Iterator()
		{
			itrCrnt=nullptr;
		}
		Iterator operator = (const Iterator& objItr)
		{
			this->itrCrnt=objItr.itrCrnt;
		}
		bool operator != (const Iterator& objItr)
		{
			if(this->itrCrnt!=objItr.itrCrnt)
			{
				return true;
			}
			return false;
		}
		T& operator* ()
		{
			return itrCrnt->data;
		}
		Iterator& operator++()
		{
			itrCrnt=itrCrnt->next;
			return (*this);
		}
		T* operator ->()
		{
			return (&itrCrnt->data);
		}
	};
	//constructors and destructors of List class
	List() //nullptr
	{
		init(); //function made to initialize values
	}
	List(const List& obj) //copy constructor
	{
		init();
		Node<T>* temp=obj.head;
		while(temp!=nullptr)
		{
			this->push_back(temp->data);
			temp=temp->next;
		}
	}
	~List() //destructor
	{
		Node<T>* temp=head;
		while(head!=nullptr)
		{
			head=head->next;
			delete temp;
			temp=head;
		}
	}
	//Operators 
	List operator = (const List& obj)
	{
		if(this!=&obj)
		{
			this->init();
			Node<T>* temp=obj.head;
			while(temp!=nullptr)
			{
				this->push_back(temp->data);
				temp=temp->next;
			}
		}
		return (*this);
	}
	//Utility Members
	void push_back(T val)
	{
		if(head==nullptr) //Empty List
		{
			head=tail=new Node<T>;
			head->data=val;
			head->prev=nullptr;
			tail->next=nullptr;
		}
		else //element somewhere in middle
		{
			Node<T>* tempPtr=new Node<T>;
			tempPtr->prev=tail;
			tail=tail->next=tempPtr;
			tail->data=val;
			tail->next=nullptr;
		}
		++count;
	}
	inline int size()const
	{
		return count;
	}
	inline int empty()
	{
		return count==0;
	}
	Iterator begin()
	{
		Iterator temp(head);
		return temp;
	}
	Iterator end()
	{
		Iterator temp;
		return temp;
	}
	private:
		inline void init()//initializing values
		{
			count=0;
			head=tail=nullptr;
		}
};

class Doc_Info
{
	string docID;
	int tf;//term frequency
public:
	Doc_Info()//Default Constructor
	{
		docID="";
	}
	Doc_Info(const string& docid)//Parameterized Constructor
	{
		this->docID=docid;
		tf=1;
	}
	~Doc_Info()
	{
	}
	//Getters
	string GetDocID()
	{
		return docID;
	}
	int termFreq()
	{
		return tf;
	}
	void incrmTermFreq()
	{
		++tf;
	}
};

class Term_Info
{
	string keyTerm;
	List<Doc_Info> docList;
public:
	Term_Info()//Default Constructor
	{
		keyTerm="";
	}
	Term_Info(const string& keyterm, const string& fileName)//Parameterized Constructor
	{
		keyTerm=keyterm;
		docList.push_back(Doc_Info(fileName));
	}
	~Term_Info()
	{
	}
	//getters
	string getKeyTerm()
	{
		return keyTerm;
	}
	List<Doc_Info> getDocList()
	{
		return docList;
	}
	void addDocId(const string& docID)//when same key term exists, then check documnet list
	{
		for(auto itr=docList.begin();itr!=docList.end();++itr)
		{
			if(itr->GetDocID()==docID)//if same docID found then add that docID to keyterm list
			{
				itr->incrmTermFreq();
				return;
			}
		}
		docList.push_back(Doc_Info(docID));//if same docID not found
	}
	void print()
	{
		cout<<keyTerm;
		for(auto itr=docList.begin();itr!=docList.end();++itr)
		{
			cout<<" || {"<<itr->GetDocID()<<" , "<<itr->termFreq()<<" }";
		}
		cout<<endl;
	}
};

class Search_Engine
{
	int arrSize;
	List<Term_Info> index;
public:
	Search_Engine()//Default Constructor
	{
		arrSize=0;
	}
	~Search_Engine()//Destructor
	{}
	void createIndex(const string* fileName, int n)//Open files to get data
	{
		ifstream dataIn;
		for(int i=0;i<n;++i)
		{
			dataIn.open(fileName[i]);
			if(!dataIn.fail())
			{
				addData(dataIn, fileName[i]);
				cout<<fileName[i]<<" Added"<<endl;
				//getch();
			}
			else//if incorrect file name
			{
				cout<<"[WARNING] || "<<fileName[i]<<" File NOT Found"<<endl;
				cout<<endl;
			}
		}
	}
	void printAllData()
	{
			cout << "\nPrinting All Data\n\nNumber of Key Term " << arrSize <<"\n\nWord\t{DocumentName.txt,TermFrequency}"<<endl<<endl;;
			for(auto itr=index.begin(); itr!=index.end();++itr)
			{
				itr->print();
			}
	}
	void inputSrc()//search input function
	{
		cin.ignore();
		string usrInput;
		string* wrdStr=nullptr;
		cout<<"Enter your search: ";
		getline(cin,usrInput);
		int wrdCount=tokenize(usrInput,wrdStr);//calling function to tokenize
		rankAndPrint(wrdStr,wrdCount);//calling function to rank search according to the conditions(term frequency e.t.c)
		delete[] wrdStr;//clearing up memory
		wrdStr=nullptr;
	}
	void addDoc()//function to add a new document 
	{
		cin.ignore();
		string docName="";
		cout<<"Enter your document name: ";
		getline(cin,docName);
		createIndex(&docName,1);
	}
private:
	void srcAlreadyExists(const string& keyTerm, const string& fileName)//searching whether keyterm already exists or not
	{
		for(auto itr=index.begin();itr!=index.end();++itr)
		{
			if(itr->getKeyTerm()==keyTerm)//if same keyterm is found, then add document id to that keyterm list
			{
				itr->addDocId(fileName);
				return;
			}
		}
		index.push_back(Term_Info(keyTerm, fileName));//if same key term is not found then add that keyterm
		++arrSize;
	}
	void addData(ifstream& dataIn, const string& fileName)//extracting whole data from the file word by word
	{
		string keyTerm;
		while(!dataIn.eof())//while end of file is not found
		{
			dataIn>>keyTerm;//inputing word
			int size=keyTerm.size();
			for(int i=0;i<size;i++)
			{
				if(keyTerm[i]>=65&&keyTerm[i]<=92)
				{
					keyTerm[i]=keyTerm[i]+32;
				}
			}
			if(index.empty())//if database is empty
			{
				index.push_back(Term_Info(keyTerm,fileName));
				++arrSize;
			}
			else
			{
				srcAlreadyExists(keyTerm,fileName);
			}
		}
		dataIn.close();
	}
	int tokenize(const string& srcStr, string *& dstStr)//convert string into single dimensional word, by removing unnecessary elements
	{
		int wrdCount=0,idx=0,n=0;
		while(srcStr[idx++]!='\0')//calculating number of words
		{
			if(srcStr[idx]==' ')
			{
				wrdCount++;
			}
		}
		dstStr=new string[++wrdCount];
		idx=0;
		string tmpStr;
		while(srcStr[idx]!='\0')//storing words into a string array
		{
			if(srcStr[idx]==' ')
			{
				dstStr[n++]=tmpStr;
				tmpStr="";
			}
			else
			{
				tmpStr+=srcStr[idx];
			}
			++idx;
		}
		dstStr[n]=tmpStr;
		return wrdCount;
	}
	void rankAndPrint(string*& wrdStr, int wrdCount)//function that ranks our search 
	{
		List<List<Doc_Info>> docList;//list to store document list 
		for(int i=0;i<wrdCount;++i)//loop to store Doc_List
		{
			bool found=false;
			for(auto itr=index.begin();itr!=index.end()&&!found;++itr)
			{
				if(itr->getKeyTerm()==wrdStr[i])//if same keyterm found, then store an iterator of that doc list
				{
					docList.push_back(itr->getDocList());
					found=true;
				}
			}
			if(found==false)//if word is not found
			{
				cout<<"[INFO] || Word "<<wrdStr[i]<<" NOT found in database"<<endl;
			}
		}
		if(!docList.empty())//if no similar keyterms found in our database
		{
			struct mySet
			{
				string docID;
				int tf;//term frequency
				int cnt;
				mySet(string docid,int tf1, int count)
				{
					docID=docid;
					tf=tf1;
					cnt=count;
				}
			};
			List<mySet*>rankList;
			auto docItr=docList.begin();
			for(int i=0;i<docList.size();++i,++docItr)
			{
				auto docLstItr=docItr->begin();
				for(int j=0;j<docItr->size();++j,++docLstItr)
				{
					rankList.push_back(new mySet(docLstItr->GetDocID(),docLstItr->termFreq(),1));
				}
			}
			//Now taking union and sorting data according to the given conditions
			for(auto iItr=rankList.begin();iItr!=rankList.end();++iItr)
			{
				if(*iItr!=nullptr)
				{
					auto tmpItr=iItr;
					for(auto jItr=++tmpItr;jItr!=rankList.end();++jItr)
					{
						if(*jItr!=nullptr&&(*iItr)->docID==(*jItr)->docID)
						{
							++(*iItr)->cnt;
							delete (*jItr);
							*jItr=nullptr;
						}
					}
				}
			}
			for(auto iItr=rankList.begin();iItr!=rankList.end();++iItr)
			{
				if(*iItr!=nullptr)
				{
					auto tmpItr=iItr;
					for(auto jItr=++tmpItr;jItr!=rankList.end();++jItr)
					{
						if(*jItr!=nullptr)
						{
							if((*iItr)->cnt==(*jItr)->cnt)//if both have same query terms
							{
								if((*iItr)->tf==(*jItr)->tf)//if both have same term frequency, swap alphabetically
								{
									if((*iItr)->docID>(*jItr)->docID)
									{
										swap(*iItr,*jItr);
									}
								}
									else if((*iItr)->tf<=(*jItr)->tf)
									{
										swap(*iItr,*jItr);
									}
							}
									else if((*iItr)->cnt<=(*jItr)->cnt)
									{
										swap(*iItr,*jItr);
									}
						}
					}
				}
			}
			//Printing our Ranked Document as well as cleaning up memory
			cout<<"DocumentName.txt"<<endl;
			for(auto itr=rankList.begin();itr!=rankList.end();++itr)
			{
				if(*itr!=nullptr)
				{
					cout<<(*itr)->docID<<endl;
					delete *itr;
					*itr=nullptr;
				}
			}
		}
	}
};

int main()
{
	string fileName[4]={"Doc1.txt","Doc2.txt","Doc3.txt","Doc4.txt"};
	Search_Engine srcEng;
	srcEng.createIndex(fileName,4);
	srcEng.printAllData();
	char a='0';
	while(a!='3')
	{
		cout<<"\nMenu"<<endl;
		cout<<"1) Input Search\n2) Add Document\n3) Exit Program\n->";
		cin>>a;
		switch(a)
		{
		case'1':
			srcEng.inputSrc();
			break;
		case '2':
			srcEng.addDoc();
			break;
		case '3':
			return 0;
		}
	};

	getch();
	system("pause");
	getch();
}







		