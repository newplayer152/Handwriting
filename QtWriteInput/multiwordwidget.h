#ifndef MULTIWORDWIDGET_H
#define MULTIWORDWIDGET_H

#include "writemanager.h"
#include "writewidget.h"
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QFont>

class MultiWordWidget : public QWidget {

    Q_OBJECT
signals:
    //候选字、小键盘、笔迹相关信号
    void candidateSelected(const bool& associateFlag, const QString& candidate);
    void keyboardSelected(const QString& keyboardType);
    void changeInkSelected(const QString& changeInkType);

public:

    MultiWordWidget(QWidget* parent = nullptr);

    // 更新候选字的方法
    void updateCandidates(const bool& associate, const QStringList& candidates);

protected: 

    //绘制候选字区域
    void paintEvent(QPaintEvent*) override;

    //鼠标悬停特效
    void mouseMoveEvent(QMouseEvent* event) override;

    //鼠标点击反馈
    void mousePressEvent(QMouseEvent* event) override;

    // 处理特殊键反馈
    void handleSpecialKeys(const QString& text);

    //画候选字与按钮方块特效
    void drawBlock(QPainter& painter, const QRect& rect, const QString& text,
        Qt::GlobalColor borderColor);

public:
    size_t m_candidateFontSize;//候选字大小
private:
    //当前候选字是否是联想得到（选中候选字时用于判断是否更新到联想词库，）
    bool m_currentCandidateIsAssociate;   // （若是联想得到的词就不需要进一步更新）
    QVector<QRect> m_candidateBlocks;       // 保存候选字方块的矩形区域
    QVector<QString> m_candidateContents;       // 保存候选字方块的矩形内容
    QVector<QRect> m_optionBlocks;       // 保存选项方块的矩形区域
    QVector<QString> m_optionContents;       // 保存选项方块的矩形内容
    QVector<QString> m_items;
    QRect m_optionHighlightedBlock;      // 当前鼠标悬停的方块
    QRect m_highlightedBlock;      // 当前鼠标悬停的方块



};
#endif // MULTIWORDWIDGET_H