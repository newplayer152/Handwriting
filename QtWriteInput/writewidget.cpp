#include "writewidget.h"

WriteWidget::WriteWidget(QWidget* parent)
    : QWidget(parent), m_lineWidth(3), m_lineColor("#757575"), m_overlapFlag(false) {
    

}

void WriteWidget::mousePressEvent(QMouseEvent* event){

    if (event->button() == Qt::LeftButton) {
        m_currentPath.clear();
        m_currentPath.push_back(event->pos());
    }
}

void WriteWidget::mouseMoveEvent(QMouseEvent* event){
    if (event->buttons() & Qt::LeftButton) {
        m_currentPath.push_back(event->pos());
        update();
    }
}

void WriteWidget::mouseReleaseEvent(QMouseEvent* event){

    if (event->button() == Qt::LeftButton) {
        m_paths.push_back(m_currentPath);
        update();

        // 发送信号：鼠标离开
        emit upMouseRelease(true);

        m_currentPath.clear();

    }
}
//绘图事件
void WriteWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    //绘制网格背景
    // 获取窗口大小
    int width = this->width();
    int height = this->height();
    // 设置网格大小
    int gridSize = 20;
    // 设置画笔样式
    QPen backgroundPen;
    backgroundPen.setColor(Qt::lightGray); // 网格颜色
    backgroundPen.setWidth(1);            // 网格线宽
    painter.setPen(backgroundPen);
    // 绘制垂直线
    for (int x = gridSize/2; x <= width; x += gridSize) {
        painter.drawLine(x, 0, x, height);
    }
    // 绘制水平线
    for (int y = gridSize/2; y <= height; y += gridSize) {
        painter.drawLine(0, y, width, y);
    }


    //绘制笔迹背景
    // 设置画笔
    QPen pen(QColor(m_lineColor), m_lineWidth); // 红色线条，线宽为5
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw all paths
    for (const auto& path : m_paths) {
        for (size_t i = 1; i < path.size(); ++i) {
            painter.drawLine(path[i - 1], path[i]);
        }
    }

    // Draw current path
    for (size_t i = 1; i < m_currentPath.size(); ++i) {
        painter.drawLine(m_currentPath[i - 1], m_currentPath[i]);
    }
}
