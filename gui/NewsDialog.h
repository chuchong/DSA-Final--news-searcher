#pragma once

#include <QDialog>
#include "ui_NewsDialog.h"
#include "../Promoter.h"
#include "../Devider.h"
#include "../Searcher.h"
#include "../CharStringLink.h"
#include "../CharString.h"
#include "../InvertDoc.h"
#include <qlist.h>
class NewsDialog : public QDialog
{
	Q_OBJECT
	std::wstring QString2WString(QString str);
	QString WString2Qstring(std::wstring wstr);
	QVector<QString> titles;
	QVector<int> ids;
signals:
	void openPromote(int);
public:
	NewsDialog(int id, QVector<QString> ,QVector<int>, QWidget *parent = Q_NULLPTR);
	~NewsDialog();
public slots:
	void onPromote(QListWidgetItem *);

private:
	Ui::NewsDialog ui;
};
