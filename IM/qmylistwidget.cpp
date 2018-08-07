#include "qmylistwidget.h"

QMyListWidget::QMyListWidget(QWidget *parent)
: QListWidget(parent)
{
    //创建一个菜单
    m_contextMenu = new QMenu;
    m_msgAction = new QAction("发送消息",this);
    m_delAction = new QAction("删除好友",this);
    m_contextMenu->addAction(m_msgAction);
    m_contextMenu->addAction(m_delAction);
}
QMyListWidget::~QMyListWidget()
{
}
void QMyListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //确保右键点击且鼠标当前位置有列表项然后跳出菜单.
    if (event->button() == Qt::RightButton && this->itemAt(mapFromGlobal(QCursor::pos())) != NULL)
    {
        m_contextMenu->exec(event->globalPos());
    }
}
