#pragma once
#include "AVLTree.h"
#include <vector>
#include "CharString.h"
#include "Devider.h"
#include "Searcher.h"
class Synonyms
{
	AVLMap<CharString, std::vector<CharString>> syno_map;
public:
	Synonyms(std::string file,Devider &,Searcher &);
	std::vector<CharString> * search(const CharString & in) {
		if (!syno_map.find(in))
			return nullptr;
		else
			return &syno_map[in];
	}
	~Synonyms();
};

