#include <QtTest>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QFile>
#include <memory>
#include "fileDao.h"

// fileDao 的单元测试。
// 数据库通过 DBManager::testInstance() 切到 %TEMP%\pa_unit_test.db，
// 不碰 AppData 里的真实 kard.db。
class TestFileDao : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();

    // ---- normalizePath：static 纯函数，不碰数据库 ----
    void normalizePath_emptyReturnsEmpty();
    void normalizePath_backslashesBecomeForwardSlashes();
    void normalizePath_uppercaseBecomesLowercase();
    void normalizePath_samePathTwoSpellingsMatch();
    void normalizePath_rootPathBoundary();
    void normalizePath_unicodePreserved();

    // ---- addFile ----
    void addFile_absolutePathReturnsPositiveId();
    void addFile_emptyPathReturnsMinusOne();
    void addFile_duplicateReturnsMinusOne();
    void addFile_differentSpellingsAreDuplicate();
    void addFile_storesNormalizedPath();
    void addFile_longPathBoundary();
    void addFile_unicodePath();

    // ---- deleteFile ----
    void deleteFile_existingIdRemovesRow();
    void deleteFile_negativeIdReturnsFalse();
    void deleteFile_missingIdCurrentBehavior();
    void deleteFile_twiceSecondCallCurrentBehavior();

    // ---- getFileInfos ----
    void getFileInfos_emptyTableReturnsEmpty();
    void getFileInfos_returnsRowsOrderedById();
    void getFileInfos_afterDeleteShrinks();

    // ---- removeInvalidRows ----
    void removeInvalidRows_dropsRelativePath();
    void removeInvalidRows_keepsAbsolutePath();
    void removeInvalidRows_emptyTableIsNoop();

private:
    std::unique_ptr<fileDao> m_dao;
};

// ===================== 生命周期 =====================

void TestFileDao::initTestCase()
{
    // 先把 DBManager 切到临时库，再构造 fileDao（构造里会建表）。
    // 顺序不能反：反了就会在真实 kard.db 上建表。
    DBManager::testInstance();
    m_dao.reset(new fileDao());
}

void TestFileDao::init()
{
    // 每个用例前清空表 + 重置自增，保证用例之间互不影响。
    QSqlDatabase db = DBManager::testInstance();
    QVERIFY2(db.open(), qPrintable(db.lastError().text()));
    QSqlQuery q(db);
    QVERIFY2(q.exec("DELETE FROM fileInfo"), qPrintable(q.lastError().text()));
    // sqlite_sequence 只在建过 AUTOINCREMENT 表之后才存在，失败无所谓
    q.exec("DELETE FROM sqlite_sequence WHERE name='fileInfo'");
    db.close();
}

void TestFileDao::cleanupTestCase()
{
    m_dao.reset();
    QFile::remove(QDir(QDir::tempPath()).filePath("pa_unit_test.db"));
}

// ===================== normalizePath =====================

void TestFileDao::normalizePath_emptyReturnsEmpty()
{
    QVERIFY(fileDao::normalizePath(QString()).isEmpty());
}

void TestFileDao::normalizePath_backslashesBecomeForwardSlashes()
{
    // 用不存在的目录，保证走 absoluteFilePath 分支，结果与磁盘状态无关
    const QString out = fileDao::normalizePath("C:\\__pa_no_such_dir__\\a.txt");
    QVERIFY2(!out.contains('\\'), qPrintable(out));
    QVERIFY2(out.startsWith("c:/"), qPrintable(out));
    QVERIFY2(out.endsWith("a.txt"), qPrintable(out));
}

void TestFileDao::normalizePath_uppercaseBecomesLowercase()
{
    const QString upper = fileDao::normalizePath("C:\\__pa_no_such_dir__\\A.TXT");
    const QString lower = fileDao::normalizePath("C:\\__pa_no_such_dir__\\a.txt");
    QCOMPARE(upper, lower);
}

void TestFileDao::normalizePath_samePathTwoSpellingsMatch()
{
    // 同一路径的两种写法必须归一，否则 addFile 的去重查询匹配不上
    QCOMPARE(fileDao::normalizePath("C:\\__pa_no_such_dir__\\a.txt"),
             fileDao::normalizePath("C:/__pa_no_such_dir__/a.txt"));
}

void TestFileDao::normalizePath_rootPathBoundary()
{
    // 边界：只有根目录，没有文件名
    const QString out = fileDao::normalizePath("C:\\__pa_root_only__\\");
    QVERIFY2(!out.contains('\\'), qPrintable(out));
    QVERIFY2(out.startsWith("c:/"), qPrintable(out));
}

void TestFileDao::normalizePath_unicodePreserved()
{
    const QString out = fileDao::normalizePath("C:/__pa_no_such_dir__/中文文件名.txt");
    QVERIFY2(out.contains(QString::fromUtf8("中文文件名")), qPrintable(out));
    QVERIFY2(out.endsWith(QString::fromUtf8(".txt")), qPrintable(out));
}

// ===================== addFile =====================

void TestFileDao::addFile_absolutePathReturnsPositiveId()
{
    QVERIFY(m_dao->addFile("C:/__pa_test__/one.txt") > 0);
}

void TestFileDao::addFile_emptyPathReturnsMinusOne()
{
    QCOMPARE(m_dao->addFile(QString()), -1);
    QCOMPARE(m_dao->getFileInfos().size(), 0);
}

void TestFileDao::addFile_duplicateReturnsMinusOne()
{
    QVERIFY(m_dao->addFile("C:/__pa_test__/dup.txt") > 0);
    QCOMPARE(m_dao->addFile("C:/__pa_test__/dup.txt"), -1);
    QCOMPARE(m_dao->getFileInfos().size(), 1);
}

void TestFileDao::addFile_differentSpellingsAreDuplicate()
{
    // 反斜杠 + 大写 vs 正斜杠 + 小写，必须认成同一个文件
    QVERIFY(m_dao->addFile("C:\\__pa_test__\\Same.TXT") > 0);
    QCOMPARE(m_dao->addFile("c:/__pa_test__/same.txt"), -1);
    QCOMPARE(m_dao->getFileInfos().size(), 1);
}

void TestFileDao::addFile_storesNormalizedPath()
{
    QVERIFY(m_dao->addFile("C:\\__pa_test__\\Mixed.TXT") > 0);
    const QVector<fileRecord> infos = m_dao->getFileInfos();
    QCOMPARE(infos.size(), 1);
    QCOMPARE(infos.at(0).path, QString("c:/__pa_test__/mixed.txt"));
}

void TestFileDao::addFile_longPathBoundary()
{
    // 边界：Windows 传统 MAX_PATH 是 260，这里放 300 字符看会不会被截断
    const QString longPath = "C:/__pa_test__/" + QString(300, QLatin1Char('a')) + ".txt";
    QVERIFY(m_dao->addFile(longPath) > 0);
    const QVector<fileRecord> infos = m_dao->getFileInfos();
    QCOMPARE(infos.size(), 1);
    QCOMPARE(infos.at(0).path.length(), longPath.length());
}

void TestFileDao::addFile_unicodePath()
{
    QVERIFY(m_dao->addFile("C:/__pa_test__/中文 空格.txt") > 0);
    const QVector<fileRecord> infos = m_dao->getFileInfos();
    QCOMPARE(infos.size(), 1);
    QVERIFY2(infos.at(0).path.contains(QString::fromUtf8("中文 空格")),
             qPrintable(infos.at(0).path));
}

// ===================== deleteFile =====================

void TestFileDao::deleteFile_existingIdRemovesRow()
{
    const int id = m_dao->addFile("C:/__pa_test__/del.txt");
    QVERIFY(id > 0);
    QVERIFY(m_dao->deleteFile(id));
    QCOMPARE(m_dao->getFileInfos().size(), 0);
}

void TestFileDao::deleteFile_negativeIdReturnsFalse()
{
    // 下界边界：id < 0 直接被拒
    QVERIFY(!m_dao->deleteFile(-1));
}

void TestFileDao::deleteFile_missingIdCurrentBehavior()
{
    QVERIFY(!m_dao->deleteFile(999999));
}

void TestFileDao::deleteFile_twiceSecondCallCurrentBehavior()
{
    // 同上，第二次删除同样返回 true
    const int id = m_dao->addFile("C:/__pa_test__/twice.txt");
    QVERIFY(id > 0);
    QVERIFY(m_dao->deleteFile(id));
    QVERIFY(!m_dao->deleteFile(id));
}

// ===================== getFileInfos =====================

void TestFileDao::getFileInfos_emptyTableReturnsEmpty()
{
    QCOMPARE(m_dao->getFileInfos().size(), 0);
}

void TestFileDao::getFileInfos_returnsRowsOrderedById()
{
    QVERIFY(m_dao->addFile("C:/__pa_test__/a.txt") > 0);
    QVERIFY(m_dao->addFile("C:/__pa_test__/b.txt") > 0);
    QVERIFY(m_dao->addFile("C:/__pa_test__/c.txt") > 0);

    const QVector<fileRecord> infos = m_dao->getFileInfos();
    QCOMPARE(infos.size(), 3);
    for (int i = 1; i < infos.size(); ++i) {
        QVERIFY2(infos.at(i - 1).id < infos.at(i).id,
                 "getFileInfos 必须按 id 升序返回");
    }
}

void TestFileDao::getFileInfos_afterDeleteShrinks()
{
    const int id1 = m_dao->addFile("C:/__pa_test__/keep.txt");
    const int id2 = m_dao->addFile("C:/__pa_test__/drop.txt");
    QVERIFY(id1 > 0 && id2 > 0);
    QVERIFY(m_dao->deleteFile(id2));

    const QVector<fileRecord> infos = m_dao->getFileInfos();
    QCOMPARE(infos.size(), 1);
    QCOMPARE(infos.at(0).id, id1);
    QCOMPARE(infos.at(0).path, QString("c:/__pa_test__/keep.txt"));
}

// ===================== removeInvalidRows =====================

void TestFileDao::removeInvalidRows_dropsRelativePath()
{
    // 绕过 addFile 直接插脏数据，模拟历史遗留的「只存文件名」记录
    QSqlDatabase db = DBManager::testInstance();
    QVERIFY(db.open());
    QSqlQuery q(db);
    QVERIFY2(q.exec("INSERT INTO fileInfo(filePath) VALUES('just_a_name.txt')"),
             qPrintable(q.lastError().text()));
    db.close();

    m_dao->removeInvalidRows();
    QCOMPARE(m_dao->getFileInfos().size(), 0);
}

void TestFileDao::removeInvalidRows_keepsAbsolutePath()
{
    QSqlDatabase db = DBManager::testInstance();
    QVERIFY(db.open());
    QSqlQuery q(db);
    QVERIFY2(q.exec("INSERT INTO fileInfo(filePath) VALUES('c:/__pa_test__/keep.txt')"),
             qPrintable(q.lastError().text()));
    db.close();

    m_dao->removeInvalidRows();
    QCOMPARE(m_dao->getFileInfos().size(), 1);
}

void TestFileDao::removeInvalidRows_emptyTableIsNoop()
{
    m_dao->removeInvalidRows();
    QCOMPARE(m_dao->getFileInfos().size(), 0);
}

QTEST_GUILESS_MAIN(TestFileDao)
#include "tst_filedao.moc"