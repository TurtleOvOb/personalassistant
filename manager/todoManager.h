#pragma once
#include"../todo/toDoKard.h"
#include<qvector.h>
#include<qwidget.h>
class todoManager :public QObject{
	Q_OBJECT
public:
	todoManager();
	~todoManager();
	toDoKard* create_toDoKard(QWidget* parent,QString title,QString des, QDate date,QString prior);
	bool delete_toDoKard(BaseKard*toDel_Kard);
	void save_toDoKard();//保存到数据库？
	void search_toDoKard(QString title);
	QVector<toDoKard*> getKardList();

    signals :
	void searchDone(QVector<toDoKard*>results);
private:
	QVector<toDoKard*>todoKardList;

};