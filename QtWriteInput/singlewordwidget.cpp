#include "singlewordwidget.h"

SingleWordWidget::SingleWordWidget(QWidget* parent) : QWidget(parent), m_currentCandidateIsAssociate(true), m_candidateFontSize(12) {
    m_items = { u8"你", u8"我", u8"他", u8"是", u8"了", u8"就", u8"在",
        u8"的", u8"都", u8"什么", u8"呵呵", u8"我们", u8"好的" };//默认显示
    setFixedSize(680, 480);
    
    setMouseTracking(true); // 启用鼠标追踪，即使不按下鼠标也能检测到移动事件
}

// 更新候选字的方法
void SingleWordWidget::updateCandidates(const bool& associate, const QStringList& candidates) {
    if (candidates.isEmpty()) {
        qDebug() << "Candidates is empty!";
        return;
    }
    m_currentCandidateIsAssociate = associate;
    m_items.clear();
    int singleWord = 0;//单字数量
    int multiWord = 0;//多字数量
    for (const QString& candidate : candidates) {
        if (candidate.size() <= 1&& singleWord < 9) {
            m_items.append(candidate);
            singleWord++;
        }
    }
    while (singleWord < 9) {//多字数量限制
        m_items.append("");
        singleWord++;
    }
    for (const QString& candidate : candidates) {
        if (candidate.size() > 1&& candidate.size() < 4 && multiWord < 4) {
            m_items.append(candidate);
            multiWord++;
        }
    }
    while (multiWord < 4) {//多字数量限制
        m_items.append("");
        multiWord++;
    }
    update(450, 0, 180, 425); // 指定候选字区域触发重绘
}

//绘制候选字区域
void SingleWordWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    int initPositionX = 450;
    int initPositionY = 0;

    m_candidateBlocks.clear();
    m_candidateContents.clear();

    // 设置显示字体为微软雅黑40字号
    QFont showFont("Microsoft YaHei", 40);
    painter.setFont(showFont);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制显示选中字
    QRect rectShowLabel(initPositionX, initPositionY, 180, 150);
    drawBlock(painter, rectShowLabel, m_highlightedText, Qt::gray); // 高亮区域显示悬停文字
    initPositionY += 150;

    // 设置候选字体为微软雅黑与字号
    QFont candidateFont("Microsoft YaHei", m_candidateFontSize);
    painter.setFont(candidateFont);

    // 绘制识别候选字 (3x3)
    int index = 0;
    int rows = 3, cols = 3;
    int cellWidth = 60;
    int cellHeight = 55;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            QRect rect(initPositionX + col * cellWidth, initPositionY + row * cellHeight, cellWidth, cellHeight);
            QString text = index < m_items.size() ? m_items[index++] : "";
            drawBlock(painter, rect, text, Qt::gray);
            m_candidateBlocks.push_back(rect); // 记录候选字方块的位置
            m_candidateContents.push_back(text);// 记录候选字方块的内容
        }
    }
    initPositionY += cellHeight * 3;

    // 绘制识别候选字 (2x2)
    rows = 2;
    cols = 2;
    cellWidth = 90;
    cellHeight = 55;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            QRect rect(initPositionX + col * cellWidth, initPositionY + row * cellHeight, cellWidth, cellHeight);
            QString text = index < m_items.size() ? m_items[index++] : "";
            drawBlock(painter, rect, text, Qt::gray);
            m_candidateBlocks.push_back(rect); // 记录候选字方块的位置
            m_candidateContents.push_back(text);// 记录候选字方块的内容
        }
    }
    initPositionY += cellHeight * 2;

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
void SingleWordWidget::mouseMoveEvent(QMouseEvent* event) {
    QPoint mousePos = event->pos();

    // 遍历所有候选字方块，检查鼠标是否悬停
    bool wordFound = false;
    for (int i = 0; i < m_candidateBlocks.size(); ++i) {
        if (m_candidateBlocks[i].contains(mousePos)) {
            if (m_candidateContents[i] == "")break;// 悬停方块对应的文字为空则不渲染
            m_highlightedBlock = m_candidateBlocks[i]; // 记录当前悬停的方块
            m_highlightedText = m_candidateContents[i];
            wordFound = true;
            break;
        }
    }
    if (!wordFound) {
        m_highlightedBlock = m_candidateBlocks[0]; // 如果鼠标没有悬停在任何方块上，高亮第一个
        m_highlightedText = m_candidateContents[0];
    }

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
    update(450, 0, 180, 425); // 指定候选字区域触发重绘
    update(0, 425, 680, 35); // 指定选项区域重绘
}

//鼠标点击反馈
void SingleWordWidget::mousePressEvent(QMouseEvent* event) {
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
void SingleWordWidget::handleSpecialKeys(const QString& text) {
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
    //else {调试时使用
    //    qDebug() << "Unrecognized special key:" << text;
    //}
}

//画候选字与按钮方块特效
void SingleWordWidget::drawBlock(QPainter& painter, const QRect& rect, const QString& text,
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
        if (rect == m_optionHighlightedBlock)painter.setPen(Qt::red); // 边框颜色：红色
    }

    painter.drawRect(rect); // 绘制边框

    // 绘制文字
    painter.setPen(Qt::black);
    painter.drawText(rect, Qt::AlignCenter, text);
}