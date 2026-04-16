#pragma once
#include<qsqlquery.h>
#include"DBManager.h"
#include<qdatetime.h>
#include"qsqlerror.h"
#include"NoteKard.h"
#include<qdebug.h>
class noteDao {

public:
	noteDao();
	~noteDao();
	int addKard(QString title, QString des, int type);
	bool deleteKard(int id);
	bool updateKard(int id, QString title, QString des, int type);
	QVector<NoteKard*> getNoteKards(QWidget* parent);
private:
	bool initQuery();


}; 