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
	int id=dao.addKard(title, des, date, prior);
	kard->setId(id);
	todoKardList.append(kard);
	return kard;
}

bool todoManager::delete_toDoKard(BaseKard* toDel_Kard)
{
	qDebug() << "deleting curKard";
	if (toDel_Kard != nullptr) {
	todoKardList.removeOne(toDel_Kard);
	auto *kard = qobject_cast<toDoKard*>(toDel_Kard);
	if (dao.deleteKard(kard->Id())) {
		qDebug() << "delete todoKard from database failed";
	}
	toDel_Kard->hide();
	toDel_Kard->deleteLater();
	toDel_Kard = nullptr;

	qDebug() << "delete success";
	return true;
	}
	qDebug() << "delete fail:"<<"none todoKard selected";
	return false;
}

bool todoManager::update_toDoKard(toDoKard* kard)
{
	if (dao.updateKard(kard->Id(), kard->Title(),
		kard->Description(), kard->Date(), kard->Priority())) {
		qDebug() << "update success";
		return true;
	}
	qDebug() << "update failed";
	return false;
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


QVector<toDoKard*> todoManager::loadFromDataBase(QWidget* parent)
{
	todoKardList =dao.getToDoKards(parent);
	return todoKardList;
}




