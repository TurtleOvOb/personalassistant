#pragma once
#include<qsortfilterproxymodel.h>
#include"fileKard.h"
#include<qobject.h>
class mySortModel :public QSortFilterProxyModel {
	Q_OBJECT
public:
	bool isSearch=0;
	explicit mySortModel(QObject* parent = nullptr);
	~mySortModel();
	void setType(fileKard::Type type);
private:
	fileKard::Type type=fileKard::Default;
	bool filterAcceptsRow(int source_row,const QModelIndex&source_parent)const override;
	

};