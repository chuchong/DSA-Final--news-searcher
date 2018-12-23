#include "Doc.h"



void Doc::parse(Devider &devider, Searcher &searcher)
{
	//std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::locale loc("chs");//windows下ok

	std::wifstream in;
	in.imbue(loc);
	std::string url = "output/" + std::to_string(id) + ".txt";
	in.open(url);

	std::wstring str;
	//std::wstring wstr;
	//wstr = conv.from_bytes(str);
	//std::wcout.imbue(loc);
	//std::wcout << wstr;
	while (getline(in, str)) {
		CharString s(str);
		if(!this->cnt.find(s)) {
			cnt.operator[](s) = 1;
			list.add(&s);
			continue;
		}
		else
			cnt.operator[](s)++;

		total_words++;
	}
	//上面是正文
	//下面是标题
	url = "output/" + std::to_string(id) + ".info";

	std::wifstream in2;
	in2.imbue(loc);
	in2.open(url);
	
	getline(in2, str);
	title = str;

	CharStringLink link;
	devider.devide(&title, &searcher, &link);

	//遍历
	Iterator* iter = link.begin();
	Iterator* begin = iter;
	Iterator* end = link.end();
	while (!(*iter == *end)) {
		CharString *str = *(*iter);


		if (!this->title_cnt.find(*str)) {
			title_cnt.operator[](*str) = 1;
			title_list.add(str);
			continue;
		}
		else
			title_cnt.operator[](*str)++;

		++(*iter);
	}

	delete begin;
	delete end;
}

Doc::Doc(int i):id(i)
{
}

Doc::Doc(CharString & str)
{
	this->title = str;
}


Doc::~Doc()
{
}
