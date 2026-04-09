#include "todoManager.h"
#include "noteManager.h"

todoManager::todoManager()
{
}

todoManager::~todoManager()
{
}

toDoKard* todoManager::create_toDoKard(QWidget*parent,QString title, QString des, QDate date, QString prior)
{
	toDoKard* kard = new toDoKard(parent);
	kard->setTitle(title);
	kard->setDescription(des);
	kard->setDate(date);
	kard->setPriority(prior);
	todoKardList.append(kard);
	return kard;
}

bool todoManager::delete_toDoKard(BaseKard* toDel_Kard)
{
	qDebug() << "deleting curKard";
	if (toDel_Kard != nullptr) {
	todoKardList.removeOne(toDel_Kard);
	toDel_Kard->hide();
	toDel_Kard->deleteLater();
	toDel_Kard = nullptr;

	qDebug() << "delete success";
	return true;
	}
	qDebug() << "delete fail:"<<"none todoKard selected";
	return false;
}



void todoManager::save_toDoKard()
{
}

void todoManager::search_toDoKard(QString title)
{
	qDebug() << "start searching";
	QVector<toDoKard*>results;
	for (toDoKard* kard : todoKardList) {
		if (kard->Title().contains(title)) {
			results.append(kard);
		}
	}
	emit searchDone(results);
	qDebug() << "search done!";
}



QVector<toDoKard*> todoManager::getKardList()
{
	return todoKardList;
}




