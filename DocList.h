#pragma once
#include"Doc.h"
#include <iostream>

struct DocNode {
	int id;
	DocNode* next = nullptr;
	DocNode* prior = nullptr;
	int cnt;

	DocNode(int i, int cn) :id(i), cnt(cn) {}
};
class DocList
{
	DocNode *head = nullptr;
	DocNode *last = nullptr;
	void qSort(DocNode * left, DocNode * right);
	DocNode *Partion(DocNode * left, DocNode * right);
	inline void swap(DocNode * i, DocNode *j) {
		int cnt = i->cnt;
		int id = i->id;
		i->id = j->id;
		i->cnt = j->cnt;
		j->id = id;
		j->cnt = cnt;
	}
public:
	//ÓÐÐòÌí¼Ó
	void Add(int id, int cnt);
	int Search(int id);
	void Edit(int id, int to_cnt);
	void Remove(int id);

	void qSort();

	void PushBack(int id, int cnt);
	void print() {
		auto i = head;
		while (i != nullptr) {
			std::cout << "(" << i->id << "," << i->cnt << ")" << " ";
			i = i->next;
		}
		std::cout << std::endl;
	}

	DocList();
	~DocList();
};

