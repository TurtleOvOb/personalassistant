#include"logger.h"

const QString logger::fileName = "lastest.log";
static QMutex mutex;
void logger::logHandler(QtMsgType msgType, const QMessageLogContext& context, const QString& msg) {
	static const QString logPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QMutexLocker locker(&mutex);
	//捕获日志
	QString logType;
	switch (msgType)
	{
	case QtDebugMsg:
		logType = "Debug";
		break;
	case QtWarningMsg:
		logType = "Warning";
		break;
	case QtCriticalMsg:
		logType = "Critical";
		break;
	case QtFatalMsg:
		logType = "Fatal";
		break;
	case QtInfoMsg:
		logType = "Info";
		break;
	default:
		logType = "Other";
		break;
	}
	//格式化
	QString contextInfo = QString("[File:%1,Line:%2]").arg(context.file).arg(context.line);
	QString datetime = QString("[time:%1]").arg(QDateTime::currentDateTime().toString("yyyy-mm-dd hh:mm:ss"));
	QString message = QString("[%1]%2%3%4").arg(logType).arg(datetime).arg(contextInfo).arg(msg);
	//输出到文件
	QDir dir;
	if (!dir.exists(logPath))dir.mkdir(logPath);
	QString filePath = QDir(logPath).filePath(fileName);
	QFile file(filePath);
	file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream ostream(&file);
	ostream << message << '\n';
	file.close();
    }
