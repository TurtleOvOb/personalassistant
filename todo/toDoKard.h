#pragma once

#include<qboxlayout.h>
#include<qlabel.h>
#include<qobject.h>
#include<qtimer.h>
#include<qapplication.h>
#include"../coreCard/BaseKard.h"
#include<qdatetime.h>
#include"themeManager.h"
class toDoKard :public BaseKard {
	Q_OBJECT
public:
	explicit toDoKard(QWidget*parent);
	~toDoKard();
 
	QString Description();
	QString Title();
	QDate Date();
	QString Priority();
	void setId(int id);
	void setDescription(QString des);
	void setTitle(QString title);
	void setDate(QDate Date);
	void setPriority(QString Prior);
	void setIcon(themeManager::Theme theme)override;
signals:
	void clicked(toDoKard* kard);
	void doubleClicked(toDoKard* kard);
	
private:
	int id = 0;
	QString description;
	QDate DeadLine;
	QVBoxLayout* mainLayout;
	QGridLayout* gridLayout;
	QHBoxLayout* hLayout;
	QHBoxLayout* hLayout1;
	QHBoxLayout* hLayout2;
	QLabel* icon;
	QLabel* label_title;
	QLabel* label_DeadLine;
	QLabel* label_Date;
	QLabel* label_Priority;
	QLabel* label_Priority_level;
	void setPriorityQss();
	void mouseReleaseEvent(QMouseEvent* event)override;
	void mouseDoubleClickEvent(QMouseEvent* event)override;

};