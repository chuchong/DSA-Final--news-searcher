#pragma once
#include "AVLTree.h"
#include "CharString.h"
#include "Devider.h"
#include "CharStringLink.h"
#include "Searcher.h"
#include "InvertDoc.h"
#include "DocList.h"

class Promoter
{
	CharString content;
	CharStringLink buflink;
	AVLMap<CharString, int>cnt;
public:
	Promoter(CharString & str, Devider &devider, Searcher &searcher);
	void Devide(Devider &devider, Searcher &searcher) {
		CharStringLink list;
		devider.devide(&content, &searcher, &list);

		Iterator * a = list.begin();
		Iterator * b = list.end();
		auto iter = a;
		while (!(*iter == *b)) {
			if (!cnt.find(**iter)) {
				cnt[***iter] = 1;
				buflink.add(**iter);
			}
			else
				cnt[**iter]++;
			(*iter)++;
		}
		delete a;
		delete b;
	}

	//in this case 781 similar -1means no result
	void Similar_n(InvertDoc &indoc, int id_cnt, int n, int similar[]) {
		double *weights = new double[id_cnt];
		for (int i = 0; i < id_cnt; i++)
			weights[i] = -1;

		Iterator * a = buflink.begin();
		Iterator * b = buflink.end();
		auto p_iter = a;


		while (!(*p_iter == *b)) {
			DocList* list = indoc.getDocList(***p_iter);

			int wcnt = cnt[***p_iter];

			if (list != nullptr) {
				auto doc = list->getHead();
				while (doc != nullptr) {
					int id = doc->id;
					int doc_wcnt = doc->cnt;

					if (indoc.getTotalWords(id) != 0)
						weights[id] += (doc_wcnt * wcnt * 20 / (indoc.getTotalWords(id)));
					doc = doc->next;
				}
			}


			DocList* title_list = indoc.getTitleDocList(***p_iter);
			if (title_list != nullptr) {
				auto title_doc = title_list->getHead();
				while (title_doc != nullptr) {
					int id = title_doc->id;
					int doc_wcnt = title_doc->cnt;

					if (indoc.getTotalWords(id) != 0)
						weights[id] += (doc_wcnt * wcnt);
					title_doc = title_doc->next;
				}
			}

			(*p_iter)++;
		}

		delete a;
		delete b;

		for (int i = 0; i < n; i++) {
			int n_id = -1;
			double now_weight = -1;
			for (int j = 0; j < id_cnt; j++) {
				bool has_shown = 0;
				for (int k = 0; k < i; k++) {
					if (j == similar[k])
						has_shown = 1;
				}
				if (weights[j] > now_weight && !has_shown) {
					n_id = j;
					now_weight = weights[j];
				}
			}
			similar[i] = n_id;
		}

		delete[] weights;
	}
	
	void Search_words(InvertDoc &indoc, int id_cnt, int n, ) {
		double *weights = new double[id_cnt];
		for (int i = 0; i < id_cnt; i++)
			weights[i] = -1;

		Iterator * a = buflink.begin();
		Iterator * b = buflink.end();
		auto p_iter = a;


		while (!(*p_iter == *b)) {
			DocList* list = indoc.getDocList(***p_iter);

			int wcnt = cnt[***p_iter];

			if (list != nullptr) {
				auto doc = list->getHead();
				while (doc != nullptr) {
					int id = doc->id;
					int doc_wcnt = doc->cnt;

					if (indoc.getTotalWords(id) != 0)
						weights[id] += (doc_wcnt * wcnt * 20 / (indoc.getTotalWords(id)));
					doc = doc->next;
				}
			}


			DocList* title_list = indoc.getTitleDocList(***p_iter);
			if (title_list != nullptr) {
				auto title_doc = title_list->getHead();
				while (title_doc != nullptr) {
					int id = title_doc->id;
					int doc_wcnt = title_doc->cnt;

					if (indoc.getTotalWords(id) != 0)
						weights[id] += (doc_wcnt * wcnt);
					title_doc = title_doc->next;
				}
			}

			(*p_iter)++;
		}

		delete a;
		delete b;

		for (int i = 0; i < n; i++) {
			int n_id = -1;
			double now_weight = -1;
			for (int j = 0; j < id_cnt; j++) {
				bool has_shown = 0;
				for (int k = 0; k < i; k++) {
					if (j == similar[k])
						has_shown = 1;
				}
				if (weights[j] > now_weight && !has_shown) {
					n_id = j;
					now_weight = weights[j];
				}
			}
			similar[i] = n_id;
		}

		delete[] weights;
	}
	~Promoter();
};
