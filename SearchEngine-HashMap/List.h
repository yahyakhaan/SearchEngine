#pragma once
template<typename type>
class list {//Singly List Class
	struct Node {//Node to hold data
		type data;
		Node* next;
		Node() :next(nullptr){}
		Node(type data,Node * next=nullptr):data(data),next(next){}
	};
	Node* head;
	Node* tail;
	int count;
public:
	class iterator {//Iterator Class
		Node* ptr;
		friend list;
	public:
		iterator() :ptr(nullptr) {
		}
		iterator(Node* objItr) :ptr(objItr) {
		}
		iterator(const iterator& objItr) {
			ptr = objItr.ptr;
		}
		~iterator() {
			ptr = nullptr;
		}
		iterator operator = (const iterator& objItr) {
			this->ptr = objItr.ItrCrnt;
		}
		bool operator != (const iterator& objItr) {
			if (this->ptr != objItr.ptr) {
				return true;
			}
			return false;
		}
		type& operator*() {
			return ptr->data;
		}
		iterator& operator ++ () {
			ptr = ptr->next;
			return *this;
		}
		type* operator -> () {
			return &ptr->data;
		}
	};
	/*constructors and destructor of list class*/
	list() {//empty set
		init(); //function made to initialize values
	}
	list(const list& obj) {//copy constructor 
		init();
		Node* tmpPtr = obj.head;
		while (tmpPtr != nullptr)
		{
			this->insert(tmpPtr->data);
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
			this->init();
			Node* tmpPtr = obj.head;
			while (tmpPtr != nullptr)
			{
				this->insert(tmpPtr->data);
				tmpPtr = tmpPtr->next;
			}
		}
		return *this;
	}
	//Utility Members
	void insert(type x) {
		if (head == nullptr) {//Empty List
			head = tail = new Node(x);
		}
		else {//element somewhere in middle
			tail = tail->next = new Node;
			tail->data = x;
			tail->next = nullptr;
		}
		++count;
	}
	bool erase(iterator itr) {
		Node* temp = head;
		while (head != nullptr && head->next != itr.ptr) {
			head = head->next;
		}
		if (head) {
			head->next = (itr.ptr)->next;
			delete (itr.ptr);
		}
		return true;
	}
	inline int size()const {
		return count;
	}
	inline int empty() {
		return count == 0;
	}
	iterator begin() {
		iterator tmpItr(head);
		return tmpItr;
	}
	iterator end() {
		iterator tmpItr;
		return tmpItr;
	}
private:
	inline void init() {//initializing values
		count = 0;
		head = tail = nullptr;
	}
};

