//由于使用了mbstowcs,需要在预处理器中添加_CRT_SECURE_NO_WARNINGS;
//需要支持c++11
//注意我将dict中的bom头删掉了
//在读取其他人文件时,还需要删掉dict的bom头
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
#include <vector>//只用在main中提取同级目录下所有文件
#include <crtdbg.h>
#include "AVLTree.h"
#include "Doc.h"
#include "DocList.h"
#include "InvertDoc.h"
#include "Promoter.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

using namespace std;
//string转为wstring
//引用 https://www.cnblogs.com/changxiaoxiao/archive/2013/05/16/3081894.html
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
//读取同级目录下所有文件
//引用 http://www.cnblogs.com/xzh1993/p/5048322.html
void getFiles(string path, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
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
//工作
void work() {
	std::vector<string> files;

	string filePath("input/");
	////获取该路径下的所有文件  
	getFiles(filePath, files);

	Devider d;
	Searcher s;
	s.initDictionary("./词库.dic");

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
	sw.initDictionary("./词库.dic");
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

	//CharStringLink list;
	//list.add(L"美元");
	//list.add(L"经济");
	//InvertDoc d;
	//d.printQuery(list, 780);
	//return;
}

void retrieve() {
	cout << "please wait" << endl;
	int wait_cnt = 0;

	Devider devider;
	Searcher searcher;
	searcher.initDictionary("./词库.dic");
	InvertDoc invertDoc;
	invertDoc.createList(devider, searcher);

	std::locale loc("chs");//windows下ok

	std::wifstream in;
	in.imbue(loc);
	std::string url = "query1.txt";
	in.open(url);

	std::wofstream os;
	os.imbue(loc);
	std::string orl = "result1.txt";
	os.open(orl);

	std::wstring str;
	while (getline(in, str)) {

		CharString s(str);
		DocList list;
		Promoter promoter(s, devider, searcher);
		CharStringLink link;

		promoter.SearchWordsOr(invertDoc, 781, list);


		if(!list.isEmpty())
			list.print(os);
		os << endl;

		wait_cnt++;
		if (wait_cnt == 200) {
			cout << ".";
			wait_cnt = 0;
		}
	}

	cout << "\n已经进行完了query1" << endl << "please wait";
	//推荐

	std::wifstream in2;
	in2.imbue(loc);
	std::string url2 = "query2.txt";
	in2.open(url2);

	std::wofstream os2;
	os2.imbue(loc);
	std::string orl2 = "result2.txt";
	os2.open(orl2);

	while (getline(in2, str)) {
		wait_cnt++;
		if (wait_cnt == 200) {
			wait_cnt = 0;
			cout << ".";
		}
		CharString s(str);
		CharStringLink link;
		Promoter promoter(s, devider, searcher);
		int similar[5];
		promoter.Similar_n(invertDoc, 781, 5, similar);
		bool flag = 1;

		for (int i = 0; i < 5; i++) {
			if (similar[i] >= 0) {
				if (!flag)
					os2 << ",";
				os2 << "(" << similar[i] << "," << invertDoc.getTitle(similar[i]) << ")";
				flag = 0;
			}
		}
		if (flag == 1)
			os2 << L"系统未匹配到和该新闻相似新闻，抱歉!";
		os2 << std::endl;
	}
	cout << "\n已经进行完了query2" << endl;
}

void createDataset() {
	std::vector<string> files;

	string filePath("dataset/");
	////获取该路径下的所有文件  
	getFiles(filePath, files);

	Devider d;
	Searcher se;
	se.initDictionary("./词库.dic");
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::locale loc("chs");//windows下ok

	for (string file : files) {
		std::ifstream in;
		in.imbue(loc);
		in.open(file);

		std::wofstream os;
		os.imbue(loc);
		os.open(file.substr(9,11)+"data");

		std::string str;
		std::wstring wstr;
		wstr = conv.from_bytes(str);

		while (getline(in, str)) {
			wstr = conv.from_bytes(str);
			CharString s(wstr);
			
			CharStringLink link;
			d.devide(&s, &se, &link);
			link.print(os);
			os << endl;
		}
	}

}

int main() {

	//_CrtSetBreakAlloc(2804683);
	//EnableMemLeakCheck();
	//testAVL();
	//testChar();
	//testMap();
	//work();
	//testDict();

	//
	retrieve();
	//testDoc();
	//createDataset();
	_CrtDumpMemoryLeaks();
	return 0;
}