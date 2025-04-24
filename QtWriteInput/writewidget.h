#ifndef WRITEWIDGET_H
#define WRITEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

class WriteWidget : public QWidget {
    Q_OBJECT

signals:
    void upMouseRelease(const bool &upPath);



public:
    WriteWidget(QWidget* parent = nullptr);
protected:

    //用各种鼠标事件画轨迹以及背景
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent* event) override;

public:
    std::vector<QPoint> m_currentPath;
    std::vector<std::vector<QPoint>> m_paths;
    bool m_overlapFlag;
    size_t m_lineWidth;
    QString m_lineColor;
};



#endif // WRITEWIDGET_H
