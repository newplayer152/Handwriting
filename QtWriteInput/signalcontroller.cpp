#include "signalcontroller.h"

SignalController::SignalController(QObject* parent) : QObject(parent) {
};
void SignalController::connectControll(
    Ui::MainWindow* ui, SingleWordWidget* singleWordPage, MultiWordWidget* multiWordPage,
    WriteWidget* writeWidget, WriteManager* writeManager, QStackedWidget* smallKeyboardStackedWidget,
    SmallKeyboardWidget* symbolKeyboardWidget, SmallKeyboardWidget* numberKeyboardWidget,
    SmallKeyboardWidget* letterKeyboardWidget, SettingWidget* settingWidget, QPushButton* btnSettings

) {
    // 连接候选字结果 recognitionFinished 信号到结果显示槽函数
    connect(writeManager, &WriteManager::recognitionFinished, [=](const bool& associate, const QStringList& result) {
        singleWordPage->updateCandidates(associate, result);
        multiWordPage->updateCandidates(associate, result);
        });

    //singleWordPage (单字界面)连接选中字到输入框
    connect(singleWordPage, &SingleWordWidget::candidateSelected, [=](const bool& candidateFlag, const QString& candidate) {
        sendKeystrokes(candidate);
        writeManager->associateAsync(candidate);//加入联想功能
        if (!candidateFlag && candidate.size() >= 2)writeManager->updateWordFrequency(candidate);//更新联想词库
        writeManager->clear();
        });

    //multiWordPage (多字界面)连接选中字到待输入窗口
    connect(multiWordPage, &MultiWordWidget::candidateSelected, [=](const bool& candidateFlag, const QString& candidate) {
        sendKeystrokes(candidate);
        writeManager->associateAsync(candidate);//加入联想功能
        if (!candidateFlag && candidate.size() >= 2)writeManager->updateWordFrequency(candidate);//更新联想词库
        writeManager->clear();
        });

    // 映射键盘类型到相应的 QWidget 简化代码
    static const QMap<QString, SmallKeyboardWidget*> keyboardMap = {
        {"symbol", symbolKeyboardWidget},
        {"number", numberKeyboardWidget},
        {"letter", letterKeyboardWidget}
    };

    //SmallKeyboardWidget （小键盘）连接选中字到待输入窗口
    for (const auto& key : keyboardMap.keys()) {
        connect(keyboardMap.value(key), &SmallKeyboardWidget::candidateSelected,[=](const QString& candidate) {
            sendKeystrokes(candidate); });
    }

    //对单字输入界面中退一笔与重写按钮连接墨迹信息
    connect(singleWordPage, &SingleWordWidget::changeInkSelected,  [=](const QString& changeInkType) {
        if (changeInkType == "goBack") {
            writeManager->backSpacing();
        }
        else if (changeInkType == "rewrite") {
            writeManager->clear();
        }

        });

    //对多字输入界面中退一笔与重写按钮连接墨迹信息
    connect(multiWordPage, &MultiWordWidget::changeInkSelected, [=](const QString& changeInkType) {
        if (changeInkType == "goBack") {
            writeManager->backSpacing();
        }
        else if (changeInkType == "rewrite") {
            writeManager->clear();
        }

        });


    //连接设置按钮到更新样式内容信息
    connect(btnSettings, &QPushButton::clicked, [=]() {//从当前各个类中获取最新状态并显示
        settingWidget->m_currentPenColor = writeWidget->m_lineColor;
        settingWidget->m_currentPenSize = writeWidget->m_lineWidth;
        settingWidget->m_currentCandidateSize = singleWordPage->m_candidateFontSize;
        settingWidget->m_currentOverlopFlag = writeWidget->m_overlapFlag;
        settingWidget->m_currentOverlopTime = writeManager->m_minOverlopTime;
        });

    // 连接设置信号transferAllStyle到更改对应样式槽函数
    connect(settingWidget, &SettingWidget::transferAllStyle, [=]
    (const QString& currentPenColor,
        const size_t& currentPenSize,
        const size_t& currentCandidateSize,
        const bool& currentOverlopFlag,
        const size_t& currentOverlopTime) {

            writeWidget->m_lineColor = currentPenColor;
            writeWidget->m_lineWidth = currentPenSize;
            singleWordPage->m_candidateFontSize = currentCandidateSize;
            multiWordPage->m_candidateFontSize = currentCandidateSize;
            writeWidget->m_overlapFlag = currentOverlopFlag;
            writeManager->m_minOverlopTime = currentOverlopTime;

        });

}

// 模拟键盘输入
void SignalController::sendKeystrokes(const QString& text) {
    if (text.isEmpty())return;
    for (const QChar& ch : text) {
        INPUT input = { 0 };
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = 0; // 虚拟键码
        input.ki.wScan = ch.unicode(); // 输入 Unicode 字符
        input.ki.dwFlags = KEYEVENTF_UNICODE; // Unicode 键
        SendInput(1, &input, sizeof(INPUT));

        // 释放按键
        input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        QThread::msleep(10); // 延迟避免丢失字符
    }
}