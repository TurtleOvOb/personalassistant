#include "myTableview.h"

myTableview::myTableview(QWidget* parent):QTableView(parent)
{
	this->setAcceptDrops(true);
}

myTableview::~myTableview()
{
}

void myTableview::dragEnterEvent(QDragEnterEvent* event)
{

	event->acceptProposedAction();
}

void myTableview::dragMoveEvent(QDragMoveEvent* event)
{
	// 必须重写这个，否则鼠标移动时接受状态会失效
	if (event->mimeData()->hasUrls()) {
		event->acceptProposedAction();
	}
	else {
		event->ignore();
	}
}

void myTableview::dropEvent(QDropEvent* event)
{
	QList<QUrl>urls = event->mimeData()->urls();
	QStringList filePaths;
	for (QUrl url : urls) {
		QString filePath = url.toLocalFile();
		filePaths.append(filePath);
	}
	emit done(filePaths);
	qDebug() << "file receive done!";
}
void myTableview::paintEvent(QPaintEvent* event) {
	QTableView::paintEvent(event); 
	if (model() == nullptr || model()->rowCount() == 0) {
		QPainter painter(viewport());
		painter.setPen(Qt::gray);

		// 设置提示文字和字体
		QFont font = painter.font();
		font.setPointSize(12);
		painter.setFont(font);
		QString text = "暂无数据，请点击“添加”按钮";
		// 在视口正中心绘制文字
		painter.drawText(viewport()->rect(), Qt::AlignCenter, text);
	}
}