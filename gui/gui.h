#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_gui.h"
#include "../Promoter.h"
#include "../Devider.h"
#include "../Searcher.h"
#include "../CharStringLink.h"
#include "../CharString.h"
#include "../InvertDoc.h"
#include <qtextcodec.h>
#include <qbytearray.h>
#include<codecvt>
#include "NewsDialog.h"
#include "../Synonyms.h"

class gui : public QMainWindow
{
	Q_OBJECT
	//以下是全局变量，不需要delete
	Searcher * searcher;
	Devider * devider;
	InvertDoc * invertDoc;
	Synonyms * syn;
	//search xiangguan
	DocWeightList *bufSearchList = nullptr;
	int bufSearchSize = 0;
	int seachPage = 0;
	QVector<QString> keywords;

	QVector<int> promoteIds;
	DocWeightList *bufPromoteList = nullptr;
	int bufPromoteSize = 0;
	const int promotePage = 0;

private:
	bool containWandSyn(CharString & str,const QString & contents) {
		if (contents.contains(WString2Qstring(str.to_wstring())))
			return true;
		std::vector<CharString>* syns = syn->search(str);
		if (!syns)
			return false;
		for (CharString & words : *syns) {
			if (contents.contains(WString2Qstring(words.to_wstring())))
				return true;
		}
		return false;
	}

	void convertWandSyns(CharString & str, QString & contents) {
		if (contents.contains(WString2Qstring(str.to_wstring()))) {
			QString qstr = WString2Qstring(str.to_wstring());
			contents.replace(qstr, "<font color=red>" + qstr + "</font>");
			return;
		}
		else {
			std::vector<CharString>* syns = syn->search(str);
			if (!syns)
				return;
			for (CharString & words : *syns) {
				if (contents.contains(WString2Qstring(words.to_wstring()))) {
					QString qstr = WString2Qstring(words.to_wstring());
					contents.replace(qstr, "<font color=red>" + qstr + "</font>");
					return;
				}
			}
			return;
		}
	}

	DocWeightList * renewSearchList() {
		if (bufSearchList != nullptr)
			delete bufSearchList;
		bufSearchList = new DocWeightList();
		return bufSearchList;
	}

	DocWeightList * renewPromoteList() {
		if (bufPromoteList != nullptr)
			delete bufPromoteList;
		bufPromoteList = new DocWeightList();
		return bufPromoteList;
	}
	std::wstring QString2WString(QString str);
	QString WString2Qstring(std::wstring wstr);

public:
	gui(QWidget *parent = Q_NULLPTR);
	void showSearchResult(int begin, int end);
	void showSearchResult(int page);

	void showPromoteResult(int begin, int end);
	void showPromoteResult(int page);

public slots:
	void search();
	
	void nextSearch();
	void prevSearch();

	void clickSheet(QListWidgetItem *);

	void openNewsBox(int id);
	void showKeyWords(int id);

	void searchResult(QListWidgetItem *);
	void promoteResult(QListWidgetItem *);
	//void nextPromote();
	//void prevPromote();
	//void deleteDialog(NewsDialog * d) {
	//	delete d;
	//}

private:
	Ui::guiClass ui;
};
