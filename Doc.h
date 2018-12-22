#pragma once
#include"AVLTree.h"
#include"CharString.h"
#include"CharStringLink.h"
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
	//文本内容之类的直接读取input，不占用缓存了

	void parse();
public:
	Doc(int i);
	~Doc();
};

