#pragma once
#include"Doc.h"
#include "DocList.h"
#include "AVLTree.h"
#include "CharString.h"
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
	InvertDoc();
	~InvertDoc();
};

