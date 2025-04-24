#ifndef LINECOMBOBOX_H
#define LINECOMBOBOX_H

#include <QApplication>
#include <QComboBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>


// 自定义委托类，用于绘制选项的图形（包括下拉框和当前显示区域）==================================
class LineDelegate : public QStyledItemDelegate {
public:
    explicit LineDelegate(QColor* lineColor, QComboBox* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
private:
    QColor* m_lineColor;
};

// 自定义 QComboBox 的派生类（用于图形下拉框）===================================================
class LineComboBox : public QComboBox {
public:
    explicit LineComboBox(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
public:
    QColor m_customColor; // 定义颜色
};


//自定义可交互Label==============================================================================
class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = nullptr);
    void setShecked();
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void enterEvent(QEvent* event) override;

    void leaveEvent(QEvent* event) override;

public:
    bool m_hightFlag;
private:
    // 更新样式表，保留原有的背景等信息
    void updateStyle(const QString& newBorderStyle);

};



#endif//LINECOMBOBOX_H
