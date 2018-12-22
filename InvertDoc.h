#pragma once
#include"Doc.h"
#include "DocList.h"
#include "AVLTree.h"
#include "CharString.h"
#include "CharStringLink.h"
class InvertDoc
{
	AVLMap<CharString, DocList> map;
public:
	//int Find(CharString & s) {
	//	return map[s].Search();
	//}

	void printQuery(const CharString & str) {
		auto a = map[str];
		a.print();
	}

	//WARNING MAGICNUMBER
	void mergeTwoDocList(const DocList& d1, const DocList& d2, DocList& d3, int maxsize) {
		for (int i = 0; i <= 780; i++) {
			int a = d1.Search(i);
			int b = d2.Search(i);
			if (a || b) {
				d3.Add(i, a + b);
			}
		}
		d3.qSort();
	}

	void printQuery(CharStringLink & list, int maxsize) {
		Iterator * a = list.begin();
		Iterator * b = list.end();
		auto iter = a;
		DocList buf(map[**iter]);
		(*iter)++;
		while (!(*iter == *b)) {
			DocList buf2;
			mergeTwoDocList(buf, map[**iter], buf2, maxsize);
			buf = buf2;
			(*iter)++;
		}
		buf.print();
		delete a;
		delete b;

	}
	InvertDoc();
	~InvertDoc();
};

