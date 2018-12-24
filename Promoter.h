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
			if (!cnt.find(***iter)) {
				cnt[***iter] = 1;
				buflink.add(**iter);
			}
			else
				cnt[***iter]++;
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
	
	void SearchWordsOr(InvertDoc &indoc, int id_cnt, DocList&list) {
		int* times = new int[id_cnt];
		for (int i = 0; i < id_cnt; i++)
			times[i] = 0;

		Iterator * a = buflink.begin();
		Iterator * b = buflink.end();
		auto p_iter = a;


		while (!(*p_iter == *b)) {
			DocList* list = indoc.getDocList(***p_iter);

			if (list != nullptr) {
				auto doc = list->getHead();
				while (doc != nullptr) {
					int id = doc->id;
					int doc_wcnt = doc->cnt;

					times[id] += doc_wcnt;
					doc = doc->next;
				}
			}

			(*p_iter)++;
		}

		delete a;
		delete b;

		for (int i = 0; i < id_cnt; i++) {
			if (times[i] != 0)
				list.Add(i, times[i]);
		}

		list.qSort();
		delete[] times;
	}

	//promoter 更看重weight而不是cnt
	void PromoteNews(InvertDoc &indoc, int id_cnt, DocWeightList&list) {
		double* weights = new double[id_cnt];
		int* match_cnts = new int[id_cnt];
		for (int i = 0; i < id_cnt; i++) {
			weights[i] = 0;
			match_cnts[i] = 0;
		}

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

					if (indoc.getTotalWords(id) != 0) {
						double ttl = indoc.getTotalWords(id) > 100 ? indoc.getTotalWords(id) : 100;
						weights[id] += (doc_wcnt * wcnt * 100 / ttl);
						match_cnts[id] ++;
					}
					doc = doc->next;
				}
			}


			DocList* title_list = indoc.getTitleDocList(***p_iter);
			if (title_list != nullptr) {
				auto title_doc = title_list->getHead();
				while (title_doc != nullptr) {
					int id = title_doc->id;
					int doc_wcnt = title_doc->cnt;

					if (indoc.getTotalWords(id) != 0) {
						weights[id] += (doc_wcnt * wcnt*2);//标题的更重要些
						if (match_cnts[id] == 0)
							match_cnts[id] ++;
					}
					title_doc = title_doc->next;
				}
			}

			(*p_iter)++;
		}

		delete a;
		delete b;

		for (int i = 0; i < id_cnt; i++) {
			if (weights[i] > 0)
				list.Add(i, 0, weights[i] * match_cnts[i]);
		}

		list.qSort();
		delete[] weights;
		delete[] match_cnts;
	}

	//search更看重匹配了多少词
	void SearchNews(InvertDoc &indoc, int id_cnt, DocWeightList&list) {
		double* weights = new double[id_cnt];
		int* match_cnts = new int[id_cnt];
		for (int i = 0; i < id_cnt; i++) {
			weights[i] = 0;
			match_cnts[i] = 0;
		}

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

					if (indoc.getTotalWords(id) != 0) {
						weights[id] += (doc_wcnt * wcnt / (double)(indoc.getTotalWords(id)));
						match_cnts[id] ++;
					}
					doc = doc->next;
				}
			}


			DocList* title_list = indoc.getTitleDocList(***p_iter);
			if (title_list != nullptr) {
				auto title_doc = title_list->getHead();
				while (title_doc != nullptr) {
					int id = title_doc->id;
					int doc_wcnt = title_doc->cnt;

					if (indoc.getTotalWords(id) != 0) {
						weights[id] += (doc_wcnt * wcnt);
						if(match_cnts[id] == 0)
							match_cnts[id] ++;
					}
					title_doc = title_doc->next;
				}
			}

			(*p_iter)++;
		}

		delete a;
		delete b;

		for (int i = 0; i < id_cnt; i++) {
			if (weights[i] > 0)
				list.Add(i, match_cnts[i], weights[i]);
		}

		list.qSort();
		delete[] weights;
		delete[] match_cnts;
	}

	~Promoter();
};



