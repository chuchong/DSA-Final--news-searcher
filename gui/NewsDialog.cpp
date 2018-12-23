#include "NewsDialog.h"
#include "../Promoter.h"
#include "../Devider.h"
#include "../Searcher.h"
#include "../CharStringLink.h"
#include "../CharString.h"
#include "../InvertDoc.h"
#include <qtextcodec.h>
#include <qbytearray.h>
#include<codecvt>
#include <qfile.h>
#include <qlist.h>
std::wstring NewsDialog::QString2WString(QString str)
{
	QTextCodec *gbk = QTextCodec::codecForName("GBK");

	std::wstring wstr = gbk->toUnicode(str.toLocal8Bit()).toStdWString();
	return wstr;
}

QString NewsDialog::WString2Qstring(std::wstring wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
	std::string str = strCnv.to_bytes(wstr);
	return QString::fromUtf8(str.c_str());
}

NewsDialog::NewsDialog(int id, QVector<QString> ttl, QVector<int> d, QWidget *parent)
	: QDialog(parent),titles(ttl), ids(d)
{
	ui.setupUi(this);
	std::string url = "output/" + std::to_string(id) + ".info";
	QString qurl = QString::fromStdString(url);
	QFile file(qurl);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QTextCodec *gbk = QTextCodec::codecForName("GBK");

	QByteArray line = file.readLine();
	QString title =gbk->toUnicode(line);
	ui.title->setText(title);

	line = file.readLine();
	QString source = gbk->toUnicode(line);
	ui.source->setText(source);

	line = file.readLine();
	QString time = gbk->toUnicode(line);
	ui.time->setText(time);

	QString contents;
	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		contents.append(gbk->toUnicode(line));
	}
	ui.content->setText(contents);

	for (int i = 0; i < ids.size(); i++)
		ui.promote->addItem(titles[i]);

	connect(ui.promote, &QListWidget::itemDoubleClicked, this, &NewsDialog::onPromote);
}

NewsDialog::~NewsDialog()
{
}
void NewsDialog::onPromote(QListWidgetItem * item) {
	emit openPromote(ids[ui.promote->currentRow()]);
}