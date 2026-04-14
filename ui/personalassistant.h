#pragma once
#include"mySortModel.h"
#include <QtWidgets/QWidget>
#include "ui_personalassistant.h"
#include"../coreCard/BaseKard.h"
#include"../manager/todoManager.h"
#include"../manager/fileManager.h"
#include"themeManager.h"
#include<noteManager.h>
#include<qfile.h>
#include<qfiledialog.h>
#include<qstandarditemmodel.h>
#include<qsettings.h>
//#include<qsortfilterproxymodel.h>
#include<qclipboard.h>
#include"todoDao.h"
//#include"NoteDao.h"
//#include"fileDao.h"
class toDoKard;
class NoteKard;
class fileKard;
QT_BEGIN_NAMESPACE
namespace Ui { class personalassistantClass; };
QT_END_NAMESPACE

class personalassistant : public QWidget
{
    Q_OBJECT

public:
    personalassistant(QWidget *parent = nullptr);
    ~personalassistant();

private:
    todoManager* todo;
    noteManager* note;
    fileManager *file;
    todoDao todoDAO;
    //noteDao noteDAO;
    //fileDao fileDAO;
    bool curTheme;
    bool isCreate_Note;
    bool isCreate;
    bool isEdit;
    BaseKard* cur_Selected_Kard;
    QStandardItemModel* model;
    mySortModel* sortModel;
    Ui::personalassistantClass *ui;
    void uiConfig();
    void initSlots();
    void handleKardAction(BaseKard*kard);
    void fillEditpage(BaseKard*kard);
    //void updateIconTheme();
    void loadFromDataBase();
    void readConfig();
    void closeEvent(QCloseEvent*event)override;

private slots:
    void switchToMidPages();
    void switchToEditPage();
    void switchToShowPage(BaseKard*kard);

    void create_toDoListKard();
    void create_NoteKard();

    void save_ToDoList();
    void save_Note();

    void on_btnBack_clicked();
    void on_btnDel_clicked();
    void on_btnDel_Note_clicked();
    void on_btnClear_clicked();
    void on_btnClear_2_clicked();
    void on_btnSetPath_clicked();


    void updateSelected(BaseKard* kard);
    void refreshCurKardStyle();

    void addFile();
    void openFile(const QModelIndex&index);
    void deleteFile();
    void getFilePath();
    void openFileLoc();
    void searchFile(const QString&fileName);
    void dragFile(QStringList& filePaths);

    void searchToDOList();
    void todo_receiveResults(QVector<toDoKard*>results);
    void searchNote();
    void note_receiveResults(QVector<NoteKard*>results);

    void sortNote(int index);
    void note_receiveSortResults(QVector<NoteKard*>results);

    void categoryBy(fileKard::Type type);
    void switchTheme(int index);
    
};

