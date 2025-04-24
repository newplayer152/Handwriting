#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include "customcontrols.h"
#include <QVariant>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QFrame>
#include <QMouseEvent>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QEventLoop>
#include <QMouseEvent>
#include <QDialog> 

//设置界面
class SettingWidget : public QDialog {
    Q_OBJECT
signals:
    void dialogClosed();
    // 样式信号
    void transferAllStyle(const QString& currentPenColor,
        const size_t& currentPenSize,
        const size_t& currentCandidateSize,
        const bool& currentOverlopFlag,
        const size_t& currentOverlopTime); 
private slots:
    // 隐藏窗口发出关闭信号，退出事件循环
    void onCloseClicked();
public:
    explicit SettingWidget(QWidget* parent = nullptr);

    //链接部件的信号与槽函数
    void feedbackStatusLink();

    //更新到最新状态并显示
    void updataCurrentStatus();

private:
    //初始化窗口部件
    void initWindowLayout();

    //显示该窗口
    void showBlockingOverlay();

public:
    QWidget* m_centralwidget;
    QComboBox* m_candidateSizeBox;
    QComboBox* m_timeComboBox;
    QCheckBox* m_overlopCheckBox;
    QPushButton* m_apply;
    QPushButton* m_reSetting;
    QFrame* m_showLine_1;
    QLabel* m_cololabel;
    QLabel* m_penSetting;
    QLabel* m_candidateSetting;
    QLabel* m_wordSize;
    QLabel* m_overlopSetting;
    QLabel* m_lineWidth;
    QLabel* m_overlopTime;
    QFrame* m_showLine_2;
    QFrame* m_showLine_3;
    QVector<ClickableLabel*> m_colorLabels;//自定义可交互label
    LineComboBox* m_lineWidthComboBox;//自定义下拉框


    //当前总数据
    std::vector<QString> m_penColors;
    std::vector<size_t> m_penSizes;
    std::vector<size_t> m_candidateSizes;
    std::vector<size_t> m_overlopTimes ;

    //当前状态数据
    size_t m_currentPenColorIndex;
    QString m_currentPenColor;
    size_t m_currentPenSize;
    size_t m_currentCandidateSize;
    bool m_currentOverlopFlag;
    size_t m_currentOverlopTime;

};
#endif //SETTINGWIDGET_H
