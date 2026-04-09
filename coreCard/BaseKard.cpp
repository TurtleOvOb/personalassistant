#include "BaseKard.h"
BaseKard::BaseKard(QWidget*parent):QFrame(parent)
{
	//设置卡片大小，sizePolicy等几何属性
    //this->setGeometry(QRect(0, 0, 50, 50));
    connect(themeManager::instance(), &themeManager::themeChanged, this, &BaseKard::onThemeChanged);
    this->setMaximumHeight(100);
	this->setMinimumWidth(50);
	this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //onThemeChanged(themeManager::instance()->currentTheme());
    QTimer::singleShot(0, this, [this]() {
        onThemeChanged(themeManager::instance()->currentTheme());
        });

}

BaseKard::~BaseKard()
{
	qDebug() << "one kard deleted";
}

void BaseKard::mouseReleaseEvent(QMouseEvent* event)
{
	qDebug() << "Bsaekard clicked";
	emit clicked();
	QWidget::mouseReleaseEvent(event);
}

void BaseKard::onThemeChanged(themeManager::Theme theme) {

 
    QString qss;
    if (theme == themeManager::Light) {
        qss = R"(/* 浅色模式 BaseKard */
    BaseKard {
    border-radius: 10px;
    /* 使用你之前定义的边框色，比深色模式更细致 */
    border: 1px solid #D1D1D6; 
    background-color: #FFFFFF;
    }

/* 悬停状态：使用浅灰色，与之前的 to_do_List 背景色一致 */
BaseKard:hover {
    background-color: #F2F2F7;
}

/* 选中状态 */
BaseKard[selected="true"] {
    /* 保持原有的绿色调，但在白底上 2px 会非常醒目 */
     border-left: 3px solid #34C759;
    /* 建议：如果想让选中感更强，可以稍微加深一点背景，例如：
    background-color: #F0FFF4; */
})";
    }
    else {
        qss = R"(
           BaseKard{
             border-radius:10px;
             border:1px solid #3A3A3C;
             background-color:#2C2C2E;
            }
           BaseKard:hover{
             background-color:#3A3A3C;
            }
         BaseKard[selected="true"] {
           border-left: 3px solid #34C759;
             }
             )";
    }
    setStyleSheet(qss);
    setIcon(theme);

}