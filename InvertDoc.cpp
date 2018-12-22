#include "InvertDoc.h"



InvertDoc::InvertDoc()
{
	for (int i = 0; i <= 780; i++) {
		Doc d(i);
		Iterator* iter = d.list.begin();
		Iterator* begin = iter;
		Iterator* end = d.list.end();
		while (!(*iter == *end)) {
			CharString *str =  *(*iter);
			map[*str].Add(i, d.cnt[*str]);
			++(*iter);
		}

		delete begin;
		delete end;
	}
}


InvertDoc::~InvertDoc()
{
}
