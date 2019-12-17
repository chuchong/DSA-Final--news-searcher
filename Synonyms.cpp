#include "Synonyms.h"

//
//Synonyms::Synonyms()
//{
//}


Synonyms::Synonyms(std::string file, Devider & devider, Searcher & searcher)
{
	std::locale loc("chs");//windowsÏÂok

	std::wifstream in;
	in.imbue(loc);
	std::string url = file;
	in.open(url);

	std::wstring str;
	//std::wstring wstr;
	//wstr = conv.from_bytes(str);
	//std::wcout.imbue(loc);
	//std::wcout << wstr;
	while (getline(in, str)) {
		CharString s1(str);
		getline(in, str);
		CharString s2(str);

		CharStringLink link;
		devider.devide(&s2, &searcher, &link);

		//±éÀú
		Iterator* iter = link.begin();
		Iterator* begin = iter;
		Iterator* end = link.end();
		while (!(*iter == *end)) {
			CharString *str = *(*iter);
			if (*str == s1) {
				++(*iter);
				continue;
			}
			syno_map.operator[](s1).push_back(str);
			++(*iter);
		}

		delete begin;
		delete end;
	}
}

Synonyms::~Synonyms()
{
}
