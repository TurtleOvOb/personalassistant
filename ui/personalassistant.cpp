#include "personalassistant.h"
#include"../todo/toDoKard.h"
#include"../note/NoteKard.h"
#include"../file/fileKard.h"

personalassistant::personalassistant(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::personalassistantClass())
{    ui->setupUi(this);
    isCreate = 0;
    isCreate_Note = 0;
    isEdit = 0;
    model = new QStandardItemModel(this);
    sortModel = new mySortModel(this);
    todo = new todoManager();
    note = new noteManager();
    file = new fileManager();

    sortModel->setFilterKeyColumn(-1);
    sortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    QStringList labels;
    labels << "文件名" << "修改日期" << "类型" << "大小";
    model->setHorizontalHeaderLabels(labels);
    uiConfig();
    initSlots();
    readConfig();
    loadFromDataBase();
    fileKard::Type type[5] = { fileKard::Default ,fileKard::Doc,fileKard::Img ,fileKard::Code ,fileKard::Else };
    for (int i = 0; i < 5; i++) {
        fileKard* kard = file->createFileKard(this, type[i], themeManager::instance()->currentTheme());
        ui->verticalLayout_13->addWidget(kard);
        connect(kard, &fileKard::clicked, this, &personalassistant::categoryBy);
    }

}

personalassistant::~personalassistant()
{
    delete ui;
}

void personalassistant::uiConfig()
{

    ui->fileViewer->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->fileViewer->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fileViewer->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fileViewer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->fileViewer->setSortingEnabled(true);
    ui->fileViewer->verticalHeader()->setVisible(false);
    ui->fileViewer->setAlternatingRowColors(true);

    ui->input_DeadLine->setCalendarPopup(true);
    ui->input_DeadLine->setDisplayFormat("yyyy/MM/dd");
    ui->input_S_Description->setReadOnly(true);
    file->setModel(model);
    file->setSortModel(sortModel);
    ui->fileViewer->setModel(sortModel);

    ui->verticalLayout_13->setAlignment(Qt::AlignTop);
    ui->verticalLayout_23->setAlignment(Qt::AlignTop);
    ui->verticalLayout_22->setAlignment(Qt::AlignTop);
    cur_Selected_Kard = nullptr;

}

void personalassistant::initSlots()
{
    connect(ui->btnCreate_td, &QPushButton::clicked, this, &personalassistant::create_toDoListKard);
    connect(ui->btnCreate_Note, &QPushButton::clicked, this, &personalassistant::create_NoteKard);
    connect(ui->btnSave, &QPushButton::clicked, this, &personalassistant::save_ToDoList);
    connect(ui->btnSave_Note, &QPushButton::clicked, this, &personalassistant::save_Note);
    connect(ui->fileViewer, &QAbstractItemView::doubleClicked, this, &personalassistant::openFile);
    connect(ui->btnDelFile, &QPushButton::clicked, this, &personalassistant::deleteFile);
    connect(ui->btnToFileLoc, &QPushButton::clicked, this, &personalassistant::openFileLoc);
    connect(ui->btnGetFilePath, &QPushButton::clicked, this, &personalassistant::getFilePath);
    connect(ui->lineEdit_fileSearch, &QLineEdit::textChanged, this, &personalassistant::searchFile);
    connect(ui->fileViewer, &myTableview::done, this,&personalassistant::dragFile);
    connect(ui->btnSearch, &QPushButton::clicked, this, &personalassistant::searchToDOList);
    connect(todo, &todoManager::searchDone, this, &personalassistant::todo_receiveResults);
    connect(ui->btnSearch_2, &QPushButton::clicked, this, &personalassistant::searchNote);
    connect(note, &noteManager::searchDone, this, &personalassistant::note_receiveResults);
    connect(ui->comboBox_Note, &QComboBox::currentIndexChanged, this, &personalassistant::sortNote);
    connect(note, &noteManager::sortDone, this, &personalassistant::note_receiveSortResults);
    connect(ui->comboBox_Theme, &QComboBox::currentIndexChanged, this, &personalassistant::switchTheme);
    connect(ui->comboBox_Theme, &QComboBox::currentIndexChanged, themeManager::instance(), &themeManager::setTheme);

    connect(ui->btnTo_do_List, &QPushButton::clicked, this, &personalassistant::switchToMidPages);
    connect(ui->btnNoteManager, &QPushButton::clicked, this, &personalassistant::switchToMidPages);
    connect(ui->btnFileManager, &QPushButton::clicked, this, &personalassistant::switchToMidPages);
    connect(ui->btnSettings, &QPushButton::clicked, this, &personalassistant::switchToMidPages);
    connect(ui->btnEdit, &QPushButton::clicked, this, &personalassistant::switchToEditPage);
    connect(ui->btnAddFile, &QPushButton::clicked, this, &personalassistant::addFile);
}
//处理各种卡片的事务
void personalassistant::handleKardAction(BaseKard* kard)
{
    if (!kard)return;

    if (auto todoKard = qobject_cast<toDoKard*>(kard)) {
        //创建edit卡片状态下才修改卡片信息
        if (isCreate || isEdit) {
            //修改卡片信息
            todoKard->setTitle(ui->input_Title->text());
            todoKard->setDescription(ui->input_Description->toPlainText());
            todoKard->setDate(ui->input_DeadLine->date());
            todoKard->setPriority(ui->comboBox_Prior->currentText());
            todo->update_toDoKard(todoKard);
        }
        //填充显示页面
        ui->label_S_Title->setText(todoKard->Title());
        ui->input_S_Description->setPlainText(todoKard->Description());
        ui->label_S_DeadLine->setText(todoKard->Date().toString("yyyy/MM/dd"));
        ui->label_S_Priority->setText(todoKard->Priority());
        return;
    }
    //点击创建，isCreate_Note为1，btnSave_Note变为创建,isCreate_Note为0时，btnSave_Note变为保存
    if (auto noteKard = qobject_cast<NoteKard*>(kard)) {
        noteKard->setTitle(ui->input_Notetitle->text());
        noteKard->setDes(ui->input_Note->toPlainText());
        noteKard->setType(ui->noteTypeBox->currentIndex(),themeManager::instance()->currentTheme());
        note->update_noteKard(noteKard);
    }

}
//填充编辑页面（toDoList））
void personalassistant::fillEditpage(BaseKard* kard)
{
    if (auto todo = qobject_cast<toDoKard*>(cur_Selected_Kard)) {
        ui->input_Title->setText(todo->Title());
        ui->input_Description->setText(todo->Description());
        ui->input_DeadLine->setDate(todo->Date());
        ui->comboBox_Prior->setCurrentIndex(0);
    }
}

void personalassistant::loadFromDataBase()
{//从数据库添加todokard
    int count = 0;
    QVector<toDoKard*>todoKardList = todo->loadFromDataBase(this);
    QVector<NoteKard*>noteKardList = note->loadFromDataBase(this);
    QVector<QString>filePaths = file->loadFromDataBase();
    for (toDoKard* kard : todoKardList) {
        connect(kard, &toDoKard::doubleClicked, this, &personalassistant::switchToShowPage);
        ui->verticalLayout_23->addWidget(kard);
        count++;
    }
    for (NoteKard* kard : noteKardList) {
        connect(kard, &NoteKard::doubleClicked, this, &personalassistant::switchToShowPage);
        ui->verticalLayout_22->addWidget(kard);
        count++;
    }
    QString fileName;
    QString lastTime;
    QString fileType;
    qint64 fileSize;
    for (QString filePath : filePaths) {
        QFileInfo info(filePath);
        fileName = info.fileName();
        lastTime = info.lastModified().toString("yyyy-MM-dd hh:mm:ss");
        if (info.isDir()) {
            fileType = "director";
        };
        fileType = info.suffix();
        fileSize = info.size();
        QList<QStandardItem*>rowInfo;
        rowInfo.append(new QStandardItem(fileName));
        rowInfo.append(new QStandardItem(lastTime));
        rowInfo.append(new QStandardItem(fileType));
        rowInfo.append(new QStandardItem(QString::number(fileSize)));
        model->appendRow(rowInfo);
    }
qDebug() << "已成功加载" << count << "条数据";
}

void personalassistant::readConfig()
{
    QFile file("config.ini");
    if (!file.exists())return;
    QSettings config("config.ini", QSettings::IniFormat);

    config.beginGroup("AppGeometry");
    QRect geometry=config.value("geometry").toRect();
    setGeometry(geometry);
    config.endGroup();


    config.beginGroup("AppTheme");
    int themeIndex = config.value("theme").toInt();
    ui->comboBox_Theme->setCurrentIndex(themeIndex);
    //switchTheme(themeIndex);
    //themeManager::instance()->setTheme(themeIndex);
    config.endGroup();

    config.beginGroup("DefalutFilePath");
    QString path = config.value("path").toString();
    ui->input_DefalutPath->setText(path);
    config.endGroup();
   
    config.beginGroup("lastPage");
    int lastMidPage = config.value("lastMidPage").toInt();
    int lastRightPage = config.value("lastRightPage").toInt();
    if (lastRightPage == 4) {
        ui->midStack->hide();
    }
    ui->midStack->setCurrentIndex(lastMidPage);
    ui->stackedWidget->setCurrentIndex(lastRightPage);
    config.endGroup();
}

void personalassistant::closeEvent(QCloseEvent* event)
{
    qDebug() << "开始保存配置";
    QSettings config("config.ini", QSettings::IniFormat);
    config.beginGroup("AppGeometry");
    config.setValue("geometry", geometry());
    config.endGroup();
    config.beginGroup("AppTheme");
    config.setValue("theme", ui->comboBox_Theme->currentIndex());
    config.endGroup();
    config.beginGroup("lastPage");
    config.setValue("lastMidPage",ui->midStack->currentIndex());
    config.setValue("lastRightPage", ui->stackedWidget->currentIndex());
    qDebug() << "保存完毕,关闭中";
    QWidget::closeEvent(event);
}
//切换到创建页面
void personalassistant::create_toDoListKard()
{
    isCreate = 1;
    isEdit = 0;
    ui->stackedWidget->setCurrentIndex(0);
    ui->input_Title->clear();
    ui->input_Description->clear();
    ui->input_DeadLine->setDate(QDate::currentDate());
    ui->comboBox_Prior->setCurrentIndex(0);
    ui->btnSave->setText("创建");
}

void personalassistant::create_NoteKard()
{
    qDebug() << "创建NoteKard中";
    isCreate_Note = 1;
    ui->stackedWidget->setCurrentIndex(2);
    ui->input_Title->clear();
    ui->input_Note->clear();
    ui->btnSave_Note->setText("创建");
}
//创建卡片
void personalassistant::save_ToDoList()
{
    if(isEdit){
        handleKardAction(cur_Selected_Kard);
        ui->stackedWidget->setCurrentIndex(1);
        isEdit = 0;
        return;
    }
    //添加卡片
    toDoKard* kard =todo->create_toDoKard(
    this, ui->input_Title->text(), ui->input_Description->toPlainText(), 
    ui->input_DeadLine->date(), ui->comboBox_Prior->currentText());
  

    ui->verticalLayout_23->addWidget(kard);
    //刷新toDoList卡片的选中效果
    if (cur_Selected_Kard != nullptr) {
    cur_Selected_Kard->setProperty("selected", false);
    refreshCurKardStyle();
    }
    cur_Selected_Kard = kard;
    cur_Selected_Kard->setProperty("selected", true);
    refreshCurKardStyle();
    //connect(kard,&toDoKard::clicked,this, &personalassistant::updateSelected);
    connect(kard, &toDoKard::doubleClicked, this, &personalassistant::switchToShowPage);
    isCreate = 0;
    //on_btnClear_clicked();
    handleKardAction(cur_Selected_Kard);
    ui->stackedWidget->setCurrentIndex(1);
    ui->btnSave->setText("保存");
}

void personalassistant::save_Note()
{
    //如果只是编辑，则只修改卡片信息
    if (!isCreate_Note) {
        qDebug() << "保存Note";
        handleKardAction(cur_Selected_Kard);
        return;
    }
    //创建卡片
    NoteKard* kard = note->create_NoteKard(
        this,
        ui->input_Notetitle->text(),
        ui->input_Note->toPlainText(),
        ui->noteTypeBox->currentIndex(),
        themeManager::instance()->currentTheme()
        );
    //在添加时根据类型决定是否显示在Layout中，而不是直接添加到layout后再每次都调用排序
    ui->verticalLayout_22->addWidget(kard);
    if (cur_Selected_Kard != nullptr) {
        cur_Selected_Kard->setProperty("selected", false);
        refreshCurKardStyle();
    }
    cur_Selected_Kard = kard;
    cur_Selected_Kard->setProperty("selected", true);
    refreshCurKardStyle();
    //connect(kard, &NoteKard::clicked, this, &personalassistant::updateSelected);
    connect(kard, &NoteKard::doubleClicked, this, &personalassistant::switchToShowPage);
    isCreate_Note = 0;
    ui->btnSave_Note->setText("保存");
    sortNote(ui->comboBox_Note->currentIndex());

}

void personalassistant::on_btnBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    isCreate = isEdit = 0;


}
//删除按钮信号连接
void personalassistant::on_btnDel_clicked()
{
    if (cur_Selected_Kard != nullptr) {
        //值传递
 ui->verticalLayout_23->removeWidget(cur_Selected_Kard);
 todo->delete_toDoKard(cur_Selected_Kard);
 cur_Selected_Kard = nullptr;
    }
    else {
        qDebug() << "delete fail:" << "none todoKard selected";
    }
}

void personalassistant::on_btnDel_Note_clicked()
{
    if (cur_Selected_Kard == nullptr)return;
    qDebug() << "deleting curKard";
    ui->verticalLayout_22->removeWidget(cur_Selected_Kard);
    note->delete_NoteKard(cur_Selected_Kard);
    cur_Selected_Kard = nullptr;
    qDebug() << "delete success";

}

void personalassistant::on_btnClear_clicked()
{
    ui->input_Search->clear();
    while (QLayoutItem* item = ui->verticalLayout_23->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            qDebug() << widget;
            widget->hide();
        }
        delete item;
    }
   
  QVector<toDoKard*>kardList=todo->getKardList();
    for (toDoKard* kard : kardList) {
        if (kard) {
            kard->show();
            //记得将kard重新添加回布局
            ui->verticalLayout_23->addWidget(kard);
        }
    }
   

}

void personalassistant::on_btnClear_2_clicked() {
    ui->input_Search_2->clear();
    while (QLayoutItem* item = ui->verticalLayout_22->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            qDebug() << widget;
            widget->hide();
        }
        delete item;
    }

    QVector<NoteKard*>kardList = note->getNoteKardList();
    for (NoteKard* kard : kardList) {
        if (kard) {
            kard->show();
            ui->verticalLayout_22->addWidget(kard);
        }
    }

}

void personalassistant::on_btnSetPath_clicked()
{
    QSettings config("config.ini", QSettings::IniFormat);
   QString defaultPath=QFileDialog::getExistingDirectory(this);
   ui->input_DefalutPath->setPlaceholderText(defaultPath);
   config.beginGroup("DefalutFilePath");
   config.setValue("path", defaultPath);
   config.endGroup();

}
//刷新卡片qss
void personalassistant::updateSelected(BaseKard* kard)
{
    qDebug() << cur_Selected_Kard<<"   " << kard;
    if (!kard)return;
    //消除旧的选中的卡片的选中状态
    if (cur_Selected_Kard && cur_Selected_Kard != kard) {
        cur_Selected_Kard->setProperty("selected", false);
        refreshCurKardStyle();
    }
    cur_Selected_Kard = kard;
    cur_Selected_Kard->setProperty("selected", true);
    
    refreshCurKardStyle();
    qDebug() << "卡片选中效果已更新";
}

void personalassistant::refreshCurKardStyle()
{
    cur_Selected_Kard->style()->unpolish(cur_Selected_Kard);
    cur_Selected_Kard->style()->polish(cur_Selected_Kard);
}
//文件操作
void personalassistant::addFile()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this);
    file->loadFile(filePaths);
}

void personalassistant::openFile(const QModelIndex& index)
{
    if (!index.isValid()) {
        ui->label_curState->setText("未选中任何文件");
        return;
    }
    if (file->openFile(index)) {
        ui->label_curState->setText("文件打开成功");
        return;
    }
    ui->label_curState->setText("文件打开失败");
}

void personalassistant::deleteFile()
{
    //弹窗提示是否删除？
  QModelIndex index=ui->fileViewer->currentIndex();
  if (!index.isValid()) {
      ui->label_curState->setText("未选中任何文件");
      return;
  }
  if (file->deleteFile(index)) {
      ui->label_curState->setText("删除成功");
  }
}

void personalassistant::getFilePath()
{


    QModelIndex index = ui->fileViewer->currentIndex();
    if (index.isValid()) {
    QString filePath= file->getFilePath(index);
    if (filePath.isEmpty()) {
        ui->label_curState->setText("获取路径失败");
        return;
    }
    QClipboard* clip = QApplication::clipboard();
    clip->setText(filePath);
    ui->label_curState->setText("已复制文件路径");
    }
    else {
        ui->label_curState->setText("未选中任何文件");
    }

}

void personalassistant::openFileLoc()
{
    QModelIndex index = ui->fileViewer->currentIndex();
    if (index.isValid()) {
        if (file->openFileLoc(index)) {
            ui->label_curState->setText("打开文件位置成功");
            return;
        };
        ui->label_curState->setText("打开文件位置失败！");
    }
    else {
        ui->label_curState->setText("未选中任何文件");
    }
}

void personalassistant::searchFile(const QString& fileName)
{
    file->searchFile(fileName);

    qDebug() << "personalassistant::searchFile";
}

void personalassistant::dragFile(QStringList& filePaths)
{
    file->loadFile(filePaths);
}
//界面切换
void personalassistant::switchToMidPages() {
 
  QString btnName=QObject::sender()->objectName();
  qDebug() << "btn clicked!" << " btnName:" << btnName;
  int index = 0,rightIndex=0;
    ui->midStack->show();
  if (btnName == ui->btnTo_do_List->objectName()) {
  
      index = 0;
      rightIndex = 1;
   
  }
  else if (btnName == ui->btnNoteManager->objectName()) {
      index = 1;
      rightIndex = 2;

  }
  else if (btnName == ui->btnFileManager->objectName()) {
      index = 2;
      rightIndex = 3;

  }
  else if (btnName == ui->btnSettings->objectName()) {
      ui->midStack->hide();
      rightIndex = 4;
  }
  ui->midStack->setCurrentIndex(index);
  ui->stackedWidget->setCurrentIndex(rightIndex);
}
//只有todoList的编辑按钮才会触发，后面给该函数改名
void personalassistant::switchToEditPage()
{
    QString btnName = QObject::sender()->objectName();
    qDebug() << "btn clicked!" << " btnName:" << btnName;
    int index = 0;
    if (btnName == ui->btnEdit->objectName()) {
        isEdit = 1;
        index = 0;
    }
    //获取当前选中的卡片信息并填充到编辑页面
    //点击edit，填充旧卡片信息到编辑页面，点击save后保存新卡片信息，isCreate始终为0
    //只负责填充edit页面的分流器
    fillEditpage(cur_Selected_Kard);
    ui->stackedWidget->setCurrentIndex(index);
}

void personalassistant::switchToShowPage(BaseKard* kard)
{    
    updateSelected(kard);
    if (qobject_cast<toDoKard*>(kard)) {
        qDebug() << "update curKard clicked";
        isCreate = isEdit = 0;
        //cur_Selected_Kard = kard;
        handleKardAction(cur_Selected_Kard);
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if (auto note =qobject_cast<NoteKard*>(kard)) {
        isCreate_Note = 0;
        //cur_Selected_Kard = kard;
        qDebug() << "选中当前Notekard";
        //填充Note卡片信息到当前页面
        qDebug() << note->Title() << note->Des() << note->type;
        ui->input_Notetitle->setText(note->Title());
        ui->input_Note->setPlainText(note->Des());
        ui->noteTypeBox->setCurrentIndex(note->type);
    }

}

void personalassistant::switchTheme(int index)
{
    QString qss;
    qss = themeManager::instance()->switchTheme(index);
    setStyleSheet(qss);
    qDebug() << "样式加载成功";
}

//卡片搜索
void personalassistant::searchToDOList()
{
    
    todo->search_toDoKard(ui->input_Search->text());
}

void personalassistant::todo_receiveResults(QVector<toDoKard*> results)
{
    //这个地方要研究一下qt的布局机制（布局是怎么把卡片存储，显示的）
    qDebug() << results.size();
    qDebug() << "移除现有widget";//takeAt拿走布局，只是不在管理卡片的位置，卡片本身还在内存中
    while (QLayoutItem* item = ui->verticalLayout_23->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            qDebug() << widget;
            widget->hide();
       }
        delete item;
    }
    qDebug() << "添加新widget";
    for (toDoKard* kard : results) {
        if (kard) {
            kard->show();
            //记得将kard重新添加回布局
            ui->verticalLayout_23->addWidget(kard);
        }
    }
    qDebug() << "添加完成";
}

void personalassistant::searchNote()
{
    note->search_NoteKard(ui->input_Search_2->text());
}

void personalassistant::note_receiveResults(QVector<NoteKard*> results)
{
    while (QLayoutItem* item = ui->verticalLayout_22->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            qDebug() << widget;
            widget->hide();
        }
        delete item;
    }
    for (NoteKard* kard : results) {
        if (kard) {
            kard->show();
            //记得将kard重新添加回布局
            ui->verticalLayout_22->addWidget(kard);
        }
    }
}

void personalassistant::sortNote(int index)
{
    note->sortBy(index);
}

void personalassistant::note_receiveSortResults(QVector<NoteKard*> results)
{
    while (QLayoutItem* item = ui->verticalLayout_22->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            qDebug() << widget;
            widget->hide();
        }
        delete item;
    }
    for (NoteKard* kard : results) {
        if (kard) {
            kard->show();
            //记得将kard重新添加回布局
            ui->verticalLayout_22->addWidget(kard);
        }
    }

}

void personalassistant::categoryBy(fileKard::Type type)
{

    sortModel->setType(type);
}
