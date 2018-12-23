#pragma once
#include"AVLTree.h"
#include"CharString.h"
#include"CharStringLink.h"
#include "Devider.h"
#include<string>
#include<locale>
#include <iostream>
#include <fstream>
#include <ostream>
#include <codecvt>
class Doc
{
public:
	int id;
	AVLMap<CharString, int> cnt;
	CharStringLink list;
	CharStringLink title_list;
	AVLMap<CharString, int> title_cnt;
	int total_words = 0;

	//文本内容之类的直接读取input，不占用缓存了
public:
	CharString title;
	void parse(Devider &, Searcher &);
	Doc(int i);
	Doc(CharString & str);
	~Doc();
};



