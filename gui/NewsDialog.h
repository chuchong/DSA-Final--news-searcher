#pragma once

#include <QDialog>
#include "ui_NewsDialog.h"

class NewsDialog : public QDialog
{
	Q_OBJECT

public:
	NewsDialog(QWidget *parent = Q_NULLPTR);
	NewsDialog(int id, QWidget *parent = Q_NULLPTR);
	~NewsDialog();

private:
	Ui::NewsDialog ui;
};
