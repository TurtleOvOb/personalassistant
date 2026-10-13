#pragma once
#include<qsqlquery.h>
#include"DBManager.h"
#include<qdatetime.h>
#include"qsqlerror.h"
#include"fileKard.h"
#include<qdebug.h>
#include<qvector.h>
#include<qstring.h>
#include<qhash.h>

//文件记录：id 是数据库主键，不是model里的行号
struct fileRecord {
	int id = -1;
	QString path;
};

class fileDao {

public:
	fileDao();
	~fileDao();
	//写入一条文件记录，返回它的真实主键id，失败返回 -1
	int addFile(QString filePath);
	//按主键id删除，绝不能用model行号
	bool deleteFile(const int id);
	QVector<fileRecord> getFileInfos();
	//启动时清理历史脏数据：之前误存成纯文件名（相对路径）的记录
	void removeInvalidRows();
private:
	bool initQuery();
	//QString fileTableName();

};