#include"mySortModel.h"

mySortModel::mySortModel(QObject* parent ):QSortFilterProxyModel(parent)
{
}

mySortModel::~mySortModel()
{
}
void mySortModel::setType(fileKard::Type type) {

	this->type = type;
	this->invalidateFilter();
	qDebug() << "sort model updated";
}

bool mySortModel::filterAcceptsRow(int row, const QModelIndex& parent) const
{
    QModelIndex indexName = sourceModel()->index(row, 0, parent);
    QModelIndex indexType = sourceModel()->index(row, 2, parent);

    QString fileName = sourceModel()->data(indexName).toString();
    QString suffix = sourceModel()->data(indexType).toString();

    // ===== 搜索条件 =====
    bool matchSearch = true;

    if (!filterRegularExpression().pattern().isEmpty()) {
        matchSearch = fileName.contains(filterRegularExpression());
    }

    // ===== 类型条件 =====
    bool matchType = true;

    if (type != fileKard::Default) {
        if (type == fileKard::Doc)
            matchType = (suffix == "pdf" || suffix == "txt" || suffix == "docx");
        else if (type == fileKard::Img)
            matchType = (suffix == "png" || suffix == "svg" || suffix == "gif" || suffix == "jpg");
        else if (type == fileKard::Code)
            matchType = (suffix == "cpp" || suffix == "java" || suffix == "py" || suffix == "h");
        else if (type == fileKard::Else)
            matchType = !(suffix == "pdf" || suffix == "txt" || suffix == "docx" ||
                suffix == "png" || suffix == "svg" || suffix == "gif" || suffix == "jpg" ||
                suffix == "cpp" || suffix == "java" || suffix == "py" || suffix == "h");
    }

    return matchSearch && matchType;
}

