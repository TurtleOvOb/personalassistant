#include "fileKard.h"

fileKard::fileKard(QWidget* parent, Type type, themeManager::Theme theme):BaseKard(parent)
{
	this->setMaximumHeight(60);
	label = new QLabel(this);
	hLayout = new QHBoxLayout();
	mainLayout = new QVBoxLayout();
	toolButton = new QToolButton(this);
	setType(type,theme);
	toolButton->setText("");
	toolButton->setMinimumSize(30,30);
	toolButton->setIconSize(toolButton->size());
	toolButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	toolButton->setStyleSheet(R"(
    background-color:rgba(0,0,0,0);
    border: none;)");
	QFont textFont;
	textFont.setPointSize(12);
	label->setFont(textFont);
	//label->setAlignment(Qt::AlignLeft);
	hLayout->addWidget(toolButton);
	hLayout->addWidget(label);
	hLayout->setStretch(1, 3);
	//hLayout->setSpacing();
	mainLayout->addLayout(hLayout);
	setLayout(mainLayout);

}


fileKard::~fileKard()
{
}

void fileKard::setType(Type type, themeManager::Theme theme)
{
	this->type = type;
	setIcon(theme);
}

void fileKard::setIcon(themeManager::Theme theme) {
	qDebug() << "fileKard setIcon triggered";
	switch (type)
	{
	case fileKard::Default:
		label->setText("全部文件");
		if (theme == themeManager::Light) {
			icon.addPixmap(QPixmap(":/blackIcon/res/black/quanbu.png"));
			qDebug() << "light";
		}
		else {
			icon.addPixmap(QPixmap(":/new/prefix1/res/quanbu.png"));
			qDebug() << "dark";
		}
		break;
	case fileKard::Doc:
		label->setText("文档");
		if (theme == themeManager::Light) {
			icon.addPixmap(QPixmap(":/blackIcon/res/black/wendang.png"));
		}
		else {
			icon.addPixmap(QPixmap(":/new/prefix1/res/wendang.png"));
		}
		break;
	case fileKard::Img:
		label->setText("图像");
		if (theme == themeManager::Light) {
			icon.addPixmap(QPixmap(":/blackIcon/res/black/tupian.png"));
		}
		else {
			icon.addPixmap(QPixmap(":/new/prefix1/res/tupian.png"));
		}

		break;
	case fileKard::Code:

		label->setText("代码");
		if (theme == themeManager::Light) {
			icon.addPixmap(QPixmap(":/blackIcon/res/black/code.png"));
		}
		else {
			icon.addPixmap(QPixmap(":/new/prefix1/res/code.png"));
		}

		break;
	case fileKard::Else:
		label->setText("其他类型");
		if (theme == themeManager::Light) {
			icon.addPixmap(QPixmap(":/blackIcon/res/black/qita.png"));
		}
		else {
			icon.addPixmap(QPixmap(":/new/prefix1/res/qita.png"));
		}

		break;
	default:
		break;
	}
	toolButton->setIcon(icon);
}
void fileKard::mouseReleaseEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton) {
		qDebug() << "filekard clicked";
		emit clicked(this->type);
	}
}

