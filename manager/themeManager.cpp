#include"themeManager.h"

themeManager::themeManager()
{

}

themeManager::~themeManager()
{
}

void themeManager::setTheme(int index)
{
    if (curTheme == index)return;
    if (index == 1) {
        curTheme = Light;
     
   }
    else if(index==2){
        curTheme = Dark;
    }
    qDebug() << curTheme;
    emit themeChanged(curTheme);
}

themeManager::Theme themeManager::currentTheme()const {
    return curTheme;
}

QString themeManager::switchTheme(int index) {
    QString qss;
    QDir dirMode;
    switch (index)
    {
    case 1:
        dirMode.setPath(":/new/prefix3/style/lightMode");
        break;
    case 2:
        dirMode.setPath(":/new/prefix2/style/darkMode");
        break;
    default:
        break;
    }
    QFileInfoList dirList = dirMode.entryInfoList();
    QFile file;
    for (QFileInfo mode : dirList) {
        QString dirPath = mode.absoluteFilePath();
        qDebug() << "dirName" << dirPath;
        QDir dir(dirPath);
        QFileInfoList fileInfoList = dir.entryInfoList();
        for (QFileInfo info : fileInfoList) {
            QString filePath = info.absoluteFilePath();
            qDebug() << "fileName" << filePath;
            file.setFileName(filePath);
            QTextStream in(&file);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qss += in.readAll();
                file.close();
            }
            else {
                qDebug() << filePath << "加载失败";
            }
        }

    }
    return qss;
}