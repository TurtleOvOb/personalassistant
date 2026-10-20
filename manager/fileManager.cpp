#include "fileManager.h"
#include<qfile.h>
#include"mySortModel.h"

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

//视图给的是proxy的index，排序、搜索、按类型分类之后它的row和源模型完全不是一回事。
//所有对外接口都要先过这一层映射，否则就会拿视图行号去动源数据。
QModelIndex fileManager::toSourceIndex(const QModelIndex& index) const
{
	if (!index.isValid() || model == nullptr) {
		return QModelIndex();
	}
	QModelIndex source = index;
	if (sortModel != nullptr) {
		source = sortModel->mapToSource(index);
	}
	if (!source.isValid()) {
		return QModelIndex();
	}
	//表格是整行选择，选中列不一定是第0列，统一归到第0列取数据
	return source.sibling(source.row(), 0);
}

QString fileManager::pathOfRow(int sourceRow) const
{
	if (model == nullptr || sourceRow < 0 || sourceRow >= model->rowCount()) {
		return QString();
	}
	QStandardItem* item = model->item(sourceRow, 0);
	if (item == nullptr) {
		return QString();
	}
	QString path = item->data(DataRole::PathRole).toString();
	if (!path.isEmpty()) {
		return path;
	}

	return QString();
}

int fileManager::idOfRow(int sourceRow) const
{
	if (model == nullptr || sourceRow < 0 || sourceRow >= model->rowCount()) return -1;
	QStandardItem* item = model->item(sourceRow, 0);
	if (item == nullptr) return -1;
	return item->data(DataRole::IdRole).toInt();
}

//把一条文件记录同时写进model和缓存，路径挂在item的role上，
//这样model自己就能回答"我是哪个文件"，不用靠行号去反查
void fileManager::appendFileRow(const QString& filePath, int id)
{
	if (id < 0)return;
		QFileInfo info(filePath);
	QString fileType = info.isDir() ? QStringLiteral("dir") : info.suffix();
	QString lastTime = info.lastModified().toString("yyyy-MM-dd hh:mm:ss");

	QStandardItem* nameItem = new QStandardItem(info.fileName());
	nameItem->setData(filePath, DataRole::PathRole);
	nameItem->setData(id, DataRole::IdRole);

	QList<QStandardItem*> rowInfo;
	rowInfo.append(nameItem);
		rowInfo.append(new QStandardItem(lastTime));
		rowInfo.append(new QStandardItem(fileType));
	rowInfo.append(new QStandardItem(QString::number(info.size())));

		model->appendRow(rowInfo);
	
	if (id >= 0) {
		idCache.insert(filePath, id);
	}
}

bool fileManager::loadFile(QStringList&filePaths)
{
	if (filePaths.isEmpty()) {
		return false;
	}
	for (const QString& filePath : filePaths) {
		if (filePath.isEmpty()) continue;
		//入库的必须是完整路径，这里以前传的是文件名，重启后就再也找不到文件了
		int id = dao.addFile(filePath);
		appendFileRow(filePath, id);
	}
	return true;
}

bool fileManager::openFile(const QModelIndex& index)
{
	QModelIndex source = toSourceIndex(index);
	if (!source.isValid()) {
		return false;
	}
	QString filePath = pathOfRow(source.row());
	if (filePath.isEmpty()) {
		return false;
	}
	return QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

bool fileManager::deleteFile(const QModelIndex& index)
{
	//不能直接拿视图行号当索引，分类或者搜索筛选后那样会删掉别的文件。
	//正确做法：proxy index -> 源模型index -> 源行上的路径和数据库id
	QModelIndex source = toSourceIndex(index);
	if (!source.isValid()) {
		return false;
	}
	int rowIndex = source.row();
	QString path = pathOfRow(rowIndex);
	if (path.isEmpty()) {
		return false;
	}
	int id = idOfRow(rowIndex);
	if (id < 0) {
		id = idCache.value(path, -1);
	}

	QFile file(path);
	QString fileName = fileNameOf(index);
	if (!file.exists()) {
		QMessageBox::warning(nullptr, "错误", fileName + ":文件不存在",QMessageBox::Close);
		return false;
	}
	if (!file.remove()) {
		QMessageBox::warning(nullptr, "����", fileName + ":�ļ�ɾ��ʧ��",QMessageBox::Close);
		return false;
	}
	QMessageBox::information(nullptr, "��Ϣ", fileName + ":�ļ�ɾ���ɹ�", QMessageBox::Yes);

	//����ɾ���ˣ���ͬ���ڴ�����ݿ�
	model->removeRow(rowIndex);
	idCache.remove(path);
	//������ɾ�������������к�
	if (id >= 0) {
		dao.deleteFile(id);
}
	return true;
}

void fileManager::searchFile(const QString& fileName)
{
	if (sortModel == nullptr) return;
	sortModel->setFilterFixedString(fileName);
	
}

QString fileManager::getFilePath(const QModelIndex& index)
{
	QModelIndex source = toSourceIndex(index);
	if (!source.isValid()) return QString(); 
	return pathOfRow(source.row());
	}

QString fileManager::fileNameOf(const QModelIndex& index)
{
	QModelIndex source = toSourceIndex(index);
	if (!source.isValid() || model == nullptr) 	return QString();
	QStandardItem* item = model->item(source.row(), 0);
	return item == nullptr ? QString() : item->text();
}

bool fileManager::openFileLoc(const QModelIndex& index)
{
	QModelIndex source = toSourceIndex(index);
	if (!source.isValid()) {
		return false;
	}
	QString path = pathOfRow(source.row());
	if (path.isEmpty()) {
		return false;
	}
	QFileInfo fileInfo(path);
#if defined(Q_OS_WIN)
	// Windows ϵͳ��explorer /select, �ļ�·��
	QString nativePath = QDir::toNativeSeparators(fileInfo.absoluteFilePath());

	// ��ȷ��ʽ��explorer /select,"C:\xxx\file.txt"
	QStringList args;
	args << "/select," << nativePath;

	QProcess::startDetached("explorer", args);

#elif defined(Q_OS_MAC)
	// macOS ϵͳ��open -R �ļ�·��
	QProcess::startDetached("open", { "-R", fileInfo.absoluteFilePath() });

#elif defined(Q_OS_LINUX)
	// Linux ϵͳ��xdg-open �ļ���·��
	QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
#endif

	return true;
	}
	return false;

}


QStringList fileManager::loadFromDataBase() {
	//先把以前存成纯文件名的脏记录清掉，那些重启后已经无法定位了
	dao.removeInvalidRows();
	model->removeRows(0, model->rowCount());
	idCache.clear();
	QStringList filePaths;
	QVector<fileRecord> records = dao.getFileInfos();
	int count = 0;//计算没用的文件数量
	for (const fileRecord& record : records) {
		if (record.path.isEmpty() || !QFileInfo::exists(record.path)) {
			//文件已经被用户在外面删掉了，顺手把记录清干净
			qInfo() << "file no longer exists, drop record:" << record.path;
			dao.deleteFile(record.id);
			continue;
		}
		filePaths.append(record.path);
		appendFileRow(record.path, record.id);
	}
	if(count)
	QMessageBox::information(nullptr, "消息", "已清除" + QString::number(count)+"条无效文件", QMessageBox::Yes);
	return filePaths;
}

//void fileManager::updateIconTheme() {
//
//	for (fileKard* kard : fileKardLists) {
//		kard->setIcon();
//	}
//}
