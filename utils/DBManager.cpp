#include<DBManager.h>
 QSqlDatabase& DBManager::instance() {
	 static QSqlDatabase instance;
	 if(!instance.isValid())
	 instance = QSqlDatabase::addDatabase("QSQLITE");
	 QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"kard.db";
	 instance.setDatabaseName(dbPath);
	 return instance;
}