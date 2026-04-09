#pragma once
#include<qobject.h>
#include<qlabel.h>
#include<qboxlayout.h>
#include<qlineedit.h>
#include"../coreCard/BaseKard.h"
#include<qtoolbutton.h>
#include"themeManager.h"

class NoteKard :public BaseKard {
	Q_OBJECT
public:
	enum noteType{Default,Work,Daily,Study};
	 NoteKard(QWidget* parent);
	~NoteKard();
	void setTitle(QString title);
	void setDes(QString des);
	void setType(int type, themeManager::Theme theme);
	void setIcon(themeManager::Theme theme)override;
	QString Title();
	QString Des();
	noteType type=Default;
signals:
	void clicked(NoteKard* kard);
	void doubleClicked(NoteKard* kard);
private:
	QVBoxLayout* mainLayout;
	QVBoxLayout* verticalLayout;
	QHBoxLayout* horizonLayout;
	QSpacerItem* spacer;
	QGridLayout* gLayout;
	QToolButton* icon_Note;
	
	QLabel* desInput;
    QString des;
	QLabel* title;
	QLabel* label_createDate;

	void mouseReleaseEvent(QMouseEvent* event)override;
	void mouseDoubleClickEvent(QMouseEvent* event)override;


};