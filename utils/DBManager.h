#pragma once
#include<qsqldatabase.h>
#include<qstandardpaths.h>
#include<qdir.h>
#include<qdebug.h>
class DBManager {
public:
	static QSqlDatabase& instance();

};