#include "smallkeyboardwidget.h"



SmallKeyboardWidget::SmallKeyboardWidget(QStringList keyItems, QWidget* parent ) : QWidget(parent) {
    setFixedSize(370, 220);
    for (const auto& item : keyItems) {
        m_items.push_back(item);
    }
    setMouseTracking(true); // 启用鼠标追踪，即使不按下鼠标也能检测到移动事件
};

//绘制键盘区域
void SmallKeyboardWidget::paintEvent(QPaintEvent*){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 设置字体为微软雅黑
    QFont font("Microsoft YaHei", 12);
    painter.setFont(font);

    int initPositionX = 0;
    int initPositionY = 0;



    // 绘制识别候选字 (6x9)
    int index = 0;
    int rows = 6, cols = 9;
    int cellWidth = 40;
    int cellHeight = 35;
    m_candidateBlocks.clear();
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            QRect rect(initPositionX + col * cellWidth, initPositionY + row * cellHeight, cellWidth, cellHeight);
            drawBlock(painter, rect, index < m_items.size() ? m_items[index++] : "", Qt::gray);
            m_candidateBlocks.push_back(rect); // 记录每个方块的位置
        }
    }
}

//鼠标悬停特效
void SmallKeyboardWidget::mouseMoveEvent(QMouseEvent* event){
    QPoint mousePos = event->pos();

    // 遍历所有候选字方块，检查鼠标是否悬停
    bool wordFound = false;
    for (int i = 0; i < m_candidateBlocks.size(); ++i) {
        if (m_candidateBlocks[i].contains(mousePos)) {
            if (m_items[i] == "")break;// 悬停方块对应的文字为空则不渲染
            m_highlightedBlock = m_candidateBlocks[i]; // 记录当前悬停的方块
            wordFound = true;
            break;
        }
    }
    if (!wordFound) {
        m_highlightedBlock = QRect(); // 如果鼠标没有悬停在任何方块上，不亮
    }
    update(); // 候选字区域触发重绘

}

//鼠标点击反馈
void SmallKeyboardWidget::mousePressEvent(QMouseEvent* event) {
    QPoint mousePos = event->pos();

    // 遍历所有方块，检查鼠标是否点击
    for (int i = 0; i < m_candidateBlocks.size(); ++i) {
        if (m_candidateBlocks[i].contains(mousePos)) {
            if (m_items[i].isEmpty()) return;

            // 默认行为（可根据需要调整）
            qDebug() << "Clicked on:" << m_items[i];
            candidateSelected(m_items[i]);
            return;
        }
    }
}

//画候选字与按钮方块特效
void SmallKeyboardWidget::drawBlock(QPainter& painter, const QRect& rect, const QString& text,
    Qt::GlobalColor borderColor) {
    if (rect == m_highlightedBlock) {
        // 如果是高亮方块，设置红色边框和淡粉红背景
        painter.fillRect(rect, QColor(255, 182, 193)); // 背景颜色：淡粉红
        painter.setPen(Qt::red); // 边框颜色：红色


    }
    else {
        // 普通方块
        painter.fillRect(rect, Qt::white); // 背景颜色：白色
        painter.setPen(borderColor);      // 边框颜色：灰色

    }

    painter.drawRect(rect); // 绘制边框

    // 绘制文字
    painter.setPen(Qt::black);
    painter.drawText(rect, Qt::AlignCenter, text);
}
