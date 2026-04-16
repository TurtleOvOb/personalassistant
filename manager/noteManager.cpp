#include<noteManager.h>
noteManager::noteManager()
{
}

noteManager::~noteManager()
{
}

NoteKard* noteManager::create_NoteKard(QWidget* parent, QString title, QString des, int type, themeManager::Theme theme)
{
    qDebug() << type;
	NoteKard* kard = new NoteKard(parent);
    kard->setTitle(title);
    kard->setDes(des);
    kard->setType(type,theme);
    dao.addKard(title, des, type);
    noteKardList.append(kard);
	return kard;
}

bool noteManager::delete_NoteKard(BaseKard* toDel_Kard)
{
    noteKardList.removeOne(toDel_Kard);
    auto notekard = qobject_cast<NoteKard*>(toDel_Kard);
    dao.deleteKard(notekard->Id());
    toDel_Kard->hide();
    toDel_Kard->deleteLater();
    toDel_Kard = nullptr;
    return true;
}

bool noteManager::update_noteKard(NoteKard* kard)
{
    if (dao.updateKard(kard->Id(), kard->Title(),
        kard->Des(), kard->type)) {
        qDebug() << "update success";
        return true;
    }
    qDebug() << "update failed";
    return false;
}

void noteManager::search_NoteKard(QString title)
{
    qDebug() << "start searching";
    QVector<NoteKard*>results;
    for (NoteKard* kard : noteKardList) {
        if (kard->Title().contains(title)) {
            results.append(kard);
        }
    }
    emit searchDone(results);
    qDebug() << "search done!";
}

void noteManager::sortBy(int type)
{
    NoteKard::noteType Type;
    switch (type)
    {
    case 0:
        Type = NoteKard::Default;
        break;
    case 1:
        Type = NoteKard::Work;
        break;
    case 2:
        Type = NoteKard::Daily;
        break;
    case 3:
        Type = NoteKard::Study;
        break;
    default:
        break;
    }
    qDebug() << "start searching";
    QVector<NoteKard*>results;
    for (NoteKard* kard : noteKardList) {
        //卡片类型与筛选类型匹配或者筛选类型为默认
        
        if (kard->type== Type|| Type ==NoteKard::Default) {
            results.append(kard);
        }
    }
    emit sortDone(results);
    qDebug() << "search done!";
}

//void noteManager::updateIconTheme()
//{//每个卡片调用seticon方法,检查当前theme并更新icon
//    for (NoteKard* kard : noteKardList) {
//        kard->setIcon();
//    }
//}

QVector<NoteKard*> noteManager::getNoteKardList() {
    return noteKardList;
}

QVector<NoteKard*>noteManager::loadFromDataBase(QWidget*parent) {
    noteKardList = dao.getNoteKards(parent);
    return noteKardList;

}