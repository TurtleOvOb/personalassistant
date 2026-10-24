#include<DBManager.h>
#include<qfile.h>

bool DBManager::s_testMode = false;
QString DBManager::s_testPath;

QSqlDatabase& DBManager::instance() {
	static QSqlDatabase instance;
	if (!instance.isValid())
		instance = QSqlDatabase::addDatabase("QSQLITE");

	//测试模式：同一个连接句柄改指向临时库，
	//这样 fileDao 一行都不用改就能被隔离在临时库里
	if (s_testMode) {
		if (instance.databaseName() != s_testPath) {
			instance.close();
			instance.setDatabaseName(s_testPath);
		}
		return instance;
	}

	QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QDir dir;
	if (!dir.exists(dataLocation)) {
		qInfo() << "数据库目录不存在，创建目录";
		if (dir.mkpath(dataLocation)) {
			qInfo() << "数据库目录创建成功";
		}
		else {
			qWarning() << "数据库目录创建失败";
		}
	}

	QString dbPath = QDir(dataLocation).filePath("kard.db");
	instance.setDatabaseName(dbPath);
	return instance;
}

QSqlDatabase& DBManager::testInstance() {
	s_testMode = true;
	if (s_testPath.isEmpty()) {
		s_testPath = QDir(QDir::tempPath()).filePath("pa_unit_test.db");
		QFile::remove(s_testPath);
	}
	return instance();
}