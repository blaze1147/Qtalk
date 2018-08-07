#ifndef QMYLISTWIDGET_H
#define QMYLISTWIDGET_H
#include <QListWidget>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QMouseEvent>
class QMyListWidget : public QListWidget
{
    Q_OBJECT
public:
    QMyListWidget(QWidget *parent);
    ~QMyListWidget();
    QMenu *m_contextMenu;
    QAction *m_msgAction;
    QAction *m_delAction;
protected:
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // QMYLISTWIDGET_H
