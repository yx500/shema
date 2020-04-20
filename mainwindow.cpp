#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDockWidget>
#include <QMessageBox>
#include <QSettings>
#include <QFileInfo>
#include <QKeyEvent>
#include <QVBoxLayout>

#include "mvp_system.h"
#include "mvp_scene.h"

#include "signalsmanager.h"

#include "activegraphicsview.h"
#include "archivemanager.h"
#include "archivereaderpks.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QCoreApplication::instance()->setOrganizationName("GTSS");
    QCoreApplication::instance()->setApplicationName("Shema");
    QSettings settings;

    // подключаем интерфейс передачи буферов

    MVP.setGetGtBufferInterface(&SM);
    // если архивы не нужны можно обойтись только юдп
    //MVP.setGetGtBufferInterface(new GtBuffers_UDP_D2());


    auto *VBoxLayout=new QVBoxLayout();
    // отнаследовал от стандартного -  колесико мыши юзаю
    auto* graphicsView = new ActiveGraphicsView(ui->frame_2);

    VBoxLayout->addWidget(graphicsView);
    ui->frame_2->setLayout(VBoxLayout);

    // создаем спец сцену
    scene=new MVP_Scene(this);

    graphicsView->setScene(scene);

    archiveManager=new ArchiveManager(this);
    archiveReaderPKS=new ArchiveReaderPKS(archiveManager);
    archiveManager->addArchiveReader(archiveReaderPKS);

    connect(archiveManager,SIGNAL(buffersChanged(QList<GtBuffer*>)),this,SLOT(buffersChanged(QList<GtBuffer*>)));
    connect(archiveManager,SIGNAL(positionChaged(int)),this,SLOT(on_positionChaged(int)));
    connect(archiveManager,SIGNAL(progress(int)),this,SLOT(on_progress(int)));



    QString fileName=settings.value("fileName").toString();
    openFile(fileName);

    // надо все время дергать станцию для отображения изменений.
    // можно по таймеру а можно как тут по изм каналов
    connect(&SM,SIGNAL(bufferChanged(GtBuffer*)),this,SLOT(chanelChanged(GtBuffer*)));



    ui->frame_Arhiv->setVisible(false);
    ui->progressBar->setVisible(false);
    onGoPrev=false;onGoNext=false;

    auto *timerGo=new QTimer(this);
    connect(timerGo,SIGNAL(timeout()),this,SLOT(on_step()));
    timerGo->start(5);

    restoreGeometry(settings.value("MainFormGeometry").toByteArray());
    restoreState(settings.value("MainFormWindowState").toByteArray());

    SM.setSignalSource(SignalsManager::ssReal);
}

bool MainWindow::openFile(const QString& fileName)
{
    loadedObject=qobject_cast<BaseObject *>(MVP.loadObject(fileName));

    if (loadedObject!=nullptr){
        QSettings settings;
        settings.setValue("fileName",fileName);
        // выбираем нужный вид - тут тупо первый
        QList<Visual_Screen*>lVisual_Screens=loadedObject->findChildren<Visual_Screen*>();
        if (!lVisual_Screens.isEmpty()){
            scene->setVisual_Screen(lVisual_Screens.first());
            return true;
        }


    }
    return false;
}

void MainWindow::buffersChanged(QList<GtBuffer *> l)
{
    Q_UNUSED(l)
    ui->labelCurrentTime->setText(SM.XB[SignalsManager::ssArchive]->maxTimeDataChanged().toString("hh:mm::ss.zzz"));
    //if (loadedObject) loadedObject->updateStates();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chanelChanged(GtBuffer *)
{
    if (loadedObject) loadedObject->updateStates();
}



void MainWindow::closeEvent(QCloseEvent *event)
{
       QSettings settings;
       settings.setValue("MainFormGeometry", saveGeometry());
       settings.setValue("MainFormWindowState", saveState());
       settings.setValue("Main/archiveFileName",archiveFileName);
       QMainWindow::closeEvent(event);
}


void MainWindow::on_progress(int procent)
{
    if (procent<0){
        ui->progressBar->setVisible(false);
        //adjustSize();
    } else {
        ui->progressBar->setVisible(true);
        ui->progressBar->setValue(procent);
    }
}



void MainWindow::on_btRegim_clicked()
{
    if (ui->frame_Arhiv->isVisible()){
        ui->frame_Arhiv->setVisible(false);
        SM.setSignalSource(SignalsManager::ssReal);
    } else {
        QSettings settings;
        QString selectedFilter;
        archiveFileName=settings.value("Main/archiveFileName").toString();
        archiveFileName = QFileDialog::getOpenFileName( this,tr("Открыть.."),archiveFileName,tr("pks2 Files (*.pks2);;All Files (*)"),&selectedFilter,nullptr);
        if (!archiveFileName.isEmpty()){
            SM.setSignalSource(SignalsManager::ssArchive);
            archiveManager->setGtBuffers(SM.XB[SignalsManager::ssArchive]);
            IArchiveReader::TArchiveSrc asrc;
            asrc.reader=archiveReaderPKS;
            asrc.params=archiveFileName;
            archiveManager->setArchiveSrc(asrc);

            ui->frame_Arhiv->setVisible(true);
        }
    }

}

void MainWindow::on_positionChaged(int newpos)
{
    ui->horizontalSlider->blockSignals(true);
    ui->horizontalSlider->setMaximum(archiveManager->recordsCount());
    ui->horizontalSlider->setValue(newpos);
    ui->horizontalSlider->blockSignals(false);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    archiveManager->setRecord(value);
}

void MainWindow::on_step()
{
    if (onGoPrev) archiveManager->go(-1);
    if (onGoNext) archiveManager->go( 1);
}

void MainWindow::on_btPrev_clicked()
{
    onGoPrev=!onGoPrev;
    if (onGoPrev) onGoNext=false;
    ui->btPrev->setDown(onGoPrev);
    ui->btNext->setDown(onGoNext);
}

void MainWindow::on_btNext_clicked()
{
    onGoNext=!onGoNext;
    if (onGoNext) onGoPrev=false;
    ui->btPrev->setDown(onGoPrev);
    ui->btNext->setDown(onGoNext);
}

void MainWindow::on_btFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName( this,
                                             tr("Открыть.."),"",
                                             tr("xml Files (*.xml);;All Files (*)"));
    if (!fileName.isEmpty())
            openFile(fileName);
}
