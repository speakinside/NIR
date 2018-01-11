#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include <QAction>
struct Core;
class ControlPanel;
class SpectraView;
class SampleTable;
class QMenuBar;
class QToolBar;
class QStatusBar;
class QAction;

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  QHash<QString, QAction *> actionMap;
  QMap<QString,QAction *> menuActionMap;
  Core *core;
  QMenuBar *initMenuBar();
  QToolBar *initToolBar();
  QStatusBar *initStatusBar();
  SpectraView *initSpectraView();
  SampleTable *initSampleTable();
  ControlPanel *initControlPanel();
  void connectEverything();

public:
  SpectraView *spectraView;
  SampleTable *sampleTable;
  ControlPanel *controlPanel;
  explicit MainWindow(Core *core, QWidget *parent = nullptr);

signals:

public slots:
  void connectDevice();
  void saveData();
  void measureDark();
  void measureRef();
  void measureSpectrum();
};

#endif // MAINWINDOW_H
