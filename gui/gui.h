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

class gui : public QMainWindow
{
	Q_OBJECT
	//以下是全局变量，不需要delete
	Searcher * searcher;
	Devider * devider;
	InvertDoc * invertDoc;
	//search xiangguan
	DocWeightList *bufSearchList = nullptr;
	int bufSearchSize = 0;
	int seachPage = 0;
	QMap<QLabel*, int> label2id;

	DocWeightList *bufPromoteList = nullptr;
	int bufPromoteSize = 0;
	const int promotePage = 0;

private:
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


	void openNewsBox(int id);
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
