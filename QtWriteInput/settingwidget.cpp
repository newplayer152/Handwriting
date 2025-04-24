#include "settingwidget.h"

void SettingWidget::onCloseClicked() {
    hide();          // 隐藏窗口
    emit dialogClosed(); // 发出关闭信号，退出事件循环
}

SettingWidget::SettingWidget(QWidget* parent) : QDialog(parent) {
    //初始化当前总数据
    m_penColors = { "#757575", "#FF0000", "#8F5F46", "#CACA00", "#00AA00" };
    m_penSizes = { 3,5,8 };
    m_candidateSizes = { 12,14,16 };
    m_overlopTimes = { 500,800,1000,2000 };

    // 初始化状态值
    m_currentPenColorIndex = 0;
    m_currentPenColor = m_penColors[0];
    m_currentPenSize = m_penSizes[0];
    m_currentCandidateSize = m_candidateSizes[0];
    m_currentOverlopFlag = false;
    m_currentOverlopTime = m_overlopTimes[1];

    setModal(true);  // 设置为模态窗口

    setFixedSize(390, 255);
    setWindowTitle(u8"手写输入法设置");  // 设置窗口标题

    initWindowLayout();//初始化窗口部件

    feedbackStatusLink();//链接部件的信号与槽函数
}

//链接部件的信号与槽函数
void SettingWidget::feedbackStatusLink() {
    // 定义颜色和 ClickableLabel 的连接
    for (int i = 0; i < m_penColors.size(); ++i) {
        connect(m_colorLabels[i], &ClickableLabel::clicked, this, [=]() {
            m_currentPenColorIndex = i;

            m_lineWidthComboBox->m_customColor = QColor(m_penColors[i]);
            m_lineWidthComboBox->update();
            for (auto& colorlabelitem : m_colorLabels) {//清空标志
                colorlabelitem->m_hightFlag = false;
                colorlabelitem->setShecked();
            }
            m_colorLabels[i]->m_hightFlag = true;//添加标志
            m_colorLabels[i]->setShecked();
            qDebug() << "Current pen color set to:" << m_penColors[i];
            });
    }

    //连接确认提交按钮
    connect(m_apply, &QPushButton::clicked, this, [=]() {
        m_currentPenColor = m_penColors[m_currentPenColorIndex]; // 获取当状态值
        m_currentPenSize = m_penSizes[m_lineWidthComboBox->currentIndex()];
        m_currentCandidateSize = m_candidateSizes[m_candidateSizeBox->currentIndex()];
        m_currentOverlopFlag = m_overlopCheckBox->isChecked();
        m_currentOverlopTime = m_overlopTimes[m_timeComboBox->currentIndex()];
        emit transferAllStyle(m_currentPenColor,//发送状态信号
            m_currentPenSize,
            m_currentCandidateSize,
            m_currentOverlopFlag,
            m_currentOverlopTime);

        onCloseClicked();// 连接关闭按钮
        });

    //连接恢复默认按钮
    connect(m_reSetting, &QPushButton::clicked, this, [=]() {
        // 重置状态
        m_currentPenColorIndex = 0;
        m_currentPenColor = m_penColors[0];
        m_currentPenSize = m_penSizes[0];
        m_currentCandidateSize = m_candidateSizes[0];
        m_currentOverlopFlag = false;
        m_currentOverlopTime = m_overlopTimes[1];

        emit transferAllStyle(m_currentPenColor,//发送状态信号
            m_currentPenSize,
            m_currentCandidateSize,
            m_currentOverlopFlag,
            m_currentOverlopTime);

        updataCurrentStatus();//更新显示
        });
}

//更新到最新状态并显示
void SettingWidget::updataCurrentStatus() {

    //更新当前颜色标识
    auto penColorsIt = std::find(m_penColors.begin(), m_penColors.end(), m_currentPenColor);
    if (penColorsIt != m_penColors.end()) {
        int index = std::distance(m_penColors.begin(), penColorsIt); // 计算下标
        m_currentPenColorIndex = index;//更新下标
        m_lineWidthComboBox->m_customColor = QColor(m_penColors[index]);//同步画笔颜色
        m_lineWidthComboBox->update();
        for (auto& colorlabelitem : m_colorLabels) {//清空标志
            colorlabelitem->m_hightFlag = false;
            colorlabelitem->setShecked();
        }
        m_colorLabels[index]->m_hightFlag = true;//添加标志
        m_colorLabels[index]->setShecked();
    }
    /*else {调试时使用
        qDebug() << "Value " << m_currentPenColor << " not found.";
    }*/

    //更新绘画粗细下拉框
    auto penSizesIt = std::find(m_penSizes.begin(), m_penSizes.end(), m_currentPenSize);
    if (penSizesIt != m_penSizes.end()) {
        int index = std::distance(m_penSizes.begin(), penSizesIt); // 计算下标
        m_lineWidthComboBox->setCurrentIndex(index);
    }
    /*else {调试时使用
        qDebug() << "Value " << m_currentPenSize << " not found.";
    }*/

    //更新候选字大小下拉框
    auto candidateSizesIt = std::find(m_candidateSizes.begin(), m_candidateSizes.end(), m_currentCandidateSize);
    if (candidateSizesIt != m_candidateSizes.end()) {
        int index = std::distance(m_candidateSizes.begin(), candidateSizesIt); // 计算下标
        m_candidateSizeBox->setCurrentIndex(index);
    }
   /* else {调试时使用
        qDebug() << "Value " << m_currentPenSize << " not found.";
    }*/

    //更新重叠开关选项
    m_overlopCheckBox->setChecked(m_currentOverlopFlag); // true: 选中; false: 未选中

    // 更新时间间隔下拉框
    auto overlopTimesIt = std::find(m_overlopTimes.begin(), m_overlopTimes.end(), m_currentOverlopTime);
    if (overlopTimesIt != m_overlopTimes.end()) {
        int index = std::distance(m_overlopTimes.begin(), overlopTimesIt); // 计算下标
        m_timeComboBox->setCurrentIndex(index);
    }
    //else {调试时使用
    //    qDebug() << "Value " << m_currentPenSize << " not found.";
    //}

    update();//更新
    showBlockingOverlay();
}

//初始化窗口部件
void SettingWidget::initWindowLayout() {
    //功能部件
    m_candidateSizeBox = new QComboBox(this);
    m_candidateSizeBox->setGeometry(QRect(70, 110, 101, 22));

    m_timeComboBox = new QComboBox(this);
    m_timeComboBox->setGeometry(QRect(180, 170, 95, 22));

    m_overlopCheckBox = new QCheckBox(this);
    m_overlopCheckBox->setGeometry(QRect(20, 170, 55, 19));

    m_apply = new QPushButton(this);
    m_apply->setGeometry(QRect(260, 210, 93, 28));

    m_reSetting = new QPushButton(this);
    m_reSetting->setGeometry(QRect(10, 210, 93, 28));

    m_cololabel = new QLabel(this);
    m_cololabel->setGeometry(QRect(20, 50, 31, 16));

    m_penSetting = new QLabel(this);
    m_penSetting->setGeometry(QRect(20, 20, 65, 16));

    m_candidateSetting = new QLabel(this);
    m_candidateSetting->setGeometry(QRect(20, 80, 95, 16));

    m_wordSize = new QLabel(this);
    m_wordSize->setGeometry(QRect(20, 110, 41, 16));

    m_overlopSetting = new QLabel(this);
    m_overlopSetting->setGeometry(QRect(20, 140, 65, 16));

    m_lineWidth = new QLabel(this);
    m_lineWidth->setGeometry(QRect(230, 50, 31, 16));

    m_overlopTime = new QLabel(this);
    m_overlopTime->setGeometry(QRect(100, 170, 61, 20));

    //装饰部件
    m_showLine_1 = new QFrame(this);
    m_showLine_1->setGeometry(QRect(90, 25, 280, 2));
    m_showLine_1->setStyleSheet(QString::fromUtf8("background-color: rgb(166, 166, 166);"));
    m_showLine_1->setFrameShape(QFrame::StyledPanel);
    m_showLine_1->setFrameShadow(QFrame::Raised);
    m_showLine_2 = new QFrame(this);
    m_showLine_2->setGeometry(QRect(120, 90, 250, 2));
    m_showLine_2->setStyleSheet(QString::fromUtf8("background-color: rgb(166, 166, 166);"));
    m_showLine_2->setFrameShape(QFrame::StyledPanel);
    m_showLine_2->setFrameShadow(QFrame::Raised);
    m_showLine_3 = new QFrame(this);
    m_showLine_3->setGeometry(QRect(90, 150, 280, 2));
    m_showLine_3->setStyleSheet(QString::fromUtf8("background-color: rgb(166, 166, 166);"));
    m_showLine_3->setFrameShape(QFrame::StyledPanel);
    m_showLine_3->setFrameShadow(QFrame::Raised);

    //添加文字
    m_overlopCheckBox->setText(u8"开启");
    m_apply->setText(u8"确定");
    m_reSetting->setText(u8"恢复默认");
    m_cololabel->setText(u8"颜色");
    m_penSetting->setText(u8"笔迹设置");
    m_candidateSetting->setText(u8"候选字号设置");
    m_wordSize->setText(u8"字号");
    m_overlopSetting->setText(u8"单字重叠");
    m_lineWidth->setText(u8"粗细");
    m_overlopTime->setText(u8"识别间隔");
    m_candidateSizeBox->addItem(u8"小(默认)");
    m_candidateSizeBox->addItem(u8"中");
    m_candidateSizeBox->addItem(u8"大");
    m_timeComboBox->addItem(u8"0.5s");
    m_timeComboBox->addItem(u8"0.8s(默认)");
    m_timeComboBox->addItem(u8"1s");
    m_timeComboBox->addItem(u8"2s");

    //设置字体样式
    QFont font;
    font.setFamily(QString::fromUtf8(u8"微软雅黑"));
    m_candidateSizeBox->setFont(font);
    m_timeComboBox->setFont(font);
    m_overlopCheckBox->setFont(font);
    m_cololabel->setFont(font);
    m_wordSize->setFont(font);
    m_lineWidth->setFont(font);
    m_overlopTime->setFont(font);

    QFont font1;
    font1.setFamily(QString::fromUtf8(u8"微软雅黑"));
    font1.setBold(true);
    font1.setWeight(75);
    m_apply->setFont(font1);
    m_penSetting->setFont(font1);
    m_candidateSetting->setFont(font1);
    m_overlopSetting->setFont(font1);
    font1.setUnderline(true);//为恢复默认按钮字体添加下划线
    m_reSetting->setFont(font1);
    m_reSetting->setStyleSheet(QString::fromUtf8("color: blue;background-color: rgba(255, 255, 255, 0);"));



    //绘制颜色label区域
    size_t xOffset = 70; // 起始 X 坐标
    size_t yOffset = 50; // 固定 Y 坐标
    size_t width = 25;   // Label 宽度
    size_t height = 16;  // Label 高度
    size_t spacing = 30; // 每个 Label 的间距
    for (int i = 0; i < m_penColors.size(); ++i) {
        ClickableLabel* colorLabel = new ClickableLabel(this);
        // 设置位置和大小
        colorLabel->setGeometry(QRect(xOffset + i * spacing, yOffset, width, height));
        // 设置背景颜色
        QString style = QString("background-color: %1;").arg(m_penColors[i]);
        colorLabel->setStyleSheet(style);
        // 将 ClickableLabel 添加到 QVector 中
        m_colorLabels.append(colorLabel);
    }
    // 创建自定义下拉框 LineComboBox（用于显示粗细）
    m_lineWidthComboBox = new LineComboBox(this);
    m_lineWidthComboBox->setGeometry(QRect(270, 48, 101, 22));

    // 添加选项
    for (int i = 1; i <= 3; ++i) {
        m_lineWidthComboBox->addItem(QString("Line Width %1").arg(i)); // 添加选项文本（文本不会显示）
        m_lineWidthComboBox->setItemData(i - 1, i * 2 + 1, Qt::UserRole);      // 设置线宽到 UserRole（3，5，7）
    }

}

//显示该窗口
void SettingWidget::showBlockingOverlay() {
    if (!parentWidget()) { qDebug() << "SettingWidget has no parent widget!";return;}

    // 设置窗口大小为父窗口的一半
    resize(parentWidget()->size() / 2);

    // 移动到父窗口的正中间
    QPoint parentCenter = parentWidget()->geometry().center();
    QPoint widgetTopLeft = parentCenter - QPoint(width() / 2, height() / 2);
    move(widgetTopLeft);

    // 显示窗口并进入事件循环，阻塞父窗口
    QEventLoop loop;
    connect(this, &SettingWidget::dialogClosed, &loop, &QEventLoop::quit);
    show();
    loop.exec();  // 阻塞，直到 `quit()` 被调用
}