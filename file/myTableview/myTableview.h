#pragma once
#include<qtableview.h>
#include<qevent.h>
#include<qmimedata.h>
#include<qobject.h>
#include<qpainter.h>
class myTableview :public QTableView {
	Q_OBJECT
public:

	explicit myTableview(QWidget *parent);
	~myTableview();
signals:
	void done( QStringList &filePaths);
private:

	void dragEnterEvent(QDragEnterEvent*event)override;
	void dragMoveEvent(QDragMoveEvent* event)override;
	void dropEvent(QDropEvent* event)override;
	void paintEvent(QPaintEvent* event)override;

};