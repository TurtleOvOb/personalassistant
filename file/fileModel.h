#pragma once
#include<qabstractitemmodel.h>

class fileModel :public QAbstractTableModel  {
public:
	fileModel();
	~fileModel();
private:
	int rowCount(const QModelIndex& index)const override;
	int columnCount(const QModelIndex& index)const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole)const override;

};