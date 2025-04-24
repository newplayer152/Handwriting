#include "customcontrols.h"

// 自定义委托类，用于绘制选项的图形（包括下拉框和当前显示区域）===================================
LineDelegate::LineDelegate(QColor* lineColor, QComboBox* parent ) : m_lineColor(lineColor), QStyledItemDelegate(parent) {};

void LineDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    painter->save();

    // 获取线的粗细信息（存储在数据模型中）
    int lineWidth = index.data(Qt::UserRole).toInt();

    // 绘制线段
    QRect rect = option.rect;
    QPen pen(*m_lineColor, lineWidth); // 父类同色线段，指定宽度
    painter->setPen(pen);
    painter->drawLine(rect.left(), rect.center().y(), rect.right(), rect.center().y());

    painter->restore();
}

QSize LineDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const{
    return QSize(100, 20); // 固定高度为20
}



// 自定义 QComboBox 的派生类（用于图形下拉框）===================================================
LineComboBox::LineComboBox(QWidget* parent) : QComboBox(parent), m_customColor(Qt::black) {
    setItemDelegate(new LineDelegate(&m_customColor, this)); // 设置自定义委托
}

void LineComboBox::paintEvent(QPaintEvent* event){
    QPainter painter(this);

    // 获取当前选中的索引
    int currentIndex = this->currentIndex();
    QRect rect = this->rect();

    if (currentIndex >= 0) {
        // 获取当前项的线宽信息
        int lineWidth = itemData(currentIndex, Qt::UserRole).toInt();

        // 绘制线段
        QRect lineRect = rect.adjusted(5, 0, -25, 0); // 为箭头预留空间
        QPen pen(m_customColor, lineWidth);
        painter.setPen(pen);
        painter.drawLine(lineRect.left(), lineRect.center().y(), lineRect.right(), lineRect.center().y());
    }

    // 绘制箭头
    QRect arrowRect = rect.adjusted(rect.width() - 20, 0, -5, 0); // 箭头区域
    QStyleOption arrowOption;
    arrowOption.rect = arrowRect;
    arrowOption.state = QStyle::State_Enabled;
    style()->drawPrimitive(QStyle::PE_IndicatorArrowDown, &arrowOption, &painter, this);
}


//自定义可交互Label类==============================================================================
ClickableLabel::ClickableLabel(QWidget* parent) : QLabel(parent), m_hightFlag(false) {
    setAttribute(Qt::WA_Hover, true);
    // 初始设置仅设置边框，背景保持外部设置
    setStyleSheet("border: 2px solid transparent; padding: 10px;");
    if (m_hightFlag)updateStyle("border: 2px solid black;");
}

//设置选中样式
void ClickableLabel::setShecked(){
    if (m_hightFlag)updateStyle("border: 2px solid black;");
    else updateStyle("border: 2px solid transparent;");
}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        updateStyle("border: 2px solid black;"); // 鼠标按下时设置黑色边框
        emit clicked();
    }
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent* event){
    if (!m_hightFlag)updateStyle("border: 2px solid transparent;"); // 鼠标释放时恢复透明边框
}

void ClickableLabel::enterEvent(QEvent* event) {
    updateStyle("border: 2px solid black;"); // 鼠标悬停时设置黑色边框
}

void ClickableLabel::leaveEvent(QEvent* event){
    if (!m_hightFlag)updateStyle("border: 2px solid transparent;"); // 鼠标离开时恢复透明边框
}

// 更新样式表，保留原有的背景等信息
void ClickableLabel::updateStyle(const QString& newBorderStyle) {
    QString currentStyle = styleSheet();

    // 使用正则表达式提取背景相关信息
    QRegularExpression bgRegex("(background-color:[^;]*;)");
    QRegularExpressionMatch match = bgRegex.match(currentStyle);
    QString backgroundStyle = match.hasMatch() ? match.captured(1) : "";

    // 合成新的样式表
    QString newStyle = QString("%1 %2").arg(backgroundStyle, newBorderStyle);
    setStyleSheet(newStyle);
}