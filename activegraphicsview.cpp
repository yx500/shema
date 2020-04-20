#include "activegraphicsview.h"
#include <QWheelEvent>

ActiveGraphicsView::ActiveGraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    mashtab=100;
    setRenderHint(QPainter::Antialiasing, true);
    setDragMode(QGraphicsView::NoDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void ActiveGraphicsView::setMashtab(int newMashtab)
{
    mashtab=newMashtab;
    QMatrix matrix;
    matrix.scale(newMashtab/100., newMashtab/100.);
    setMatrix(matrix);
    emit stateChanged();
}



void ActiveGraphicsView::wheelEvent(QWheelEvent *event)
{
    //QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;
    int newMashtab=mashtab+numDegrees.y();
    setMashtab(newMashtab);
//        if (!numPixels.isNull()) {
//            scrollWithPixels(numPixels);
//        } else if (!numDegrees.isNull()) {
//            QPoint numSteps = numDegrees / 15;
//            scrollWithDegrees(numSteps);
//        }

    event->accept();
}
