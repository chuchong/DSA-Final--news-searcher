#pragma once

#include <QWidget>
#include "ui_NewsBox.h"

class NewsBox : public QWidget
{
	Q_OBJECT

public:
	NewsBox(QWidget *parent = Q_NULLPTR);
	~NewsBox();

private:
	Ui::NewsBox ui;
};
