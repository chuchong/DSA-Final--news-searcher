#pragma once
//��.h���ṩ�洢String�����ݽṹCollection �Լ���Collection������Iterator
//�Ѿ�ʵ����˫������,��������ΪCharStringLink
//��ʵ��������collection�ṹ,�����collection�Ƴ�ȥ
#include <string>
#include <fstream>

#include <ostream>
#include <locale>
#include <codecvt>
#include <assert.h>
#include "CharString.h"
class Iterator {
public:
	virtual Iterator& operator++ () = 0;
	virtual const Iterator & operator++ (int ) = 0;
	virtual CharString* operator*() = 0;
	virtual bool operator==(Iterator&) = 0;
	virtual ~Iterator() {}
};
class CharStringCollection {
public:
	virtual void add(CharString *str) = 0;
	virtual void remove(CharString *str) = 0;
	virtual bool search(CharString *str) = 0;
	virtual bool isEmpty() = 0;
	virtual Iterator * begin() = 0;
	virtual Iterator * end() = 0;
	virtual ~CharStringCollection() {}
};
//------------------------����Ϊ˫������ʵ��
struct ListNode {
	CharString *str_ = nullptr;
	ListNode * next_ = nullptr;
	ListNode * prior_ = nullptr;
	ListNode(CharString * str);
	ListNode() {
	}
	~ListNode() {
		delete str_;
	}
};
class CharStringLink;
//�Ƽ�ʹ��Iterator����StringLink�еı���
class ListIterator : public Iterator {
private:
	CharStringLink * checker;
	ListNode * node;
	friend class CharStringLink;
public:
	virtual const Iterator & operator++ (int) {
		ListIterator tmp = *this;
		node = node->next_;
		return tmp;
	}
	virtual Iterator& operator++ () {
		node = node->next_;
		return *this;
	}
	virtual CharString* operator*() {
		return node->str_;
	}
	virtual bool operator==(Iterator &other) {
		return (node == dynamic_cast<ListIterator*>(&other)->node);
	}
	ListIterator(ListNode * n) :node(n) {

	}
	virtual ~ListIterator() {}
};

class CharStringLink : public CharStringCollection {
	ListNode * head_ = nullptr;
	ListNode * last_ = nullptr;
public:
	inline void print(std::wofstream &os){
		ListNode * node = head_;
		while (node != nullptr) {
			os << *node->str_;
			os << " ";
			node = node->next_;
		}
	}
	//delete iterator ָ��� node,ע��:!!iter��ʧЧ 
	virtual void remove(ListIterator * iter);
	virtual Iterator * begin() {
		return new ListIterator(head_);
	}
	virtual Iterator * end() {
		return new ListIterator(nullptr);
	}
	virtual bool isEmpty() override {
		return head_ == nullptr;
	}

	void add(const std::wstring & str);
	void add(CharString * str);
	//����ɾ��ĳ��ͬ���ַ����ĵ�һ��ƥ����
	virtual void remove(CharString *str);
	virtual bool search(CharString *str) override;
	void output(CharString * place);
	virtual ~CharStringLink();
};

