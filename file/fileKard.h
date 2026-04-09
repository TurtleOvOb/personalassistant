#pragma once
#include"../coreCard/BaseKard.h"
#include<qlabel.h>
#include<qboxlayout.h>
#include<qtoolbutton.h>
#include"themeManager.h"
class fileKard :public BaseKard {
	Q_OBJECT
public:
	enum Type{Default,Doc,Img,Code,Else};
	explicit fileKard(QWidget* parent,Type type=fileKard::Default, themeManager::Theme theme=themeManager::Dark);
	~fileKard();
	void setType(Type type, themeManager::Theme theme);
	void setIcon(themeManager::Theme theme)override;
signals:
	void clicked(Type type);
	
private:
	QHBoxLayout* hLayout;
	QVBoxLayout* mainLayout;
	QIcon icon;
	QToolButton* toolButton;
	QLabel* label;
	Type type = fileKard::Default;
	
	void mouseReleaseEvent(QMouseEvent* event)override;
\
};