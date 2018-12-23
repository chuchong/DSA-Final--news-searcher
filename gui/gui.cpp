#include "gui.h"
#include "../Promoter.h"
#include "../Devider.h"
#include "../Searcher.h"
#include "../CharStringLink.h"
#include "../CharString.h"
#include "../InvertDoc.h"
#include <qtextcodec.h>
#include <qbytearray.h>
#include<codecvt>
#define PAGESIZE 10

std::wstring gui::QString2WString(QString str)
{
	QTextCodec *gbk = QTextCodec::codecForName("GBK");

	std::wstring wstr = gbk->toUnicode(str.toLocal8Bit()).toStdWString();
	return wstr;
}

QString gui::WString2Qstring(std::wstring wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
	std::string str = strCnv.to_bytes(wstr);
	return QString::fromUtf8(str.c_str());
}

gui::gui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	searcher = new Searcher();
	searcher->initDictionary("./词库.dic");
	devider = new Devider();
	invertDoc = new InvertDoc();
	invertDoc->createList(*devider, *searcher);
	bufSearchList = new DocList();

	connect(this->ui.searchBtn,&QPushButton::clicked, this, &gui::search);
	connect(this->ui.searchPrev, &QPushButton::clicked, this, &gui::prevSearch);
	connect(this->ui.searchNext, &QPushButton::clicked, this, &gui::nextSearch);

}

void gui::showSearchResult(int begin, int end)
{
	ui.ResultList->clear();
	auto iter = bufSearchList->getHead();
	int cnt = 0;
	while (iter != nullptr && cnt < bufSearchSize && cnt < begin) {
		iter = iter->next;
		cnt++;
	}

	while (iter != nullptr && cnt < bufSearchSize && cnt < end) {
		int id = iter->id;
		CharString s = invertDoc->getTitle(id);

		QString qstr = WString2Qstring(s.to_wstring());
		ui.ResultList->addItem(qstr);

		iter = iter->next;
		cnt++;
	}
}

inline void gui::showSearchResult(int page) {

	showSearchResult(page * PAGESIZE, page * PAGESIZE + PAGESIZE);
}

void gui::nextSearch()
{
	if (seachPage + 1 > bufSearchSize / PAGESIZE)
		return;
	seachPage++;
	showSearchResult(seachPage);
}

void gui::prevSearch()
{
	if (seachPage - 1 < 0)
		return;
	seachPage--;
	showSearchResult(seachPage);
}

void gui::search()
{
	QString str = ui.SearchEdit->text();
	//ui.ResultList->addItem(str);

	std::wstring wstr = QString2WString(str);

	CharString s(wstr);
	DocList list;
	Promoter promoter(s, *devider, *searcher);
	CharStringLink link;
	promoter.SearchWordsOr(*invertDoc, 781, *renewSearchList());
	
	bufSearchSize = 0;
	seachPage = 0;
	auto iter = bufSearchList->getHead();
	while (iter != nullptr) {
		bufSearchSize++;
		iter = iter->next;
	}

	std::wstring num_r = L"搜索结果共" + std::to_wstring(bufSearchSize) + L"条";
	ui.searchNum->setText(WString2Qstring(num_r));

	showSearchResult(seachPage);
	//推荐
}
