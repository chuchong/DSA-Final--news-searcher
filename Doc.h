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
	//�ı�����֮���ֱ�Ӷ�ȡinput����ռ�û�����

	void parse();
public:
	Doc(int i);
	~Doc();
};

