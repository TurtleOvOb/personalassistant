#pragma once
#include<qobject.h>
#include<NoteKard.h>
#include<qvector.h>
#include"themeManager.h"
class noteManager :public QObject {
	Q_OBJECT
public:
	noteManager();
	~noteManager();
	NoteKard* create_NoteKard(QWidget* parent, QString title, QString des, int type,themeManager::Theme theme);
	bool delete_NoteKard(BaseKard* toDel_Kard);
	void save_NoteKard();//保存到数据库？
	void search_NoteKard(QString title);
	void sortBy(int type);
	//void updateIconTheme();
	QVector<NoteKard*> getNoteKardList();

signals:
	void searchDone(QVector<NoteKard*>results);
	void sortDone(QVector<NoteKard*>results);
private:
	QVector<NoteKard*>noteKardList;
	


};