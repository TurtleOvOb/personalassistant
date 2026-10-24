#include"filterRules.h"

namespace filterRules {

bool matchesTitle(const QString& kardTitle, const QString& keyword)
{
    if (keyword.isEmpty()) {
        return true;
    }
    return kardTitle.contains(keyword);
}

bool matchesType(int kardType, int filterType)
{
    // Default(0) 表示「全部类型」，不过滤
    if (filterType == 0) {
        return true;
    }
    return kardType == filterType;
}

}