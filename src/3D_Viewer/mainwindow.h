#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QSettings>
#include <QShortcut>

#include "glwidget.h"
// Includes for screen capture
#include <QProcess>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_QuitButton_clicked();
  void on_loadModelFileButton_clicked();
  void on_changeBGColorButton_clicked();

  void on_moveUpButton_clicked();
  void on_moveDownButton_clicked();
  void on_moveLeftButton_clicked();
  void on_moveRightButton_clicked();
  void on_moveForwardButton_clicked();
  void on_moveBackwardButton_clicked();
  void on_smallerButton_clicked();
  void on_biggerButton_clicked();

  void on_rotateUpButton_clicked();
  void on_rotateDownButton_clicked();
  void on_rotateLeftButton_clicked();
  void on_rotateRightButton_clicked();
  void on_rotateClockwiseButton_clicked();
  void on_rotateCounterclockwiseButton_clicked();

  void on_parallelButton_clicked();
  void on_centralButton_clicked();

  void on_solidButton_clicked();
  void on_dashedButton_clicked();
  void on_thinnerButton_clicked();
  void on_thickerButton_clicked();
  void on_edgesColorButton_clicked();

  void on_verticesDecreaseSizeButton_clicked();
  void on_verticesIncreaseSizeButton_clicked();
  void on_verticesColorButton_clicked();
  void on_noneDisplayMethodButton_clicked();
  void on_squareDisplayMethodButton_clicked();
  void on_circleDisplayMethodButton_clicked();

  void onModelLoaded(int numVertices, int numEdges);

  void on_screenshotButton_clicked();
  void on_screencastButton_clicked();
  void captureScreencastFrame();
  void createGifFromImages(const QString &gifFilename,
                           const QStringList &imageFilenames, int delay);

  void on_resetPreferencesButton_clicked();

  void on_yMoveDoubleSpinBox_editingFinished();
  void on_xMoveDoubleSpinBox_editingFinished();
  void on_zMoveDoubleSpinBox_editingFinished();
  void on_sizeDoubleSpinBox_editingFinished();
  void on_xRotateDoubleSpinBox_editingFinished();
  void on_yRotateDoubleSpinBox_editingFinished();
  void on_zRotateDoubleSpinBox_editingFinished();

 private:
  Ui::MainWindow *ui;
  GLWidget *glWidget;
  QLabel *numVerticesLabel;
  QLabel *numEdgesLabel;
  // Variables for screencast
  QTimer *screencastTimer;
  int screencastFrameCount;
  QList<QImage> screencastFrames;
};
#endif  // MAINWINDOW_H
