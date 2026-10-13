#pragma once
#include<qstandarditemmodel.h>
#include<qsortfilterproxymodel.h>
#include<qfileinfo.h>
#include<qvector.h>
#include<qdesktopservices.h>
#include<qurl.h>
#include<qprocess.h>
#include<qdir.h>
#include<qhash.h>
#include<qmessagebox.h>
#include<fileKard.h>
#include"themeManager.h"
#include"fileDao.h"
class fileManager {
public:
	//挂在model第0列item上的自定义数据：完整路径和数据库真实主键id
	enum DataRole {
		PathRole = Qt::UserRole + 1,
		IdRole
	};
	fileManager();
	~fileManager();
	fileKard* createFileKard(QWidget*parent,fileKard::Type type,themeManager::Theme theme);
	void setModel(QStandardItemModel* model);
	void setSortModel(QSortFilterProxyModel* model);
	bool loadFile(QStringList& filePaths);
	//下面这几个收到的都是视图(proxy)的index，内部会先映射回源模型
	bool openFile(const QModelIndex&index);
	bool deleteFile(const QModelIndex& index);
	void searchFile(const QString& fileName);
	QString getFilePath(const QModelIndex& index);
	QString fileNameOf(const QModelIndex& index);
	bool openFileLoc(const QModelIndex& index);
	QStringList loadFromDataBase();
	//void updateIconTheme();
private:
	//视图index -> 源模型index，排序/筛选后这一步是必须的
	QModelIndex toSourceIndex(const QModelIndex& index) const;
	QString pathOfRow(int sourceRow) const;//获取视图中的文件路径（用于）
	int idOfRow(int sourceRow) const;//获取视图中的id（用于数据库）
	void appendFileRow(const QString& filePath, int id);
	fileDao dao;
	QStandardItemModel* model;
	QSortFilterProxyModel* sortModel;

	QHash<QString, int> idCache;//完整路径 -> 数据库id
	QVector<fileKard*>fileKardLists;
};