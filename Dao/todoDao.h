#pragma once
#include<qsqlquery.h>
#include"DBManager.h"
#include<qdatetime.h>
#include"qsqlerror.h"
#include"toDoKard.h"
#include<qdebug.h>
class todoDao {

public:
	todoDao();
	~todoDao();
	int addKard(QString title,QString des, QDate deadLine , QString priority);
	bool deleteKard(int id);
	bool updateKard();
	QVector<toDoKard*> getToDoKards(QWidget* parent);
private:
	bool initQuery();

	
};