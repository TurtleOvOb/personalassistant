#pragma once
#include<qobject.h>
#include<NoteKard.h>
#include<qvector.h>
#include"themeManager.h"
#include"NoteDao.h"
class noteManager :public QObject {
	Q_OBJECT
public:
	noteManager();
	~noteManager();
	NoteKard* create_NoteKard(QWidget* parent, QString title, QString des, int type,themeManager::Theme theme);
	bool delete_NoteKard(BaseKard* toDel_Kard);
	bool update_noteKard(NoteKard* kard);
	void search_NoteKard(QString title);
	void sortBy(int type);
	//void updateIconTheme();
	QVector<NoteKard*> getNoteKardList();
	QVector<NoteKard*>loadFromDataBase(QWidget*parent);
signals:
	void searchDone(QVector<NoteKard*>results);
	void sortDone(QVector<NoteKard*>results);
private:
	noteDao dao;
	QVector<NoteKard*>noteKardList;
	


};