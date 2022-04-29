#include<queue>
#include<stack>
#include<string>
#include<iomanip>
#include<fstream>
#include<iostream>
#include<conio.h>
#include<algorithm>
using namespace std;
template<typename type>
class avl {
	struct treeNode {//node struct
		type data;
		int h;
		treeNode* lchild;
		treeNode* rchild;
		treeNode(const type & data,int h=0,treeNode* lchild=nullptr, treeNode* rchild = nullptr):data(data),h(h),lchild(lchild),rchild(rchild) {}
	};
	treeNode* root;
	int count;
public:
	class iterator {//iterator class
		queue<treeNode*>* treeNodeList;
	public:
		iterator(treeNode* ptr = nullptr) {
			treeNodeList = new queue<treeNode*>;
			if (ptr != nullptr) {
				fillTheQueue(ptr);
			}
			treeNodeList->emplace(nullptr);
		}
		iterator(const iterator& obj) {//copy constructor
			treeNodeList = new queue<treeNode*>;
			*treeNodeList = *(obj.treeNodeList);
		}

		type& operator* () {
			if (!treeNodeList->empty()) {
				return treeNodeList->front()->data;
			}
		}

		iterator operator++() {
			if (!treeNodeList->empty()) {
				treeNodeList->pop();
			}
			return *this;
		}

		bool operator != (const iterator& obj) {
			return treeNodeList->front() != obj.treeNodeList->front();
		}

		bool operator == (const iterator& obj) {
			return treeNodeList->front() == obj.treeNodeList->front();
		}

		~iterator() {
			delete treeNodeList;
			treeNodeList = nullptr;
		}
	private:
		//InOrder
		void fillTheQueue(treeNode* ptr) {
			if (ptr != nullptr) {
				fillTheQueue(ptr->lchild);
				treeNodeList->emplace(ptr);
				fillTheQueue(ptr->rchild);

			}
		}
	};
	//iterator functions
	iterator begin() {
		return iterator(root);
	}
	iterator end() {
		return iterator(nullptr);
	}
	avl():root(nullptr),count(0) {}//default constructor
	~avl() {
		clearMemory(root);
	}
	bool insert(const type& data) {//insert in AVL tree
		stack<treeNode*> ancestors;
		if (getAccess(data, ancestors)) {
			return false;
		}
		else {
			treeNode* pptr = ancestors.size() == 0 ? nullptr : ancestors.top();
			if (pptr == nullptr) {
				root = new treeNode(data);
			}
			else if (pptr->data < data) {
				pptr->rchild =new treeNode(data);
			}
			else {
				pptr->lchild = new treeNode(data);
			}
			while (!ancestors.empty())
			{
				treeNode* anc = ancestors.top();
				ancestors.pop();
				updateHeight(anc);//updating height according to balance factor
				if (bf(anc) > 1 || bf(anc) < -1) {
					treeNode* panc = ancestors.empty() ? nullptr : ancestors.top();
					if (bf(anc) == 2 && bf(anc->rchild) == 1) {
						rl(anc, panc);
					}
					else if(bf(anc)==-2 && bf(anc->lchild)==-1){
						rr(anc, panc);
					}
					else if (bf(anc) == 2 && bf(anc->rchild) == -1) {
						rr(anc->rchild, anc);
						rl(anc, panc);
					}
					else if (bf(anc) == -2 && bf(anc->lchild) == 1) {
						rl(anc->lchild, anc);
						rr(anc, panc);
					}
					break;
				}
			}
			++count;
			return true;
		}
	}
	int getSize() {
		return count;
	}
	void print()
	{
		printInOrder(root);
	}
	bool isEmpty() {
		return count == 0;
	}
private:
	bool getAccess(const type& data, stack<treeNode*>& ancestors) {
		treeNode* ptr = root;
		while (ptr != nullptr && ptr->data != data) {
			ancestors.push(ptr);
			if (ptr->data < data) {
				ptr = ptr->rchild;
			}
			else {
				ptr = ptr->lchild;
			}
		}
		return ptr != nullptr;
	}
	bool rr(treeNode* ptr, treeNode* pptr) {//rotate right
		treeNode* cptr = ptr->lchild;
		if (cptr == nullptr) {
			return false;
		}
		else {
			if (pptr == nullptr) {//If ptr is the root ptr
				root = cptr;
			}
			else {
				if (ptr == pptr->lchild) {
					pptr->lchild = cptr;
				}
				else {
					pptr->rchild = cptr;
				}
			}
			//Linking Child Node
			ptr->lchild = cptr->rchild;
			cptr->rchild = ptr;
			updateHeight(ptr);
			updateHeight(cptr);
			return true;
		}
	}
	bool rl(treeNode* ptr, treeNode* pptr) {//rotate left
		treeNode* cptr = ptr->rchild;
		if (cptr == nullptr) {
			return false;
		}
		else {
			if (pptr == nullptr) {//If ptr is the root ptr
				root = cptr;
			}
			else {
				if (ptr == pptr->lchild) {
					pptr->lchild = cptr;
				}
				else {
					pptr->rchild = cptr;
				}
			}
			//Linking Child Node
			ptr->rchild = cptr->lchild;
			cptr->lchild = ptr;
			updateHeight(ptr);
			updateHeight(cptr);
			return true;
		}
	}
	bool updateHeight(treeNode* ptr) {//update height according to balance factor
		int old_h = height(ptr);
		int new_h = max(height(ptr->lchild), height(ptr->rchild)) + 1;
		ptr->h = new_h;
		return old_h != new_h;
	}
	int height(treeNode* ptr) {//get height
		if (ptr != nullptr) {
			return ptr->h;
		}
		else {
			return -1;
		}
	}
	int bf(treeNode* ptr) {//balance factor
		return height(ptr->rchild) - height(ptr->lchild);
	}
	void clearMemory(treeNode*& ptr) {//clearing memory (called in destructor)
		if (ptr != nullptr) {

			clearMemory(ptr->lchild);
			clearMemory(ptr->rchild);
			delete ptr;
			--count;
			ptr = nullptr;
		}
	}
	void printInOrder(treeNode* ptr) {//print
		if (ptr != nullptr) {
			printInOrder(ptr->lchild);
			cout << (ptr->data);
			printInOrder(ptr->rchild);
		}
	}
};
template<typename type>
class list {//Singlly List Class
	struct Node {//Node to hold data
		type data;
		Node* next;
	};
	Node* head;
	Node* tail;
	int count;
public:
	class iterator {//Iterator Class
		Node* itrCrnt;
	public:
		iterator() :itrCrnt(nullptr) {//default constructor
		}
		iterator(Node* objItr) :itrCrnt(objItr) {
		}
		/*iterator(const iterator& objItr) {
			itrCrnt = new Node;
			*itrCrnt = objItr;
		}*/
		~iterator() {
			itrCrnt = nullptr;
		}
		iterator operator = (const iterator& objItr) {
			this->itrCrnt = objItr.ItrCrnt;
		}
		bool operator != (const iterator& objItr)const {
			if (this->itrCrnt != objItr.itrCrnt) {
				return true;
			}
			return false;
		}
		type& operator*() {
			return itrCrnt->data;
		}
		iterator& operator ++ () {
			itrCrnt = itrCrnt->next;
			return *this;
		}
		type* operator -> () {
			return &itrCrnt->data;
		}
	};
	//constructors and destructor of list class/
	list() {//empty set
		init();//function made to initialize values
	}
	list(const list& obj) {//copy constructor 
		init();//function made to initialize values
		Node* tmpPtr = obj.head;
		while (tmpPtr != nullptr)
		{
			this->push(tmpPtr->data);
			tmpPtr = tmpPtr->next;
		}
	}
	~list() {//clean up memory
		Node* crntPtr = head;
		while (head != nullptr) {
			head = head->next;
			delete crntPtr;
			crntPtr = head;
		}
	}
	//Operators
	list operator = (const list& obj) {
		if (this != &obj) {
			this->~list();
			this->init();
			Node* tmpPtr = obj.head;
			while (tmpPtr != nullptr)
			{
				this->push(tmpPtr->data);
				tmpPtr = tmpPtr->next;
			}
		}
		return *this;
	}
	//Utility Members
	void push(type x) {
		if (head == nullptr) {//Empty List
			head = tail = new Node;
			head->data = x;
			tail->next = nullptr;
		}
		else {//Element in middle somewehere
			tail = tail->next = new Node;
			tail->data = x;
			tail->next = nullptr;
		}
		++count;
	}
	//Getters
	inline int size()const {
		return count;
	}
	inline int empty() {
		return count == 0;
	}
	//iterator functions
	iterator begin() {
		return 	iterator(head);
	}
	iterator end() {
		 return iterator(nullptr);
	}
private:
	inline void init() {//initialising values
		count = 0;
		head = tail = nullptr;
	}
};
class Doc_Info
{
	string docId;
	int tf;//term frequence
public:
	Doc_Info():docId(""),tf(-1) {}//default constructor
	//parameterized constructor
	Doc_Info(const string& docId)
	{
		this->docId=docId;
		tf=1;
	}
	//Destructor
	~Doc_Info() {}
	//Getters
	string getDocId()const {
		return docId;
	}
	int trmFreq()const {
		return tf;
	}
	void incrmTermFreq() {//Increment The Term Frequence
		++tf;
	}
};
class  Term_Info {
	string keyTerm;
	list<Doc_Info> docLst;
public:
	Term_Info() :keyTerm("") {//default constuctor
	}
	Term_Info(const string& keyTerm, const string& fileName) :keyTerm(keyTerm) {//parameterized constructor
		docLst.push(Doc_Info(fileName));
	}	
	~Term_Info() {
	}
	//getters
	string getKeyTerm() {
		return keyTerm;
	}
	list<Doc_Info> getDocLst()const {
		return docLst;
	}
	void addDocId(const string& docId) {//When same key term exist, then check document list 
		for (auto itr = docLst.begin(); itr != docLst.end(); ++itr) {
			if (itr->getDocId() == docId) {//If same docId found than add that docId to that keyterm list
				itr->incrmTermFreq();
				return;
			}
		}
		//if same document id not found
		docLst.push(Doc_Info(docId));
	}
	//OverLoaded Operators
	bool operator < (const Term_Info& obj) {
		if (this->keyTerm < obj.keyTerm) {
			return true;
		}
		return false;
	}
	bool operator > (const Term_Info& obj) {
		return !(*this<obj);
	}
	bool operator == (const Term_Info& obj) {
		if (this->keyTerm == obj.keyTerm) {
			return true;
		}
		return false;
	}
	bool operator != (const Term_Info& obj) {
		return !(*this == obj);
	}
	friend ostream& operator << (ostream& out, const Term_Info& obj) {
		out << left << obj.keyTerm;
		auto end = (obj.getDocLst()).end();
		list<Doc_Info> doc_info = obj.getDocLst();
		for (auto itr = doc_info.begin();itr!=end ; ++itr) {
			out << "  ||  {" << itr->getDocId() << "," << itr->trmFreq() << "}";
		}
		out << endl;
		return out;
	}
};
class Search_Engine
{
	avl<Term_Info> index;
public:
	Search_Engine() {}
	~Search_Engine() {}
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
		index.print();
		cout<<endl;
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
	void checkTermExits(const string& keyTerm, const string& fileName) {//Search whether keyTerm already exist or not
		for (auto itr = index.begin(); itr != index.end(); ++itr) {
			if ((*itr).getKeyTerm() == keyTerm) {//If same keyTerm found than add document id to that keyterm list
				(*itr).addDocId(fileName);
				return;
			}
		}
		//same key term not found then add that key term
		index.insert(Term_Info(keyTerm, fileName));
	}
	void addData(ifstream& dataIn, const string& fileName) {//extracting whole data from the file word by word
		string keyTerm;
		while (!dataIn.eof()) {//While File End Not Found

			dataIn >> keyTerm;//inputing word
			if (index.isEmpty()) {//If dataBase is Empty
				index.insert(Term_Info(keyTerm, fileName));
			}
			else {//Check KeyTerm Is Already Present Or Not
				checkTermExits(keyTerm, fileName);
			}
		}
		dataIn.close();
	}
	int tokenize(const string& srcStr, string*& dstStr) {
		//convert string into single dimensional word, by removing unnecessary elements

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
			for (auto itr = index.begin(); itr != index.end() && !found; ++itr) {
				if ((*itr).getKeyTerm() == wrdStr[i]) {//if same keyterm found, then store an iterator of that doc list
					docLst.push((*itr).getDocLst());
					found = true;
				}
			}
			if (found == false) {//if Word Not Found
				cout << "[INFO] || Word  " << wrdStr[i] << " Not Found In DataBase" << endl;
			}
		}
		if (!docLst.empty()) {//If No Similar KeyTerms Found In our DataBase
			struct mySet
			{
				string docId;
				int tf;//term Frequency
				int cnt;
				mySet(string docId, int tf, int cnt) :docId(docId), tf(tf), cnt(cnt) {}//initialising values
			};
			list<mySet*> rankLst;
			auto docItr = docLst.begin();
			for (int i = 0; i < docLst.size(); ++i, ++docItr) {
				auto docLstItr = docItr->begin();
				for (int j = 0; j < docItr->size(); ++j, ++docLstItr) {
					rankLst.push(new mySet(docLstItr->getDocId(), docLstItr->trmFreq(), 1));
				}
			}
			//Now Taking Union  And sorting Data According To given Condition
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
							if ((*iItr)->cnt == (*jItr)->cnt) {//If Both Have Same Query Items
								if ((*iItr)->tf == (*jItr)->tf) {//If Both Have Same Term Freqeuncy, swap alphabetically
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
			//Printing Ranked Document As Well Clearing Memory
			cout <<endl<< "DocumentName.txt" << endl;
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
	const int numFile = 5;
	string fileName[numFile] = { "Doc1.txt","Doc2.txt","Doc3.txt","Doc4.txt","Doc5.txt" };
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
	getch();
	return 0;
}