#pragma once
#include<qstring.h>

// 卡片筛选规则。
// 把「筛选条件」从「遍历 widget 列表」里拆了出来。
namespace filterRules {

// 标题是否命中所选关键词。
// 与旧代码 kard->Title().contains(title) 语义一致：
// 空关键词命中所有标题，大小写敏感。
bool matchesTitle(const QString& kardTitle, const QString& keyword);

// 卡片类型是否命中所选筛选类型。
// filterType 为 0（Default）表示不过滤，命中一切。
bool matchesType(int kardType, int filterType);

}