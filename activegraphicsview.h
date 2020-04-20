#ifndef ACTIVEGRAPHICSVIEW_H
#define ACTIVEGRAPHICSVIEW_H

#include <QGraphicsView>

class ActiveGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    ActiveGraphicsView(QWidget * parent = 0);
    virtual ~ActiveGraphicsView(){}
signals:
    void stateChanged();

public slots:
    void setMashtab(int newMashtab);

protected:
    int mashtab;
    void wheelEvent(QWheelEvent * event);
};

#endif // ACTIVEGRAPHICSVIEW_H
