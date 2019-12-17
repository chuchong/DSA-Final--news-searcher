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
#include<qtextdocument.h>
#include <qcursor.h>
#include <qstackedwidget.h>
#include <qlayout.h>
#include "HtmlDelegate.h"
#include <qfile.h>
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
	setWindowIcon(QIcon(QStringLiteral(":/gui/SEARCHER")));
	ui.setupUi(this);
	searcher = new Searcher();
	searcher->initDictionary("./词库.dic");
	devider = new Devider();
	invertDoc = new InvertDoc();
	invertDoc->createList(*devider, *searcher);
	bufSearchList = new DocWeightList();
	bufPromoteList = new DocWeightList();
	syn = new Synonyms("synonyms.txt", *devider, *searcher);

	connect(this->ui.searchBtn,&QPushButton::clicked, this, &gui::search);
	connect(this->ui.searchPrev, &QPushButton::clicked, this, &gui::prevSearch);
	connect(this->ui.searchNext, &QPushButton::clicked, this, &gui::nextSearch);
	connect(this->ui.ResultList, &QListWidget::itemDoubleClicked, this, &gui::searchResult);
	connect(this->ui.promoteList, &QListWidget::itemDoubleClicked, this, &gui::promoteResult);
	connect(this->ui.ResultList, &QListWidget::itemClicked, this, &gui::clickSheet);
	ui.ResultList->setItemDelegate(new HtmlDelegate);
	//ui.keywordsList->setItemDelegate(new HtmlDelegate);
	ui.keywordsList->setHtml(u8"<p><font color = red>使用方法:</font></p> <p>在搜索框中输入文字，单机'搜一搜'按钮，可以进行查询和推荐</p> <p><font color=red>单击</font>搜索结果框中的一行，会在本文本框中显示其预览信息</p> <p><font color=red>双击</font>结果框或者推荐框中的一栏，可以查看新闻具体信息</p><p>新闻页中，也可以通过双击推荐框查看推荐内容</p> <p>author:黎思宇 感谢使用！</p>");
	//ui.keywordsList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
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
		for (QString words : keywords) {
			CharString wstr = QString2WString(words);
			this->convertWandSyns(wstr, qstr);
		}

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
			promoteIds.append(id);
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

void gui::clickSheet(QListWidgetItem *)
{
	int row = ui.ResultList->currentRow();
	int cnt = 0;
	auto iter = bufSearchList->getHead();
	while (iter != nullptr && cnt < bufSearchSize && cnt < row + seachPage * PAGESIZE) {
		iter = iter->next;
		cnt++;
	}

	int id = iter->id;
	showKeyWords(id);
}

void gui::openNewsBox(int id)
{
	QString title = WString2Qstring(invertDoc->getTitle(id).to_wstring());
	CharString str(QString2WString(title));
	Promoter promoter(str, *devider, *searcher);
	DocWeightList wlist;
	promoter.PromoteNews(*invertDoc, 781, wlist, *syn);
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

	//NewsDialog dialog(id, titles, ids, this);
	//NewsDialog*box = &dialog;
	NewsDialog* box = new NewsDialog(id, titles, ids, this);
	connect(box, &NewsDialog::openPromote, this, &gui::openNewsBox);
	//connect(box, &NewsDialog::sendP, this, &gui::deleteDialog);
	box->show();
	//box->exec();
}

void gui::showKeyWords(int id)
{
	ui.keywordsList->clear();

	int size = keywords.size();
	QVector<bool> isFind;
	isFind.resize(size);
	for (bool & i : isFind)
		i = 0;

	std::string url = "output/" + std::to_string(id) + ".info";
	QString qurl = QString::fromStdString(url);
	QFile file(qurl);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QTextCodec *gbk = QTextCodec::codecForName("GBK");

	QByteArray line = file.readLine();
	//QString title = gbk->toUnicode(line);
	//ui.title->setText(title);

	line = file.readLine();
	line = file.readLine();
	QString htmlcon;
	QString contents;
	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		contents = (gbk->toUnicode(line));
		//splitString
		bool shown_new = 0;
		for (int i = 0; i < size; i++) {
			CharString wstr = QString2WString(keywords[i]);
			if (!isFind[i] && this->containWandSyn(wstr ,contents)) {
				shown_new = 1;
				isFind[i] = 1;
			}
		}
		if (shown_new) {
			for (int i = 0; i < size; i++) {
				CharString wstr = QString2WString(keywords[i]);
				this->convertWandSyns(wstr, contents);
			}
			contents.append("...<br/>");
			contents.insert(0, "...");
			htmlcon.append(contents);
		}
	}

	ui.keywordsList->setHtml(htmlcon);
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
	openNewsBox(promoteIds[row]);
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


	promoter.SearchNews(*invertDoc, 781, *renewSearchList(), *syn);
	bufSearchSize = 0;
	keywords.clear();
	seachPage = 0;
	auto iter = bufSearchList->getHead();
	while (iter != nullptr) {
		bufSearchSize++;
		iter = iter->next;
	}

	Iterator * p_begin = promoter.getWords().begin();
	Iterator* p_end = promoter.getWords().end();
	Iterator* p_i = p_begin;
	while (!(*p_i == *p_end)) {
		CharString str = ***p_i;
		this->keywords.append(WString2Qstring(str.to_wstring()));
		(*p_i)++;
	}


	delete p_begin;
	delete p_end;


	std::wstring num_r = L"搜索结果共" + std::to_wstring(bufSearchSize) + L"条";
	ui.searchNum->setText(WString2Qstring(num_r));

	showSearchResult(seachPage);
	//推荐

	promoteIds.clear();
	bufPromoteSize = 0;
	//promotePage = 0;
	promoter.PromoteNews(*invertDoc, 781, *renewPromoteList(),*syn);
	auto promote_iter = bufPromoteList->getHead();
	while (promote_iter != nullptr) {
		bufPromoteSize++;
		promote_iter = promote_iter->next;
	}

	showSearchResult(seachPage);
	showPromoteResult(promotePage);

	
}
