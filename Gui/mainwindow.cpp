#include "mainwindow.h"
#include "TabArea/controlpanel.h"
#include "TableArea/sampletable.h"
#include "SpectraArea/spectraview.h"
#include "Dialogs/measuredialog.h"
#include "Dialogs/connectdialog.h"
#include "Dialogs/savedialog.h"
#include "Core/init.h"

#include <plog/Log.h>
#include <QGridLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QMessageBox>
#include <QSplitter>
#include <QInputDialog>
#include <QMessageBox>
#include <QSizePolicy>
#include <QActionGroup>
#include <QColorDialog>
#include <QPersistentModelIndex>
#include <QHeaderView>
#include <QProgressDialog>
#include <QProgressBar>
#include <QFile>
#include <Qt>

MainWindow::MainWindow(Core *core, QWidget *parent) : core(core), QMainWindow(parent)
{
    //Basic Geometry set
    setMinimumSize(1200, 800);

    //initialize widgets
    setMenuBar(initMenuBar());
    addToolBar(initToolBar());
    setStatusBar(initStatusBar());
    spectraView = initSpectraView();
    sampleTable = initSampleTable();
    controlPanel = initControlPanel();

    auto *verticalSplitter = new QSplitter(Qt::Vertical, this);
    auto *horizontalSplitter = new QSplitter(Qt::Horizontal, this);
    verticalSplitter->addWidget(spectraView);
    verticalSplitter->addWidget(sampleTable);
    horizontalSplitter->addWidget(verticalSplitter);
    horizontalSplitter->addWidget(controlPanel);
    setCentralWidget(horizontalSplitter);

    //singal connect
    connectEverything();
}

void MainWindow::connectDevice()
{
    static ConnectDialog *dialog = new ConnectDialog(this);
    dialog->exec();
    if(dialog->result() == QDialog::Rejected)
        return;
    core->deviceInterface->openDevice(dialog->getPortName(),dialog->getBaudRate());
    LOG_DEBUG << QString("PortName:%1 BaudRate: %2").arg(dialog->getPortName(),QString::number(dialog->getBaudRate()));
}

void MainWindow::saveData()
{
    static auto dialog = new SaveDialog(this);
    dialog->exec();
}

void MainWindow::measureDark()
{
    core->dataModel->getDark(1);
}

void MainWindow::measureRef()
{
    core->dataModel->getRef(1);
}

void MainWindow::measureSpectrum()
{
    static MeasureDialog *dialog = new MeasureDialog(this);
    dialog->exec();
    if(dialog->result() == QDialog::Rejected)
        return;
    QString name = dialog->getSampleName();
    if(name.isEmpty())
        return;
    int repeatTimes = dialog->getRepeatTimes();
    QProgressDialog progress("Measure Progress","Abort",0,repeatTimes,this);
        progress.setMinimumDuration(0);
            progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);
    progress.setValue(1);
    //progress.setBar(new QProgressBar);


    //progress.show();
    for(int i=1;i<=repeatTimes;i++)
    {
        core->dataModel->getSpectrum(name,i);
        progress.setValue(i);
    }
    /*
    QString name = QInputDialog::getText(this, tr("Before Measure"), tr("Sample Name:"));
    if (!name.isEmpty())
        core->dataModel->getSpectrum(name, 1);
    */
}

QMenuBar *MainWindow::initMenuBar()
{
    LOG_DEBUG << "Init MenuBars";
    auto menuBar = new QMenuBar();

    auto startMenu = new QMenu(tr("Start"));
    //TO DO
    menuActionMap.insert("start-connect",startMenu->addAction(tr("Connect")));
    menuActionMap.insert("start-save",startMenu->addAction(tr("Save ..."),this,&MainWindow::saveData));

    auto confMenu = new QMenu(tr("Configure"));
    //TO DO
    menuActionMap.insert("conf-blabla",confMenu->addAction(tr("blabla")));

    auto spectraMenu = new QMenu(tr("Spectra"));
    //TO DO

    menuActionMap.insert("spectra-setToDefaultScale",spectraMenu->addAction(tr("set to default scale")));
    menuActionMap.insert("spectra-saveCurrentImage",spectraMenu->addAction(tr("save current image")));

    auto helpMenu = new QMenu(tr("Help"));
    //Build "About" menu
    menuActionMap.insert("help-aboutNIR",helpMenu->addAction(tr("About NIR")));
    menuActionMap.insert("help-aboutPlog",helpMenu->addAction(tr("About Plog"),[this]{
        QFile f(":/text/res/aboutPlog.txt");
        f.open(QIODevice::ReadOnly);
        QMessageBox::about(this,tr("About plog"),f.readAll());}));
    menuActionMap.insert("help-aboutQt",helpMenu->addAction(tr("About Qt"), [this]{LOG_DEBUG<<"Trigger \"About Qt\" Page"; QMessageBox::aboutQt(this); }));

    menuBar->addMenu(startMenu);
    menuBar->addMenu(confMenu);
    menuBar->addMenu(spectraMenu);
    menuBar->addMenu(helpMenu);
    LOG_DEBUG << "Init MenuBars...Done";
    return menuBar;
}

QToolBar *MainWindow::initToolBar()
{
    LOG_DEBUG << "Init ToolBars";
    auto toolBar = new QToolBar();
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    toolBar->setMovable(false);
    //QAction below will use icon insteading of string to exhibit
    actionMap.insert("connect", toolBar->addAction(QIcon(":icons/res/connect.png"), "Connect"));
    actionMap.insert("dark", toolBar->addAction(QIcon(":icons/res/dark.png"), "Dark"));
    actionMap.insert("ref", toolBar->addAction(QIcon(":icons/res/ref.png"), "Ref"));
    actionMap.insert("spectra", toolBar->addAction(QIcon(":icons/res/spectra.png"), "Spectra"));
    actionMap.insert("spectraFast", toolBar->addAction(QIcon(":icons/res/spectra.png"), "Spectra Fast"));
    toolBar->addSeparator();
    actionMap.insert("absorbance", toolBar->addAction("Absorbance"));
    actionMap.insert("intensity", toolBar->addAction("Intensity"));

    actionMap["absorbance"]->setCheckable(true);
    actionMap["intensity"]->setCheckable(true);
    actionMap["absorbance"]->setChecked(false);
    actionMap["intensity"]->setChecked(true);

    toolBar->addSeparator();
    actionMap.insert("clearChart", toolBar->addAction("Clear Chart"));
    actionMap.insert("delSelectedEntry", toolBar->addAction("Delete Selected Entry"));
    actionMap.insert("delAll", toolBar->addAction("Delete All Entry"));

    LOG_DEBUG << "Init ToolBars...Done";
    return toolBar;
}

QStatusBar *MainWindow::initStatusBar()
{
    LOG_DEBUG << "Init StatusBars";
    return new QStatusBar();
}

SpectraView *MainWindow::initSpectraView()
{
    LOG_DEBUG << "Init SpectraView";
    return new SpectraView(this);
}

SampleTable *MainWindow::initSampleTable()
{
    LOG_DEBUG << "Init SampleTable";
    return new SampleTable(core->dataModel, this);
}

ControlPanel *MainWindow::initControlPanel()
{
    LOG_DEBUG << "Init ControlPane";
    return new ControlPanel(this);
}

void MainWindow::connectEverything()
{
    LOG_DEBUG << "Build Connections";
    // MenuBar
    connect(menuActionMap["start-connect"],&QAction::triggered,actionMap["connect"],&QAction::trigger);
    // ToolBar
    connect(actionMap["connect"],&QAction::triggered,this,&MainWindow::connectDevice);
    connect(actionMap["dark"], &QAction::triggered, this, &MainWindow::measureDark);
    connect(actionMap["ref"], &QAction::triggered, this, &MainWindow::measureRef);
    connect(actionMap["spectra"], &QAction::triggered, this, &MainWindow::measureSpectrum);
    connect(actionMap["spectraFast"], &QAction::triggered, [this](bool) { this->core->dataModel->getSpectrum(tr("test"), 1); });
    connect(actionMap["clearChart"], &QAction::triggered, spectraView, &SpectraView::hideAllSeries);
    connect(actionMap["delSelectedEntry"], &QAction::triggered, [this] {
        auto list = QList<QPersistentModelIndex>();
        for (auto &i : sampleTable->selectedIndexes())
            if (i.row() != 1 && i.row() != 0)
                list << QPersistentModelIndex(i);
        for (auto &i : list)
            if (i.isValid())
                core->dataModel->removeRow(i.row());
    });
    connect(actionMap["delAll"], &QAction::triggered, [this] { core->dataModel->removeRows(2, core->dataModel->rowCount() - 2); });
    {
        auto chartActionGroup = new QActionGroup(this);
        chartActionGroup->addAction(actionMap["absorbance"]);
        chartActionGroup->addAction(actionMap["intensity"]);
        connect(chartActionGroup, &QActionGroup::triggered, [this](QAction *action) { spectraView->changeChart(action->text()); });
    }

    //sampleTable
    connect(sampleTable, &QTableView::doubleClicked, [this](auto &index) {
        if (index.column() == 4)
            core->dataModel->changeVisible(index);
    });
    connect(sampleTable->verticalHeader(), &QHeaderView::sectionDoubleClicked,
            [this](int logicalIndex) {
                auto color = QColorDialog::getColor(std::get<4>(core->dataModel->data_table.at(logicalIndex))->color(), this, tr("Choose color."));
                if (!color.isValid())
                    return;
                core->dataModel->setHeaderData(logicalIndex, Qt::Vertical, color, Qt::DecorationRole);
            });
    //contralPanel
    connect(core->deviceInterface,&DeviceInterface::portConnected,controlPanel->deviceConditionBox,&DeviceConditionBox::setConnectivity);
    connect(core->deviceInterface,&DeviceInterface::portDisconnected,controlPanel->deviceConditionBox,&DeviceConditionBox::setLost);
    //Measure Logic
    connect(core->dataModel, &DataModel::newSeriesCreated, spectraView, &SpectraView::addNewSeriesToChart);
    connect(core->dataModel, &DataModel::noRefAndDark,
            [this] { QMessageBox::warning(this, tr("Warning"), tr("Measure Refference and Dark spectra before sample measurement!"), QMessageBox::Ok); });
    connect(core->dataModel, &DataModel::noConnection,
            [this] { QMessageBox::warning(this, tr("Warning"), tr("Connect Device before sample measurement!"), QMessageBox::Ok); });
}
