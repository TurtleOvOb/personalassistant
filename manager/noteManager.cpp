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
    noteKardList.append(kard);
	return kard;
}

bool noteManager::delete_NoteKard(BaseKard* toDel_Kard)
{
    noteKardList.removeOne(toDel_Kard);
    toDel_Kard->hide();
    toDel_Kard->deleteLater();
    toDel_Kard = nullptr;
    return true;
}

void noteManager::save_NoteKard()
{
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
        Type = NoteKard::Study;
        break;
    case 3:
        Type = NoteKard::Daily;
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