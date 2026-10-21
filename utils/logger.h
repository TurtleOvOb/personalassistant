#pragma once
#include<qlogging.h>
#include<qmutex.h>
#include<qdatetime.h>
#include<qdir.h>
#include<qstandardpaths.h>
class logger {
private:
	static const QString fileName;
public:
	static void logHandler(QtMsgType msgType, const QMessageLogContext& context, const QString& msg);
};