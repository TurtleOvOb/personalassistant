#include "NoteKard.h"

NoteKard::NoteKard(QWidget* parent):BaseKard(parent)
{
	des = "";
	 mainLayout=new QVBoxLayout();
	 verticalLayout = new QVBoxLayout();
	 horizonLayout = new QHBoxLayout();
	 spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding,QSizePolicy::Minimum);
	 //gLayout=new QGridLayout();
	 icon_Note=new  QToolButton(this);
	 desInput = new QLabel(this);
	 title = new QLabel(this);
	 label_createDate = new QLabel(this);

	 QFont titleFont;
	 titleFont.setPointSize(12);
	 titleFont.setBold(true);
	 title->setFont(titleFont);
	 
	 desInput->setText(des);
	 icon_Note->setIconSize(QSize(20,20));
	 //由于qt的绘图机制，导致必须要同时设置背景和边界才能使按钮透明
	 icon_Note->setStyleSheet(R"(
     background-color:transparent;
     border:none;
    )");
	 QFont DateFont;
	 DateFont.setPointSize(12);
	 label_createDate->setFont(DateFont);
    label_createDate->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd"));
	QFont desFont;
	desFont.setPointSize(9);
	desInput->setFont(desFont);
	 desInput->setMinimumHeight(40);
	 desInput->setWordWrap(true);
	 desInput->setAlignment(Qt::AlignTop);
	 desInput->setStyleSheet("border-top:1px solid #3A3A3C;padding:4px");
	 
	 horizonLayout->addWidget(icon_Note);
	 horizonLayout->addWidget(title);
	 horizonLayout->addSpacerItem(spacer);
	 horizonLayout->addWidget(label_createDate);

	 verticalLayout->addLayout(horizonLayout);
	 verticalLayout->addWidget(desInput);
	 verticalLayout->setStretch(1, 3);
	 verticalLayout->setSpacing(4);
	 verticalLayout->setContentsMargins(5,0,5,0);
	 mainLayout->addLayout(verticalLayout);
	setLayout(mainLayout);


}

NoteKard::~NoteKard()
{
}

void NoteKard::setTitle(QString title)
{
	this->title->setText(title);
}

void NoteKard::setDes(QString des)
{
	this->des = des;
	desInput->setText(des);
}

void NoteKard::setType(int type, themeManager::Theme theme)
{
	switch (type)
	{
	case 0:
		this->type = NoteKard::Default;
		break;
	case 1:
		this->type = NoteKard::Work;
		break;
	case 2:
		this->type = NoteKard::Study;
		break;
	case 3:
		this->type = NoteKard::Daily;
		break;
	default:
		break;
	}
	setIcon(theme);
}

QString NoteKard::Title()
{
	return title->text();
}

QString NoteKard::Des()
{
	return des;
}

void NoteKard::setIcon(themeManager::Theme theme)
{
	qDebug() << "noteKard setIcon triggered";
	switch (type)
	{
	case NoteKard::Default:
		if (theme==themeManager::Light) {
			icon_Note->setIcon(QIcon(QPixmap(":/blackIcon/res/black/biji.png")));
		}
		else {
			icon_Note->setIcon(QIcon(QPixmap(":/new/prefix1/res/biji.png")));
		}
		break;
	case NoteKard::Work:
		qDebug() << "work";
		if (theme == themeManager::Light) {
			icon_Note->setIcon(QIcon(QPixmap(":/blackIcon/res/black/gongzuo.png")));
		}
		else {
			icon_Note->setIcon(QIcon(QPixmap(":/new/prefix1/res/gongzuo.png")));
		}
		break;
	case NoteKard::Study:
		if (theme == themeManager::Light) {
			icon_Note->setIcon(QIcon(QPixmap(":/blackIcon/res/black/xuexi.png")));
		}
		else {
		icon_Note->setIcon(QIcon(QPixmap(":/new/prefix1/res/xuexi.png")));
		}
		break;
	case NoteKard::Daily:
		if (theme == themeManager::Light) {
			icon_Note->setIcon(QIcon(QPixmap(":/blackIcon/res/black/shenghuo.png")));
		}
		else {
			icon_Note->setIcon(QIcon(QPixmap(":/new/prefix1/res/shenghuo.png")));
		}
		break;

	default:
		break;
	}

}

void NoteKard::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		QFrame::mouseReleaseEvent(event);
		emit clicked(this);
	}

}

void NoteKard::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		QFrame::mouseDoubleClickEvent(event);
		emit doubleClicked(this);
	}
}

