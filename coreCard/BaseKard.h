#pragma once
//#include<qwidget.h>
#include<qframe.h>
#include<qobject.h>
#include<qcolor.h>
#include<qdebug.h>
#include<qevent.h>
#include"themeManager.h"
#include<qtimer.h>
class BaseKard : public QFrame {
	Q_OBJECT
public:
    explicit BaseKard(QWidget*parent);
	~BaseKard();
	
	virtual void setIcon(themeManager::Theme theme)=0;
    signals:
	void clicked();
private:
	
	void mouseReleaseEvent(QMouseEvent*event)override;
private slots:
	void onThemeChanged(themeManager::Theme theme);

};