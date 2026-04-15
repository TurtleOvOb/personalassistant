	#include "toDoKard.h"

toDoKard::toDoKard(QWidget* parent):BaseKard(parent)
{

	    description = "";
		DeadLine.setDate(2006, 7, 18);
		//label字体大小要改，hlayout_1上间隔要改
		mainLayout = new QVBoxLayout();
		gridLayout = new QGridLayout();
		hLayout = new QHBoxLayout();
		hLayout1 = new QHBoxLayout();
		hLayout2 = new QHBoxLayout();
		label_title = new QLabel(this);
		 label_DeadLine = new QLabel(this);
		 label_Date = new QLabel(this);
		 label_Priority = new QLabel(this);
		 label_Priority_level = new QLabel(this);
		icon = new QLabel(this);
		QFont font;
		font.setPointSize(12);
		font.setBold(true);
		label_title->setFont(font);
		
		icon->setMinimumSize(20, 20);
		icon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	
	    
		label_DeadLine->setMinimumSize(15, 15);
		label_DeadLine->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	
		label_Priority->setMinimumSize(15, 15);
		label_Priority->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
		label_Priority->setAlignment(Qt::AlignBottom);

		QFont font2;
		font2.setPointSize(9);
		label_Priority_level->setFont(font2);
		label_Priority_level->setAlignment(Qt::AlignCenter);
		label_Priority_level->setMaximumSize(40, 20);
		//label_Priority->setAlignment(Qt::AlignCenter);
		/*label_Priority_level->setText("HIGH");*/
		//label_Priority_level->setAlignment(Qt::AlignCenter);
		hLayout->addWidget(icon);
		hLayout->addWidget(label_title);
		hLayout->setSpacing(5);
		hLayout1->addWidget(label_DeadLine);
		hLayout1->addWidget(label_Date);
		hLayout1->setSpacing(5);
		hLayout2->addWidget(label_Priority);
		hLayout2->addWidget(label_Priority_level);
		hLayout2->setSpacing(5);
        gridLayout->setVerticalSpacing(18);
		/*gridLayout->setColumnStretch(0,1);
		gridLayout->setColumnStretch(1, 2);
		gridLayout->setColumnStretch(2, 1);*/
		gridLayout->setContentsMargins(0, 5, 0, 0);
		gridLayout->addLayout(hLayout,0,0);
		gridLayout->addLayout(hLayout1, 1, 0,Qt::AlignBottom);
		gridLayout->addLayout(hLayout2, 1, 1, Qt::AlignBottom);
		mainLayout->addLayout(gridLayout);
		mainLayout->setAlignment(Qt::AlignTop);
		setLayout(mainLayout);
	
	}

toDoKard::~toDoKard()
	{
	}

int toDoKard::Id() {
	return this->id;
}

QString toDoKard::Description()
{
	return description;
}

QString toDoKard::Title()
{
	return label_title->text();
}

QDate toDoKard::Date()
{
	return DeadLine;
}

QString toDoKard::Priority()
{
	return label_Priority_level->text();
}

void toDoKard::setId(int id)
{
	this->id = id;
}

void toDoKard::setDescription(QString des)
{
	description = des;

}

void toDoKard::setTitle(QString title)
{
	label_title->setText(title);
}

void toDoKard::setDate(QDate Date)
{
	this->DeadLine = Date;
	label_Date->setText(DeadLine.toString("yyyy/MM/dd"));
}

void toDoKard::setPriority(QString Prior)
{
	label_Priority_level->setText(Prior);
	setPriorityQss();
}

void toDoKard::setIcon(themeManager::Theme theme)
{
	QIcon realIcon, DeadLineIcon, PriorityIcon;
	QPixmap pixmap, pixmap2, pixmap3;
	switch (theme)
	{
	case themeManager::Light:
		realIcon.addFile(":/blackIcon/res/black/-todo.png");
		pixmap = realIcon.pixmap(realIcon.actualSize(QSize(20, 20)));
		DeadLineIcon.addFile(":/blackIcon/res/black/faburiqi.png");
		pixmap2 = DeadLineIcon.pixmap(DeadLineIcon.actualSize(QSize(20, 20)));
		PriorityIcon.addFile(":/blackIcon/res/black/youxianx.png");
		pixmap3 = PriorityIcon.pixmap(PriorityIcon.actualSize(QSize(18, 18)));

		break;
	case themeManager::Dark:
		realIcon.addFile(":/new/prefix1/res/-todo.png");
		pixmap = realIcon.pixmap(realIcon.actualSize(QSize(20, 20)));
		DeadLineIcon.addFile(":/new/prefix1/res/faburiqi.png");
		pixmap2 = DeadLineIcon.pixmap(DeadLineIcon.actualSize(QSize(20, 20)));
		PriorityIcon.addFile(":/new/prefix1/res/youxianx.png");
		pixmap3 = PriorityIcon.pixmap(PriorityIcon.actualSize(QSize(18, 18)));
		break;
	default:
		break;
	}	
	icon->setPixmap(pixmap);
	label_DeadLine->setPixmap(pixmap2);
	label_Priority->setPixmap(pixmap3);
}

void toDoKard::setPriorityQss()
{
	QString qss;
	if (label_Priority_level->text() == "HIGH") {
		qss = R"(
         background-color:#FCC2BE;
         border-radius:4px;
         color:black;
         )";
	}
	else if (label_Priority_level->text() == "MIDDLE") {
		qss = R"(
         background-color:#FFCC33;
         border-radius:4px;
          color:black;
         )";

	}
	else if (label_Priority_level->text() == "LOW") {
		qss = R"(
         background-color:#27AE60;
         border-radius:4px;
          color:black;
         )";

	}
	label_Priority_level->setStyleSheet(qss);
}

void toDoKard::mouseReleaseEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::LeftButton) {
	    QFrame::mouseReleaseEvent(event);
		emit clicked(this);
		}
	

	}

	void toDoKard::mouseDoubleClickEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::LeftButton) {
		QFrame::mouseDoubleClickEvent(event);
		emit doubleClicked(this);
		}
	
	}
