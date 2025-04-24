#ifndef SMALLKEYBOARDWIDGET_H
#define SMALLKEYBOARDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
class SmallKeyboardWidget : public QWidget {
    Q_OBJECT

signals:
    void candidateSelected(const QString& candidate);
public:
    SmallKeyboardWidget(QStringList keyItems, QWidget* parent = nullptr);
protected:

    //绘制键盘区域
    void paintEvent(QPaintEvent*) override;

    //鼠标悬停特效
    void mouseMoveEvent(QMouseEvent* event) override;

    //鼠标点击反馈
    void mousePressEvent(QMouseEvent* event) override;

    //画候选字与按钮方块特效
    void drawBlock(QPainter& painter, const QRect& rect, const QString& text,
        Qt::GlobalColor borderColor);


private:
    QVector<QRect> m_candidateBlocks;       // 保存所有候选字方块的矩形区域
    QVector<QString> m_items;
    QRect m_highlightedBlock;      // 当前鼠标悬停的方块

};

#endif//SMALLKEYBOARDWIDGET_H
