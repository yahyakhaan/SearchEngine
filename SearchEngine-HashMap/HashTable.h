#pragma once
#include"list.h"
#include<string>
#include<cassert>
#include<iostream>
#define PRIME 5
using namespace std;
//HashMap Using Chaining
template<typename keyType, typename dataType>
class HashTable {
	struct  HashItem
	{
		keyType key;
		dataType data;
		HashItem() {}
		HashItem(keyType key, dataType data) :key(key), data(data) {}
	};
	int maxSize;
	int tableSize;
	list<HashItem>* table;
public:
	HashTable(int maxSize = 10) {
		assert(maxSize > 0);//If user pass capacity less than 1
		intialize(maxSize);
	}
	~HashTable() {

		delete[] table;
		table = nullptr;
	}
	void insert(keyType key, dataType data) {
		if (loadFactor()) {
			doubleCapcaity();
		}
		int idx = abs(hash(key));
		table[idx].insert(HashItem(key, data));
		++tableSize;
	}
	void print() {
		for (int i = 0; i < maxSize; ++i) {
			for (auto itr = table[i].begin(); itr != table[i].end(); ++itr) {
				cout << (*itr).data;
			}
		}
	}
	bool deleteKey(keyType key) {
		int idx = abs(hash(key));
		auto itr = searchKey(key, idx);
		if (itr != table[idx].end()) {
			table[idx].erase(itr);
			--tableSize;
			return true;
		}
		return false;
	}
	dataType& get(keyType key) {
		int idx = abs(hash(key));
		auto itr = searchKey(key, idx);
		if (itr != table[idx].end()) {
			return itr->data;
		}
		assert(true);
	}
	bool find(keyType key) {
		int idx = abs(hash(key));
		auto itr = searchKey(key, idx);
		return itr != table[idx].end() ? true : false;
	}
	inline int size() {
		return tableSize;
	}
	inline int capacity() {
		return maxSize;
	}
	dataType& operator [] (keyType key) {
		return get(key);
	}
private:
	int hash(int key) {
		return key % maxSize;
	}
	int hash(string str) {
		int idx = 0;
		int hashVal = 5381;
		while (str[idx]) {
			hashVal = ((hashVal << PRIME) + hashVal) + str[idx++];
		}
		return hash(hashVal);
	}
	inline bool loadFactor() {
		return ((tableSize / maxSize) * 100) >= 70 ? true : false;
	}
	void intialize(int maxSize) {
		this->maxSize = maxSize;
		tableSize = 0;
		table = new list<HashItem>[maxSize];
	}
	void reHash(list<HashItem>& obj) {
		for (auto itr = obj.begin(); itr != obj.end(); ++itr) {
			this->insert(itr->key, itr->data);
		}
	}
	void doubleCapcaity() {
		list<HashItem>* tempArr = table;
		intialize(maxSize * 2);
		for (int i = 0; i < (maxSize / 2); ++i) {
			reHash(tempArr[i]);
		}
	}
	typename list<HashItem>::iterator searchKey(keyType key, int idx) {
		for (auto itr = table[idx].begin(); itr != table[idx].end(); ++itr)
		{
			if (itr->key == key) {
				return itr;
			}
		}
		return table[idx].end();
	}
};