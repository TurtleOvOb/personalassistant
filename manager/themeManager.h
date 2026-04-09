#pragma once
#include<qobject.h>
#include<qdebug.h>
#include<qdir.h>
class themeManager :public QObject{
	Q_OBJECT
public:
	enum Theme {
		Light=1,
		Dark
	};
	themeManager();
	~themeManager();
	static themeManager* instance() {
		static themeManager themeInstance;
		return &themeInstance;
	}
	Theme currentTheme()const;
	QString switchTheme(int index);
signals:
void themeChanged(Theme theme);
public slots:
		void setTheme(int index);
private:
	Theme curTheme = Dark;

	
};