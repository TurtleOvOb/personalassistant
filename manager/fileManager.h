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
	//����model��0��item�ϵ��Զ������ݣ�����·�������ݿ���ʵ����id
	enum DataRole {
		PathRole = Qt::UserRole + 1,
		IdRole
	};
	fileManager();
	~fileManager();
	void setSearchState(bool state);
	fileKard* createFileKard(QWidget*parent,fileKard::Type type,themeManager::Theme theme);
	void setModel(QStandardItemModel* model);
	void setSortModel(QSortFilterProxyModel* model);
	bool loadFile(QStringList& filePaths);
	//�����⼸���յ��Ķ�����ͼ(proxy)��index���ڲ�����ӳ���Դģ��
	bool openFile(const QModelIndex&index);
	bool deleteFile(const QModelIndex& index);
	void searchFile(const QString& fileName);
	QString getFilePath(const QModelIndex& index);
	QString fileNameOf(const QModelIndex& index);
	bool openFileLoc(const QModelIndex& index);
	QStringList loadFromDataBase();
	//void updateIconTheme();
private:
	//��ͼindex -> Դģ��index������/ɸѡ����һ���Ǳ����
	QModelIndex toSourceIndex(const QModelIndex& index) const;
	QString pathOfRow(int sourceRow) const;//��ȡ��ͼ�е��ļ�·�������ڣ�
	int idOfRow(int sourceRow) const;//��ȡ��ͼ�е�id���������ݿ⣩
	void appendFileRow(const QString& filePath, int id);
	fileDao dao;
	QStandardItemModel* model;
	QSortFilterProxyModel* sortModel;

	QHash<QString, int> idCache;//完整路径 -> 数据库id
	QVector<fileKard*>fileKardLists;
};