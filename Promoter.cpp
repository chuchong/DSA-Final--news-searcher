#include "Promoter.h"



Promoter::Promoter(CharString & str, Devider &devider, Searcher &searcher)
{
	content = str;
	Devide(devider, searcher);
}


Promoter::~Promoter()
{
}
