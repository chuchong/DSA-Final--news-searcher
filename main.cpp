//����ʹ����mbstowcs,��Ҫ��Ԥ�����������_CRT_SECURE_NO_WARNINGS;
//��Ҫ֧��c++11
//ע���ҽ�dict�е�bomͷɾ����
//�ڶ�ȡ�������ļ�ʱ,����Ҫɾ��dict��bomͷ
#include "Stack.h"
#include "CharString.h"
#include "CharStringLink.h"
#include "Dict.h"
#include "Searcher.h"
#include "HtmlParser.h"
#include "Devider.h"
#include "HtmlFilter.h"
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <locale>
#include <codecvt>
#include <io.h>
#include <vector>//ֻ����main����ȡͬ��Ŀ¼�������ļ�
#include <crtdbg.h>
#include "AVLTree.h"
#include "Doc.h"
#include "DocList.h"
#include "InvertDoc.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

using namespace std;
//stringתΪwstring
//���� https://www.cnblogs.com/changxiaoxiao/archive/2013/05/16/3081894.html
std::wstring s2ws(const string& s)
{
	setlocale(LC_ALL, "chs");

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	wstring result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, "C");

	return result;
}
//��ȡͬ��Ŀ¼�������ļ�
//���� http://www.cnblogs.com/xzh1993/p/5048322.html
void getFiles(string path, vector<string>& files)
{
	//�ļ����  
	long   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮  
			//�������,�����б�  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
//����
void work() {
	std::vector<string> files;

	string filePath("input/");
	////��ȡ��·���µ������ļ�  
	getFiles(filePath, files);

	Devider d;
	Searcher s;
	s.initDictionary("./�ʿ�.dic");

	for (string file : files) {
		std::wstring  wide = s2ws(file);
		CharString str(wide);
		if (str.indexOf(L".html",0) == -1)
			continue;

		int left = str.indexOf(L"\\", 0) + 1;
		int right = str.indexOf(L".html", 0);
		CharStringLink l;
		HtmlParser parser;

		CharString * sub = str.subString(left, right);
		CharString out(L"./output/");
		out.concat(sub);
		delete sub;
		parser.setDoc(&out);
		std::locale loc("chs");
		std::wcout.imbue(loc);

		std::wcout << "---------------------------" << str << "---------------------" << endl;
		parser.parse(wide);
		parser.output();

		//HtmlFilter fil;
		//fil.searchTree(parser.getDocNode());
		d.devide(parser.getText(), &s, &l);
		l.output(parser.getDoc());
		std::wcout << "---------------------------" <<str<< "---------------------" << endl;
	}

}

void testFilter() {

}

void testAVL() {
	AVLTree<int> t;

	for (int i = 0; i <= 100; i++)
		t.Insert(i);

	for (int i = 0; i <= 150; i++) {
		auto k = t.Search(i);
		if (k)
			cout << *k;
	}

	for (int i = 0; i <= 150; i++) {
		if (i == 30)
			i = 30;

		t.Remove(i/2);
	}
	return;

}

void testChar() {
	CharString a(L"123");
	CharString b(L"123");
	cout << (a < b) << (a == b) << (a > b) << endl;
	a = L"1234";
	cout << (a < b) << (a == b) << (a > b) << endl;
	a = L"12";
	cout << (a < b) << (a == b) << (a > b) << endl;
	a = L"234";
	cout << (a < b) << (a == b) << (a > b) << endl;
}

void testMap() {
	AVLMap<int, CharString> map;
	for (int i = 0; i < 1; i++) {
		map[i] = L"s";
		map.deleteX(i);
	}
}
void testDict() {
	Searcher sw;
	sw.initDictionary("./�ʿ�.dic");
	HashDict d(3);
	CharString s(L"L");
	d.addCopyOf(&s);
}

void testDoc() {
	//std::vector<Doc*> vd;
	//for (int i = 0; i < 781; i++) {
	//	Doc* d = new Doc(i);
	//	vd.push_back(d);
	//}

	CharStringLink list;
	list.add(L"��Ԫ");
	list.add(L"����");
	InvertDoc d;
	d.printQuery(list, 780);
	return;
}

int main() {

	//_CrtSetBreakAlloc(2407877);
	//EnableMemLeakCheck();
	//testAVL();
	//testChar();
	//testMap();
	//work();
	//testDict();
	//_CrtDumpMemoryLeaks();
	//
	testDoc();
	return 0;
}