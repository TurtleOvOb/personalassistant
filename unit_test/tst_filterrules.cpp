#include <QtTest>
#include "filterRules.h"

class TestFilterRules : public QObject
{
    Q_OBJECT

private slots:
    // ---- matchesTitle ----
    void matchesTitle_emptyKeyword_matchesAll();
    void matchesTitle_substringInMiddle_matches();
    void matchesTitle_prefix_matches();
    void matchesTitle_suffix_matches();
    void matchesTitle_notContained_doesNotMatch();
    void matchesTitle_isCaseSensitive_currentBehavior();
    void matchesTitle_chineseSubstring_matches();
    void matchesTitle_emptyTitle_withKeyword_doesNotMatch();
    void matchesTitle_emptyTitle_emptyKeyword_matches();
    void matchesTitle_wholeTitleExact_matches();

    // ---- matchesType ----
    void matchesType_defaultFilter_matchesAllTypes();
    void matchesType_sameType_matches();
    void matchesType_differentType_doesNotMatch();
    void matchesType_negativeKardType_doesNotMatch();
    void matchesType_outOfRangeFilter_doesNotMatch();
};

// ==================== matchesTitle ====================

void TestFilterRules::matchesTitle_emptyKeyword_matchesAll()
{
    QVERIFY(filterRules::matchesTitle(QStringLiteral("任意标题"), QString()));
    QVERIFY(filterRules::matchesTitle(QString(), QString()));
}

void TestFilterRules::matchesTitle_substringInMiddle_matches()
{
    QVERIFY(filterRules::matchesTitle(QStringLiteral("下周算法作业"), QStringLiteral("算法")));
}

void TestFilterRules::matchesTitle_prefix_matches()
{
    QVERIFY(filterRules::matchesTitle(QStringLiteral("算法作业"), QStringLiteral("算法")));
}

void TestFilterRules::matchesTitle_suffix_matches()
{
    QVERIFY(filterRules::matchesTitle(QStringLiteral("下周算法"), QStringLiteral("算法")));
}

void TestFilterRules::matchesTitle_notContained_doesNotMatch()
{
    QVERIFY(!filterRules::matchesTitle(QStringLiteral("算法作业"), QStringLiteral("英语")));
}

void TestFilterRules::matchesTitle_isCaseSensitive_currentBehavior()
{
    QVERIFY(!filterRules::matchesTitle(QStringLiteral("Apple"), QStringLiteral("apple")));
    QVERIFY(filterRules::matchesTitle(QStringLiteral("Apple"), QStringLiteral("Apple")));
}

void TestFilterRules::matchesTitle_chineseSubstring_matches()
{
    QVERIFY(filterRules::matchesTitle(QStringLiteral("今天要交周报"), QStringLiteral("周报")));
}

void TestFilterRules::matchesTitle_emptyTitle_withKeyword_doesNotMatch()
{
    QVERIFY(!filterRules::matchesTitle(QString(), QStringLiteral("算法")));
}

void TestFilterRules::matchesTitle_emptyTitle_emptyKeyword_matches()
{
    QVERIFY(filterRules::matchesTitle(QString(), QString()));
}

void TestFilterRules::matchesTitle_wholeTitleExact_matches()
{
    QVERIFY(filterRules::matchesTitle(QStringLiteral("算法作业"), QStringLiteral("算法作业")));
}

// ==================== matchesType ====================

void TestFilterRules::matchesType_defaultFilter_matchesAllTypes()
{
    for (int t = 0; t <= 3; ++t) {
        QVERIFY(filterRules::matchesType(t, 0));
    }
}

void TestFilterRules::matchesType_sameType_matches()
{
    for (int t = 1; t <= 3; ++t) {
        QVERIFY(filterRules::matchesType(t, t));
    }
}

void TestFilterRules::matchesType_differentType_doesNotMatch()
{
    QVERIFY(!filterRules::matchesType(1, 2));
    QVERIFY(!filterRules::matchesType(2, 3));
    QVERIFY(!filterRules::matchesType(3, 1));
}

void TestFilterRules::matchesType_negativeKardType_doesNotMatch()
{
    QVERIFY(!filterRules::matchesType(-1, 1));
}

void TestFilterRules::matchesType_outOfRangeFilter_doesNotMatch()
{
    QVERIFY(!filterRules::matchesType(1, 99));
}

QTEST_GUILESS_MAIN(TestFilterRules)
#include "tst_filterrules.moc"