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

class gui : public QMainWindow
{
	Q_OBJECT

	Searcher * searcher;
	Devider * devider;
	InvertDoc * invertDoc;
	//search xiangguan
	DocList *bufSearchList = nullptr;
	int bufSearchSize = 0;
	int seachPage = 0;

private:
	DocList * renewSearchList() {
		if (bufSearchList != nullptr)
			delete bufSearchList;
		bufSearchList = new DocList();
		return bufSearchList;
	}
	std::wstring QString2WString(QString str);
	QString WString2Qstring(std::wstring wstr);

public:
	gui(QWidget *parent = Q_NULLPTR);
	void showSearchResult(int begin, int end);
	void showSearchResult(int page);

public slots:
	void search();
	void nextSearch();
	void prevSearch();
private:
	Ui::guiClass ui;
};
