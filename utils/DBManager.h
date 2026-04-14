#pragma once
#include<qsqldatabase.h>

class DBManager {
public:
	static QSqlDatabase& instance();

};