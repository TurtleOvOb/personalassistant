cmake_minimum_required(VERSION 3.16)
project(personalassistant LANGUAGES CXX)



include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Դ�ļ�����ͳһΪ UTF-8(��ǩ�� BOM) ----
# 1) ���� .h/.cpp ����� UTF-8 with BOM��MSVC �� BOM �����ϳ� UTF-8�����ٰ����ش���ҳ�£�
# 2) /utf-8 �ȼ��� /source-charset:utf-8 /execution-charset:utf-8��
#    ��Դ���ִ����խ�ַ������� UTF-8������ע�ͺ� QString �������Ų������롣
add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,MSVC,MSVC-like>:/utf-8>)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Sql
)
qt_standard_project_setup()

qt_add_resources(APP_RESOURCES Resource.qrc)
set(PROJECT_SOURCES
    main.cpp
    ui/personalassistant.ui
    ui/personalassistant.h
    ui/personalassistant.cpp

    coreCard/BaseKard.h
    coreCard/BaseKard.cpp

    todo/toDoKard.h
    todo/toDoKard.cpp

    note/NoteKard.h
    note/NoteKard.cpp

    file/fileKard.h
    file/fileKard.cpp
    file/fileModel.h
    file/fileModel.cpp
    file/myTableview/myTableview.h
    file/myTableview/myTableview.cpp
    file/mySortModel/mySortModel.h
    file/mySortModel/mySortModel.cpp

    manager/todoManager.h
    manager/todoManager.cpp
    manager/noteManager.h
    manager/noteManager.cpp
    manager/fileManager.h
    manager/fileManager.cpp
    manager/themeManager.h
    manager/themeManager.cpp

    Dao/todoDao.h
    Dao/todoDao.cpp
    Dao/NoteDao.h
    Dao/NoteDao.cpp
    Dao/fileDao.h
    Dao/fileDao.cpp

    utils/DBManager.h
    utils/DBManager.cpp

    style/darkMode/todoList/button.qss
    style/darkMode/todoList/inputWidget.qss
    style/darkMode/todoList/widget.qss
    style/darkMode/note/button.qss
    style/darkMode/note/inputWidget.qss
    style/darkMode/note/widget.qss
    style/darkMode/fileManager/inputWidget.qss
    style/darkMode/fileManager/widget.qss
     style/darkMode/fileManager/button.qss
     style/darkMode/settings/widget.qss

    style/lightMode/todoList/button.qss
    style/lightMode/todoList/inputWidget.qss
    style/lightMode/todoList/widget.qss
    style/lightMode/note/button.qss
    style/lightMode/note/inputWidget.qss
    style/lightMode/note/widget.qss
    style/lightMode/fileManager/inputWidget.qss
    style/lightMode/fileManager/widget.qss
     style/lightMode/fileManager/button.qss
       style/lightMode/settings/widget.qss
    ${APP_RESOURCES}
)


qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})


target_include_directories(
    ${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}              # ��Ŀ��Ŀ¼
    ${CMAKE_CURRENT_SOURCE_DIR}/file/myTableview  # ר�������� m_ScrollArea
    ${CMAKE_CURRENT_SOURCE_DIR}/file/mySortModel
    # ����˳��������ļ���Ҳ���ϣ��Ժ��ڴ����� include �Ͳ���Ҫд������·����
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/coreCard
    ${CMAKE_CURRENT_SOURCE_DIR}/todo
    ${CMAKE_CURRENT_SOURCE_DIR}/note
    ${CMAKE_CURRENT_SOURCE_DIR}/file
    ${CMAKE_CURRENT_SOURCE_DIR}/manager
     ${CMAKE_CURRENT_SOURCE_DIR}/Dao
    ${CMAKE_CURRENT_SOURCE_DIR}/utils

)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE FALSE
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Sql
)

