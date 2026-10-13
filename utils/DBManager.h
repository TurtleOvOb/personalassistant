#pragma once
#include<qsqldatabase.h>
#include<qstandardpaths.h>
class DBManager {
public:
	static QSqlDatabase& instance();

};