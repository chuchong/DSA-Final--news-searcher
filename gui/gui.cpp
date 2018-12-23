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
#define PROMOTESIZE 10

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
	bufSearchList = new DocWeightList();
	bufPromoteList = new DocWeightList();

	connect(this->ui.searchBtn,&QPushButton::clicked, this, &gui::search);
	connect(this->ui.searchPrev, &QPushButton::clicked, this, &gui::prevSearch);
	connect(this->ui.searchNext, &QPushButton::clicked, this, &gui::nextSearch);
	connect(this->ui.ResultList, &QListWidget::itemDoubleClicked, this, &gui::searchResult);
	connect(this->ui.promoteList, &QListWidget::itemDoubleClicked, this, &gui::promoteResult);

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

void gui::showPromoteResult(int begin, int end)
{
	ui.promoteList->clear();
	auto iter = bufPromoteList->getHead();
	int cnt = 0;
	while (iter != nullptr && cnt < bufPromoteSize && cnt < begin) {
		iter = iter->next;
		cnt++;
	}

	while (iter != nullptr && cnt < bufPromoteSize && cnt < end) {
		int id = iter->id;
		bool flag = 1;
		auto searchIter = bufSearchList->getHead();
		int searchCnt = 0;
		while (searchIter != nullptr && searchCnt < PAGESIZE) {
			if (searchIter->id == id)
				flag = 0;

			searchCnt++;
			searchIter = searchIter->next;
		}

		if (flag == 1) {
			CharString s = invertDoc->getTitle(id);

			QString qstr = WString2Qstring(s.to_wstring());
			ui.promoteList->addItem(qstr);
			cnt++;
		}
		iter = iter->next;
	}
}

void gui::showPromoteResult(int page)
{
	showPromoteResult(page * PROMOTESIZE, page * PROMOTESIZE + PROMOTESIZE);
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

void gui::openNewsBox(int id)
{
	QString title = WString2Qstring(invertDoc->getTitle(id).to_wstring());
	CharString str(QString2WString(title));
	Promoter promoter(str, *devider, *searcher);
	DocWeightList wlist;
	promoter.PromoteNews(*invertDoc, 781, wlist);
	DocNode * promote_iter = wlist.getHead();
	
	int cnt = 0;
	QVector<int>ids;
	QVector<QString> titles;
	while (promote_iter != nullptr &&cnt < 10) {
		
		if (promote_iter->id != id) {
			ids.push_back(promote_iter->id);
			CharString tit = invertDoc->getTitle(promote_iter->id);
			QString qtit = WString2Qstring(tit.to_wstring());
			titles.push_back(qtit);
			cnt++;
		}
		else
			cnt = cnt;
		promote_iter = promote_iter->next;
	}

	NewsDialog* box = new NewsDialog(id, titles, ids, this);
	connect(box, &NewsDialog::openPromote, this, &gui::openNewsBox);
	box->show();
}

void gui::searchResult(QListWidgetItem *)
{
	int row = ui.ResultList->currentRow();


	int cnt = 0;
	auto iter = bufSearchList->getHead();
	while (iter != nullptr && cnt < bufSearchSize && cnt < row + seachPage * PAGESIZE) {
		iter = iter->next;
		cnt++;
	}

	int id = iter->id;
	openNewsBox(id);
}

void gui::promoteResult(QListWidgetItem *){
	int row = ui.promoteList->currentRow();


	auto iter = bufPromoteList->getHead();
	int id = iter->id;
	bool flag = 1;
	int cnt = 0;
	while (iter != nullptr && cnt < bufPromoteSize && cnt < row) {
		int id = iter->id;
		bool flag = 1;
		auto searchIter = bufSearchList->getHead();
		int searchCnt = 0;
		while (searchIter != nullptr && searchCnt < PAGESIZE) {
			if (searchIter->id == id)
				flag = 0;

			searchCnt++;
			searchIter = searchIter->next;
		}

		if (flag == 1) {
			cnt++;
		}
		iter = iter->next;
	}
	id = iter->id;
	openNewsBox(id);
}

//void gui::nextPromote()
//{
//	if (promotePage + 1 > bufSearchSize / PAGESIZE)
//		return;
//	promotePage++;
//	showSearchResult(promotePage);
//}
//
//void gui::prevPromote()
//{
//	if (promotePage - 1 < 0)
//		return;
//	promotePage--;
//	showSearchResult(promotePage);
//}

void gui::search()
{
	QString str = ui.SearchEdit->text();
	//ui.ResultList->addItem(str);

	std::wstring wstr = QString2WString(str);

	CharString s(wstr);
	DocList list;
	Promoter promoter(s, *devider, *searcher);
	CharStringLink link;


	promoter.SearchNews(*invertDoc, 781, *renewSearchList());
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

	bufPromoteSize = 0;
	//promotePage = 0;
	promoter.PromoteNews(*invertDoc, 781, *renewPromoteList());
	auto promote_iter = bufPromoteList->getHead();
	while (promote_iter != nullptr) {
		bufPromoteSize++;
		promote_iter = promote_iter->next;
	}

	showSearchResult(seachPage);
	showPromoteResult(promotePage);

	
}
