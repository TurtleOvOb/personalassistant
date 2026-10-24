#pragma once
#include<qsqldatabase.h>
#include<qstandardpaths.h>
#include<qdir.h>
#include<qdebug.h>
class DBManager {
public:
	static QSqlDatabase& instance();
	//单元测试专用：把默认连接切到一个临时库文件，避免污染 AppData 里的真实 kard.db
	static QSqlDatabase& testInstance();
private:
	static bool s_testMode;
	static QString s_testPath;
};