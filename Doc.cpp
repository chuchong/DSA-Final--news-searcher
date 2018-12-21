#include "Doc.h"



void Doc::parse()
{
	//std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::locale loc("chs");//windowsÏÂok

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
	}
}

Doc::Doc(int i):id(i)
{
	parse();
}


Doc::~Doc()
{
}
