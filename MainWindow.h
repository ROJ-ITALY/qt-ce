#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "LinuxHandler.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  // Constructor
  explicit MainWindow(QWidget *parent = nullptr);

  // Destructor
  ~MainWindow();

private slots:
  void on_bnt_install_released();
  void on_btn_cancel_released();
  void on_progressBar_valueChanged(int value);
  void onInstallationFinished(int exitCode);
  void onInstallationError(QByteArray error);

private:
  // Variables
  Ui::MainWindow *ui;
  LinuxHandler *lhandler;

  // Functions
  void init_ui();
};

#endif // MAINWINDOW_H
