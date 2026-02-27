#pragma once

#include <QMainWindow>
#include <QTreeWidget>
#include <QHeaderView>
#include <QToolButton>
#include <QTimer>
#include <QPainter>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void createRow(QTreeWidgetItem* parent = nullptr);

    QTreeWidget* m_tree;
    QTimer* m_timer;
};

class HeaderWithButton : public QHeaderView
{
    Q_OBJECT

public:
    HeaderWithButton(const QString& title,
                     Qt::Orientation orientation,
                     QWidget *parent = nullptr)
        : QHeaderView(orientation, parent),
        m_title(title)
    {
        setSectionsClickable(true);
        setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        m_button = new QToolButton(this);
        m_button->setText("...");
        m_button->setCursor(Qt::PointingHandCursor);
        m_button->setFixedSize(20, 20);
    }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        QHeaderView::resizeEvent(event);

        // Place button on LEFT side of first section
        int x = sectionPosition(0) + 4;
        int y = (height() - m_button->height()) / 2;

        m_button->move(x, y);
    }

    void paintSection(QPainter *painter,
                      const QRect &rect,
                      int logicalIndex) const override
    {
        if (logicalIndex != 0)
        {
            QHeaderView::paintSection(painter, rect, logicalIndex);
            return;
        }

        QStyleOptionHeader opt;
        initStyleOption(&opt);
        opt.rect = rect;
        opt.text = ""; // we paint text manually

        //style()->drawControl(QStyle::CE_Header, &opt, painter, this);

        // Adjust text rect to avoid button overlap
        QRect textRect = rect;
        //textRect.setLeft(rect.left() + m_button->width() + 8);
        textRect.setLeft(33);

        painter->drawText(textRect,
                          Qt::AlignLeft | Qt::AlignVCenter,
                          m_title);
    }

private:
    QToolButton *m_button;
    QString m_title;
};
