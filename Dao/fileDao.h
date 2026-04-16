#pragma once
#include<qsqlquery.h>
#include"DBManager.h"
#include<qdatetime.h>
#include"qsqlerror.h"
#include"fileKard.h"
#include<qdebug.h>
class fileDao {

public:
	fileDao();
	~fileDao();
	int addFile(QString filePath);
	bool deleteFile(int row);
	QStringList getFileInfos();
private:
	bool initQuery();


};