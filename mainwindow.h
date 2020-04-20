#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gtbuffer.h"

class MVP_Scene;
class QMenu;
class SignalsManager;
class GtBuffer;
class BaseObject;
class ArchiveManager;
class ArchiveReaderPKS;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

public slots:
    void chanelChanged(GtBuffer *GtBuffer);

private:
    Ui::MainWindow *ui;

protected:
    MVP_Scene *scene;
    QString archiveFileName;
    void closeEvent(QCloseEvent *event);
    bool onGoPrev,onGoNext;
    BaseObject *loadedObject;
    ArchiveManager *archiveManager;
    ArchiveReaderPKS *archiveReaderPKS;
    bool openFile(const QString& fileName);
private slots:
    void on_progress(int procent);
    void buffersChanged(QList<GtBuffer*> l);
    void on_btRegim_clicked();
    void on_positionChaged(int newpos);
    void on_horizontalSlider_valueChanged(int value);
    void on_step();
    void on_btPrev_clicked();
    void on_btNext_clicked();
    void on_btFile_clicked();
};

#endif // MAINWINDOW_H
