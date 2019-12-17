#pragma once
#include"Doc.h"
#include "DocList.h"
#include "AVLTree.h"
#include "CharString.h"
#include "CharStringLink.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include "HashTable.h"
class InvertDoc
{
	CharString titles[781];
	int total_words[781];
	AVLMap<CharString, DocList> content_map;

	AVLMap<CharString, DocList> title_map;
	//HashString2Map<DocList>  content_map;
	//HashString2Map<DocList>  title_map;

public:
	//int Find(CharString & s) {
	//	return map[s].Search();
	//}

	CharString & getTitle(int id) {
		return titles[id];
	}

	int getTotalWords(int id) {
		return total_words[id];
	}

	void printQuery(const CharString & str, std::wofstream & os) {
		auto a = content_map[str];
		a.print(os);
	}

	DocList * getDocList(CharString&str) {
		if (content_map.find(str))
			return &content_map[str];
		else
			return nullptr;
	}

	DocList * getTitleDocList(CharString& str) {
		if (title_map.find(str))
			return &title_map[str];
		else
			return nullptr;
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

	void intersectTwoDocList(const DocList& d1, const DocList& d2, DocList& d3, int maxsize) {
		for (int i = 0; i <= 780; i++) {
			int a = d1.Search(i);
			int b = d2.Search(i);
			if (a && b) {
				d3.Add(i, a + b);
			}
		}
		d3.qSort();
	}

	void printQuery(CharStringLink & list, int maxsize, std::wofstream & os) {
		Iterator * a = list.begin();
		Iterator * b = list.end();
		auto iter = a;
		DocList buf(content_map[**iter]);
		(*iter)++;
		while (!(*iter == *b)) {
			DocList buf2;
			mergeTwoDocList(buf, content_map[**iter], buf2, maxsize);
			buf = buf2;
			(*iter)++;
		}
		buf.print(os);
		delete a;
		delete b;

	}

	void createList(Devider & devider, Searcher& searcher) {
		for (int i = 0; i <= 780; i++) {
			Doc d(i);


			d.parse(devider, searcher);
			Iterator* iter = d.list.begin();
			Iterator* begin = iter;
			Iterator* end = d.list.end();
			while (!(*iter == *end)) {
				CharString *str = *(*iter);
				content_map[*str].Add(i, d.cnt[*str]);
				++(*iter);
			}

			delete begin;
			delete end;

			Iterator* iter2 = d.title_list.begin();
			Iterator* begin2 = iter2;
			Iterator* end2 = d.title_list.end();
			while (!(*iter2 == *end2)) {
				CharString *str = *(*iter2);
				title_map[*str].Add(i, d.title_cnt[*str]);
				++(*iter2);
			}

			delete begin2;
			delete end2;

			titles[i] = d.title;
			total_words[i] = d.total_words;
		}
	}
	InvertDoc();
	~InvertDoc();
};

