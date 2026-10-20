#include<DBManager.h>
 QSqlDatabase& DBManager::instance() {
	 static QSqlDatabase instance;

	 if(!instance.isValid())
	 instance = QSqlDatabase::addDatabase("QSQLITE");
	 QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	 QDir dir;
	 if (!dir.exists(dataLocation)) {
		 qInfo() << "数据库目录不存在，创建目录";
		 if (dir.mkdir(dataLocation)) {
			 qInfo() << "数据库目录创建成功";
		 }
		 else {
			 qWarning() << "数据库目录创建失败";
		 }

	 }
		
	 QString dbPath =QDir(dataLocation).filePath("kard.db");
	 instance.setDatabaseName(dbPath);
	 return instance;
}