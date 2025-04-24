#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "singlewordwidget.h"
#include "multiwordwidget.h"
#include "smallkeyboardwidget.h"
#include "settingwidget.h"
#include "signalcontroller.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QListWidget>
#include <QSizePolicy>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
        
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //浮动在上层
    void showAt(const QPoint& pos);
private:

    //初始化当前界面内连接信号
    void currentWidgetConnect();

    // 实现拖动窗口功能
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    //各个部件与ui
    Ui::MainWindow * m_ui;
    SingleWordWidget* m_singleWordPage;//单字输入窗口
    MultiWordWidget* m_multiWordPage;//多字输入窗口
    WriteWidget* m_writeWidget; //手写输入窗口
    WriteManager* m_writeManager; //手写数据处理中心
    QStackedWidget* m_smallKeyboardStackedWidget;//小键盘窗口容器
    SmallKeyboardWidget* m_symbolKeyboardWidget;//符号小键盘输入窗口
    SmallKeyboardWidget* m_numberKeyboardWidget;//数字小键盘输入窗口
    SmallKeyboardWidget* m_letterKeyboardWidget; //字母小键盘输入窗口
    SettingWidget* m_settingWidget;
    QPushButton* m_btnClose;//关闭窗口按钮
    QPushButton* m_btnSettings ;//设置窗口
    bool m_drag = false;//拖动标志
    QPoint m_dragPosition;//拖动位置



    
};
#endif // MAINWINDOW_H
