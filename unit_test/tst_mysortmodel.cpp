#include <QtTest>
#include <QStandardItemModel>
#include <QStringList>
#include "mySortModel.h"

class TestMySortModel : public QObject
{
    Q_OBJECT

private:
    static void addRow(QStandardItemModel& m, const QString& name, const QString& suffix)
    {
        QList<QStandardItem*> cells;
        cells << new QStandardItem(name)
              << new QStandardItem(QStringLiteral("0"))
              << new QStandardItem(suffix);
        m.appendRow(cells);
    }

private slots:
    void defaultType_showsAllRows();
    void docType_keepsOnlyDocSuffixes();
    void imgType_keepsOnlyImgSuffixes();
    void codeType_keepsOnlyCodeSuffixes();
    void elseType_keepsOnlyUnknownSuffixes();
    void emptySuffix_treatedAsElse();
    void unknownSuffix_notMatchedByDocOrCode();
    void search_matchesSubstringOfName();
    void search_emptyPattern_showsAllRows();
    void search_noMatch_hidesAllRows();
    void search_isCaseSensitive_currentBehavior();
    void uppercaseSuffix_normalizedAndMatched();
    void searchAndType_areCombinedWithAnd();
    void setType_reEvaluatesImmediately();
    void emptySourceModel_returnsZeroRowCount();
};

void TestMySortModel::defaultType_showsAllRows()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("a.pdf"), QStringLiteral("pdf"));
    addRow(src, QStringLiteral("b.png"), QStringLiteral("png"));
    addRow(src, QStringLiteral("c.xyz"), QStringLiteral("xyz"));

    mySortModel proxy;
    proxy.setSourceModel(&src);

    QCOMPARE(proxy.rowCount(), 3);
}

void TestMySortModel::docType_keepsOnlyDocSuffixes()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("a.pdf"), QStringLiteral("pdf"));
    addRow(src, QStringLiteral("b.txt"), QStringLiteral("txt"));
    addRow(src, QStringLiteral("c.docx"), QStringLiteral("docx"));
    addRow(src, QStringLiteral("d.png"), QStringLiteral("png"));
    addRow(src, QStringLiteral("e.cpp"), QStringLiteral("cpp"));

    mySortModel proxy;
    proxy.setSourceModel(&src);
    proxy.setType(fileKard::Doc);

    QCOMPARE(proxy.rowCount(), 3);
}

void TestMySortModel::imgType_keepsOnlyImgSuffixes()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("a.png"), QStringLiteral("png"));
    addRow(src, QStringLiteral("b.svg"), QStringLiteral("svg"));
    addRow(src, QStringLiteral("c.gif"), QStringLiteral("gif"));
    addRow(src, QStringLiteral("d.jpg"), QStringLiteral("jpg"));
    addRow(src, QStringLiteral("e.pdf"), QStringLiteral("pdf"));

    mySortModel proxy;
    proxy.setSourceModel(&src);
    proxy.setType(fileKard::Img);

    QCOMPARE(proxy.rowCount(), 4);
}

void TestMySortModel::codeType_keepsOnlyCodeSuffixes()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("a.cpp"), QStringLiteral("cpp"));
    addRow(src, QStringLiteral("b.java"), QStringLiteral("java"));
    addRow(src, QStringLiteral("c.py"), QStringLiteral("py"));
    addRow(src, QStringLiteral("d.h"), QStringLiteral("h"));
    addRow(src, QStringLiteral("e.png"), QStringLiteral("png"));

    mySortModel proxy;
    proxy.setSourceModel(&src);
    proxy.setType(fileKard::Code);

    QCOMPARE(proxy.rowCount(), 4);
}

void TestMySortModel::elseType_keepsOnlyUnknownSuffixes()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("a.pdf"), QStringLiteral("pdf"));
    addRow(src, QStringLiteral("b.png"), QStringLiteral("png"));
    addRow(src, QStringLiteral("c.cpp"), QStringLiteral("cpp"));
    addRow(src, QStringLiteral("d.xyz"), QStringLiteral("xyz"));
    addRow(src, QStringLiteral("e.mp4"), QStringLiteral("mp4"));

    mySortModel proxy;
    proxy.setSourceModel(&src);
    proxy.setType(fileKard::Else);

    QCOMPARE(proxy.rowCount(), 2);
}

void TestMySortModel::emptySuffix_treatedAsElse()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("noext"), QString());
    addRow(src, QStringLiteral("a.pdf"), QStringLiteral("pdf"));

    mySortModel proxy;
    proxy.setSourceModel(&src);
    proxy.setType(fileKard::Else);

    QCOMPARE(proxy.rowCount(), 1);
}

void TestMySortModel::unknownSuffix_notMatchedByDocOrCode()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("a.xyz"), QStringLiteral("xyz"));

    mySortModel proxy;
    proxy.setSourceModel(&src);

    proxy.setType(fileKard::Doc);
    QCOMPARE(proxy.rowCount(), 0);

    proxy.setType(fileKard::Code);
    QCOMPARE(proxy.rowCount(), 0);
}

void TestMySortModel::search_matchesSubstringOfName()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("apple.txt"), QStringLiteral("txt"));
    addRow(src, QStringLiteral("banana.txt"), QStringLiteral("txt"));

    mySortModel proxy;
    proxy.setSourceModel(&src);
    proxy.setFilterFixedString(QStringLiteral("app"));

    QCOMPARE(proxy.rowCount(), 1);
}

void TestMySortModel::search_emptyPattern_showsAllRows()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("apple.txt"), QStringLiteral("txt"));
    addRow(src, QStringLiteral("banana.txt"), QStringLiteral("txt"));

    mySortModel proxy;
    proxy.setSourceModel(&src);
    proxy.setFilterFixedString(QString());

    QCOMPARE(proxy.rowCount(), 2);
}

void TestMySortModel::search_noMatch_hidesAllRows()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("apple.txt"), QStringLiteral("txt"));

    mySortModel proxy;
    proxy.setSourceModel(&src);
    proxy.setFilterFixedString(QStringLiteral("zzz"));

    QCOMPARE(proxy.rowCount(), 0);
}

void TestMySortModel::search_isCaseSensitive_currentBehavior()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("Apple.txt"), QStringLiteral("txt"));

    mySortModel proxy;
    proxy.setSourceModel(&src);
    proxy.setFilterFixedString(QStringLiteral("apple"));

    QCOMPARE(proxy.rowCount(), 0);
}

void TestMySortModel::uppercaseSuffix_normalizedAndMatched()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("PHOTO.PDF"), QStringLiteral("PDF"));

    mySortModel proxy;
    proxy.setSourceModel(&src);
    proxy.setType(fileKard::Doc);

    // filterAcceptsRow 已对 suffix 做 toLower()，大写后缀要能被认出来
    QCOMPARE(proxy.rowCount(), 1);
}

void TestMySortModel::searchAndType_areCombinedWithAnd()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("note_a.pdf"), QStringLiteral("pdf"));
    addRow(src, QStringLiteral("note_b.png"), QStringLiteral("png"));
    addRow(src, QStringLiteral("other_a.pdf"), QStringLiteral("pdf"));

    mySortModel proxy;
    proxy.setSourceModel(&src);
    proxy.setType(fileKard::Doc);
    proxy.setFilterFixedString(QStringLiteral("note"));

    QCOMPARE(proxy.rowCount(), 1);
}

void TestMySortModel::setType_reEvaluatesImmediately()
{
    QStandardItemModel src;
    addRow(src, QStringLiteral("a.pdf"), QStringLiteral("pdf"));
    addRow(src, QStringLiteral("b.png"), QStringLiteral("png"));

    mySortModel proxy;
    proxy.setSourceModel(&src);
    QCOMPARE(proxy.rowCount(), 2);

    proxy.setType(fileKard::Doc);
    QCOMPARE(proxy.rowCount(), 1);

    proxy.setType(fileKard::Default);
    QCOMPARE(proxy.rowCount(), 2);
}

void TestMySortModel::emptySourceModel_returnsZeroRowCount()
{
    QStandardItemModel src;

    mySortModel proxy;
    proxy.setSourceModel(&src);
    proxy.setType(fileKard::Doc);

    QCOMPARE(proxy.rowCount(), 0);
}

QTEST_GUILESS_MAIN(TestMySortModel)
#include "tst_mysortmodel.moc"