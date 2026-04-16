#pragma once
#include<qstandarditemmodel.h>
#include<qsortfilterproxymodel.h>
#include<qfileinfo.h>
#include<qvector.h>
#include<qdesktopservices.h>
#include<qurl.h>
#include<qprocess.h>
#include<qdir.h>
#include<fileKard.h>
#include"themeManager.h"
#include"fileDao.h"
class fileManager {
public:
	fileManager();
	~fileManager();
	void setSearchState(bool state);
	fileKard* createFileKard(QWidget*parent,fileKard::Type type,themeManager::Theme theme);
	void setModel(QStandardItemModel* model);
	void setSortModel(QSortFilterProxyModel* model);
	bool loadFile(QStringList& filePaths);
	bool openFile(const QModelIndex&index);
	bool deleteFile(QModelIndex& index);
	void searchFile(const QString& fileName);
	QString getFilePath(QModelIndex&index);
	bool openFileLoc(QModelIndex& index);
	QStringList loadFromDataBase();
	//void updateIconTheme();
private:
	fileDao dao;
	QStandardItemModel* model;
	QSortFilterProxyModel* sortModel;
	QStringList files;//临时存储到内存中，后续再改成数据库
	QVector<fileKard*>fileKardLists;
};