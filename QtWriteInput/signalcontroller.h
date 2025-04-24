#ifndef SIGNALCONTROLLER_H
#define SIGNALCONTROLLER_H

#include <QObject>
#include <QStackedWidget>
#include "ui_mainwindow.h"
#include "singlewordwidget.h"
#include "multiwordwidget.h"
#include "smallkeyboardwidget.h"
#include "settingwidget.h"


class SignalController : public QObject {
    Q_OBJECT

public:
    SignalController(QObject* parent = nullptr);
    void connectControll(
        Ui::MainWindow* ui, SingleWordWidget* singleWordPage, MultiWordWidget* multiWordPage,
        WriteWidget* writeWidget, WriteManager* writeManager, QStackedWidget* smallKeyboardStackedWidget,
        SmallKeyboardWidget* symbolKeyboardWidget, SmallKeyboardWidget* numberKeyboardWidget,
        SmallKeyboardWidget* letterKeyboardWidget, SettingWidget* settingWidget, QPushButton *btnSettings

    );
private:
    // 模拟键盘输入
    void sendKeystrokes(const QString& text);

};



#endif // SIGNALCONTROLLER_H
