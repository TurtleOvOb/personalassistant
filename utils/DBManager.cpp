#include<DBManager.h>
 QSqlDatabase& DBManager::instance() {
	 static QSqlDatabase instance;
	 instance = QSqlDatabase::addDatabase("QSQLITE");
	 instance.setDatabaseName("kard.db");
	 return instance;
}