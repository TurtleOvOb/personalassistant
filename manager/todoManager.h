#pragma once
#include"../todo/toDoKard.h"
#include<qvector.h>
#include<qwidget.h>
#include"todoDao.h"
class todoManager :public QObject{
	Q_OBJECT
public:
	todoManager();
	~todoManager();
	toDoKard* create_toDoKard(QWidget* parent,QString title,QString des, QDate date,QString prior);
	bool delete_toDoKard(BaseKard*toDel_Kard);
	bool update_toDoKard(toDoKard* kard);
	void search_toDoKard(QString title);
	QVector<toDoKard*>getKardList();
	QVector<toDoKard*> loadFromDataBase(QWidget* parent);
    signals :
	void searchDone(QVector<toDoKard*>results);
private:
	todoDao dao;
	QVector<toDoKard*>todoKardList;

};