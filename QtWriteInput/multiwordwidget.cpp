#include "multiwordwidget.h"

MultiWordWidget::MultiWordWidget(QWidget* parent) : QWidget(parent), m_currentCandidateIsAssociate(true),
m_candidateFontSize(12) {

    m_items = { u8"你", u8"我", u8"他", u8"是", u8"了", u8"就", u8"在",u8"的" };//默认显示
    setFixedSize(680, 480);
    setMouseTracking(true); // 启用鼠标追踪，即使不按下鼠标也能检测到移动事件

}

// 更新候选字的方法
void MultiWordWidget::updateCandidates(const bool& associate, const QStringList& candidates) {
    if (candidates.isEmpty()) {
        qDebug() << "Candidates is empty!";
        return;
    }
    m_currentCandidateIsAssociate = associate;
    m_items.clear();
    int singleWord = 0;//单字数量
    int multiWord = 0;//多字数量
    for (const QString& candidate : candidates) {
        if (candidate.size() <= 1 && singleWord < 9) {
            m_items.append(candidate);
            singleWord++;
        }
    }
    while (singleWord < 9) {//多字数量限制
        m_items.append("");
        singleWord++;
    }
    for (const QString& candidate : candidates) {
        if (candidate.size() > 1  && multiWord < 4) {
            m_items.append(candidate);
            multiWord++;
        }
    }
    while (multiWord < 4) {//多字数量限制
        m_items.append("");
        multiWord++;
    }
    update(0, 0, 680, 55); // 指定候选字区域触发重绘
}


//绘制候选字区域
void MultiWordWidget::paintEvent(QPaintEvent*){
    QPainter painter(this);
    // 设置字体为微软雅黑
    QFont candidateFont("Microsoft YaHei", m_candidateFontSize);
    painter.setFont(candidateFont);
    painter.setRenderHint(QPainter::Antialiasing);

    int initPositionX = 0;//起始位置
    int initPositionY = 0;
    int cellWidth = 60;//单字宽度
    int cellHeight = 55;

    m_candidateBlocks.clear();
    m_candidateContents.clear();

    // 绘制识别候选字 平铺
    for (int index = 0; index < m_items.size(); ++index) {
        QString text = m_items[index];
        if (text.size() == 0 || initPositionX + cellWidth * 0.5 * (text.size() + 1) >= 680)continue;
        QRect rect(initPositionX, initPositionY, cellWidth * 0.5 * (text.size() + 1), cellHeight);
        drawBlock(painter, rect, text, Qt::gray);
        m_candidateBlocks.push_back(rect); // 记录候选字方块的位置
        m_candidateContents.push_back(text);// 记录候选字方块的内容
        initPositionX += cellWidth * 0.5 * (text.size() + 1);
    }


    initPositionX = 450;
    initPositionY = 425;

    // 设置选项字体为微软雅黑
    QFont font("Microsoft YaHei", 12);
    painter.setFont(font);

    // 绘制选项 (1x4)
    QStringList itemsOption = { u8"，。", u8"123", u8"ABC", u8"中",u8"退一笔",u8"重写" };
    cellWidth = 45;
    cellHeight = 35;
    for (int col = 0; col < 4; ++col) {
        QRect rect(initPositionX + col * cellWidth, initPositionY, cellWidth, cellHeight);
        drawBlock(painter, rect, itemsOption[col], Qt::gray);
        m_optionBlocks.push_back(rect);
        m_optionContents.push_back(itemsOption[col]);
    }
    for (int col = 0; col < 2; ++col) {
        QRect rect(col * cellWidth * 3, initPositionY, cellWidth * 3, cellHeight);
        drawBlock(painter, rect, itemsOption[4 + col], Qt::gray);
        m_optionBlocks.push_back(rect);
        m_optionContents.push_back(itemsOption[4 + col]);
    }
}

//鼠标悬停特效
void MultiWordWidget::mouseMoveEvent(QMouseEvent* event){
    QPoint mousePos = event->pos();

    // 遍历所有候选字方块，检查鼠标是否悬停
    bool wordFound = false;
    for (int i = 0; i < m_candidateBlocks.size(); ++i) {
        if (m_candidateBlocks[i].contains(mousePos)) {
            if (m_candidateContents[i] == "")break;// 悬停方块对应的文字为空则不渲染
            m_highlightedBlock = m_candidateBlocks[i]; // 记录当前悬停的方块
            wordFound = true;
            break;
        }
    }
    if (!wordFound && !m_candidateBlocks.empty()) {
        m_highlightedBlock = m_candidateBlocks[0]; // 如果鼠标没有悬停在任何方块上，高亮第一个
    }
    update(0, 0, 680, 55); // 指定候选字区域触发重绘

    // 遍历所有选项方块，检查鼠标是否悬停
    bool optionFound = false;
    for (int i = 0; i < m_optionBlocks.size(); ++i) {
        if (m_optionBlocks[i].contains(mousePos)) {
            m_optionHighlightedBlock = m_optionBlocks[i]; // 记录当前悬停的方块
            optionFound = true;
            break;
        }
    }
    if (!optionFound) {
        m_optionHighlightedBlock = QRect(); // 如果鼠标没有悬停在任何方块上清空
    }
    update(0, 425, 680, 35); // 指定选项区域重绘
}

//鼠标点击反馈
void MultiWordWidget::mousePressEvent(QMouseEvent* event){
    QPoint mousePos = event->pos();

    // 遍历所有方块，检查鼠标是否点击
    for (int i = 0; i < m_candidateBlocks.size(); ++i) {
        if (m_candidateBlocks[i].contains(mousePos)) {
            if (m_candidateContents[i].isEmpty()) return;

            // 默认行为（可根据需要调整）
            qDebug() << "Clicked on:" << m_candidateContents[i];
            candidateSelected(m_currentCandidateIsAssociate, m_candidateContents[i]);
            return;
        }
    }

    for (int i = 0; i < m_optionBlocks.size(); ++i) {
        if (m_optionBlocks[i].contains(mousePos)) {
            if (m_optionContents[i].isEmpty()) return;
            // 处理特殊键反馈
            handleSpecialKeys(m_optionContents[i]);
            return;
        }
    }

}

// 处理特殊键反馈
void MultiWordWidget::handleSpecialKeys(const QString& text) {
    static const QHash<QString, QString> specialKeyActions = {
        { u8"，。", "symbol" },
        { u8"123", "number" },
        { u8"ABC", "letter" },
        { u8"中", "noKeyboard" }
    };

    if (specialKeyActions.contains(text)) {
        emit keyboardSelected(specialKeyActions[text]);
    }
    else if (text == u8"退一笔") {
        emit changeInkSelected("goBack");
    }
    else if (text == u8"重写") {
        emit changeInkSelected("rewrite");
    }
    /*else {调试时使用
        qDebug() << "Unrecognized special key:" << text;
    }*/
}

//画候选字与按钮方块特效
void MultiWordWidget::drawBlock(QPainter& painter, const QRect& rect, const QString& text,
    Qt::GlobalColor borderColor) {
    if (rect == m_highlightedBlock) {
        // 如果是高亮方块，设置红色边框和淡粉红背景
        painter.fillRect(rect, QColor(255, 182, 193)); // 背景颜色：淡粉红
        painter.setPen(Qt::red); // 边框颜色：红色
        //sendKeystrokes(text);

    }
    else {
        // 普通方块
        painter.fillRect(rect, Qt::white); // 背景颜色：白色
        painter.setPen(borderColor);      // 边框颜色：灰色
        if (rect == m_optionHighlightedBlock)painter.setPen(Qt::red); // 边框颜色：红色
    }

    painter.drawRect(rect); // 绘制边框

    // 绘制文字
    painter.setPen(Qt::black);
    painter.drawText(rect, Qt::AlignCenter, text);
}
