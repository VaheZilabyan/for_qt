#include "mainwindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTime>
#include <QToolButton>
#include <QStyleFactory>
#include <QApplication>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_tree = new QTreeWidget(this);

    HeaderWithButton *header = new HeaderWithButton("My Title", Qt::Horizontal, m_tree);
    m_tree->setHeader(header);

    m_tree->setColumnCount(2);
    m_tree->setHeaderLabels({"Name", "Clock"});
    m_tree->header()->setSectionResizeMode(QHeaderView::Stretch);
    m_tree->setStyleSheet(R"(
        QTreeWidget::item {
          border-right: 1px solid #ccc;
        }
    )");

    qDebug() << "SmallIcon:"
             << qApp->style()->pixelMetric(QStyle::PM_SmallIconSize);

    qDebug() << "devicePixelRatio:"
             << qApp->primaryScreen()->devicePixelRatio();

    qDebug() << "logical DPI:"
             << qApp->primaryScreen()->logicalDotsPerInch();

    m_tree->setIndentation(20);
    m_tree->setRootIsDecorated(true);

    setCentralWidget(m_tree);

    m_timer = new QTimer(this);
    m_timer->start(1000);

    // создаём 3 строки
    for (int i = 0; i < 3; ++i)
        createRow();
}

void MainWindow::createRow(QTreeWidgetItem* parent)
{
    QTreeWidgetItem* item;

    if (parent)
        item = new QTreeWidgetItem(parent);
    else
        item = new QTreeWidgetItem(m_tree);


    // ===== Column 0 (Name + Button) =====
    QWidget* container = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(container);
    layout->setContentsMargins(0,0,0,0);

    QLineEdit* nameEdit = new QLineEdit("Probe");
    nameEdit->setFrame(false);
    nameEdit->setStyleSheet(
        "QLineEdit {"
        "   border: 1px solid gray;"
        "   padding: 0px;"
        "}"
        );
    nameEdit->setTextMargins(0, 0, 0, 0);

    QPushButton* addBtn = new QPushButton("+");
    addBtn->setFixedWidth(25);

    layout->addWidget(nameEdit);
    layout->addWidget(addBtn);

    m_tree->setItemWidget(item, 0, container);

    // ===== Column 1 (Real Time Clock) =====
    QLineEdit* clockEdit = new QLineEdit;
    clockEdit->setReadOnly(true);
    m_tree->setItemWidget(item, 1, clockEdit);

    // обновление времени
    connect(m_timer, &QTimer::timeout, this, [clockEdit]()
            {
                clockEdit->setText(QTime::currentTime().toString("HH:mm:ss"));
            });

    // добавление child
    connect(addBtn, &QPushButton::clicked, this, [this, item]()
            {
                createRow(item);
                item->setExpanded(true);
            });
}
