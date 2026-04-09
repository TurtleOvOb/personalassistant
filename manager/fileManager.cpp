#include "fileManager.h"

fileManager::fileManager()
{
	model = nullptr;
	sortModel = nullptr;
	
}

fileManager::~fileManager()
{
}

void fileManager::setSearchState(bool state)
{

}

fileKard* fileManager::createFileKard(QWidget* parent, fileKard::Type type, themeManager::Theme theme)
{
	fileKard* kard = new fileKard(parent, type,theme);
	fileKardLists.append(kard);
	return kard;
}

void fileManager::setModel(QStandardItemModel* model)
{
	this->model = model;
}

void fileManager::setSortModel(QSortFilterProxyModel* model)
{
	this->sortModel = model;
	sortModel->setSourceModel(this->model);
}

bool fileManager::loadFile(QStringList&filePaths)
{
	if (filePaths.isEmpty()) {
		return false;
	}
	QString fileName;
	QString lastTime; 
	QString fileType;
	qint64 fileSize;
	for (QString filePath : filePaths) {
		files.append(filePath);
		QFileInfo info(filePath);
	    fileName=info.fileName();
		lastTime=info.lastModified().toString("yyyy-MM-dd hh:mm:ss");
		if (info.isDir()) {
			fileType = "director";
		};
		fileType =info.suffix();
		fileSize=info.size();
		QList<QStandardItem*>rowInfo;
		rowInfo.append(new QStandardItem(fileName));
		rowInfo.append(new QStandardItem(lastTime));
		rowInfo.append(new QStandardItem(fileType));
		rowInfo.append(new QStandardItem(QString::number(fileSize)));
		model->appendRow(rowInfo);
	}
	return true;
}

bool fileManager::openFile(const QModelIndex& index)
{
	if (index.isValid()) {
	int rowIndex=index.row();
	QString filePath=files.at(rowIndex);
	QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
	return true;
	}

}

bool fileManager::deleteFile(QModelIndex& index)
{
	/*if (!index.isValid()) {
		qDebug() << "none file selected!";
		return false;
	}*/
	int rowIndex = index.row();
	QFile file(files.at(rowIndex));
    model->removeRow(rowIndex);
	files.remove(rowIndex);
	if (!file.exists()) {
		qDebug() << "file doesn't exists";
		return false;
	}
	if (file.remove()) {
		qDebug() << "file delete success";
		return true;
	}
	else {
		qWarning() << "file delete fail:" << file.errorString();
		return false;
	}

}

void fileManager::searchFile(const QString& fileName)
{
	sortModel->setFilterFixedString(fileName);
	qDebug() << "fileManager::searchFile";
}

QString fileManager::getFilePath(QModelIndex&index)
{
	if (index.isValid()) {
	int rowIndex = index.row();
	return files.at(rowIndex);
	}

}

bool fileManager::openFileLoc(QModelIndex& index)
{
	if (index.isValid()) {
	int rowIndex = index.row();
	QString path =files.at(rowIndex); 
	QFileInfo fileInfo(path);
#if defined(Q_OS_WIN)
	// Windows 系统：explorer /select, 文件路径
	QString nativePath = QDir::toNativeSeparators(fileInfo.absoluteFilePath());

	// 正确格式：explorer /select,"C:\xxx\file.txt"
	QStringList args;
	args << "/select," << nativePath;

	QProcess::startDetached("explorer", args);

#elif defined(Q_OS_MAC)
	// macOS 系统：open -R 文件路径
	QProcess::startDetached("open", { "-R", fileInfo.absoluteFilePath() });

#elif defined(Q_OS_LINUX)
	// Linux 系统：xdg-open 文件夹路径
	QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
#endif

	return true;
	}
	return false;

}

//void fileManager::updateIconTheme() {
//
//	for (fileKard* kard : fileKardLists) {
//		kard->setIcon();
//	}
//}
