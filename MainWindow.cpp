#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      lhandler(new LinuxHandler(this)) {
  init_ui();
}

void MainWindow::init_ui() {
  ui->setupUi(this);
  ui->stackedWidget->setCurrentWidget(ui->prompt);

  const QString new_version = LinuxHandler::get_new_version();
  const QString current_version = LinuxHandler::get_current_version();

  ui->lbl_new->setText(new_version);
  ui->lbl_new_installing->setText("Installing: " + new_version);
  ui->lbl_current->setText(current_version);

  if (new_version == current_version) {
    ui->message->setText(QString("You will reinstall the current version"));
    ui->message->setStyleSheet("QLabel { color: #FFA500;}");
    ui->bnt_install->setText(QString("Reinstall"));
    ui->bnt_install->setStyleSheet(
        "QPushButton { background-color: #FFA500; color: white; color: white; "
        "border-radius: 10px;}");
    ui->label_3->setStyleSheet("QLabel { color: #FFA500; }");
    ui->lbl_new->setStyleSheet("QLabel { color: #FFA500; }");

  } else {
    ui->message->setText(QString("You will update the current version"));
    ui->bnt_install->setText(QString("Update"));
  }
}

void MainWindow::on_bnt_install_released() {
  // Set progress bar
  const int initialProgress = 0;
  ui->progressBar->setValue(initialProgress);

  // Switch screen
  ui->stackedWidget->setCurrentWidget(ui->installation);

  // Connect events
  connect(lhandler, &LinuxHandler::installation_progress, this,
          &MainWindow::on_progressBar_valueChanged);
  connect(lhandler, &LinuxHandler::installation_finished, this,
          &MainWindow::onInstallationFinished);
  connect(lhandler, &LinuxHandler::installation_error, this,
          &MainWindow::onInstallationError);

  // Start the installation
  lhandler->start_instalation();
}

void MainWindow::on_btn_cancel_released() {
  ui->lbl_state->setText(QString("Installation canceled"));
  ui->lbl_state->setStyleSheet("QLabel { color: orange; font-weight: bold;}");
  ui->lbl_final_msg->setText(
      QString("1) Unplug the USB device \n\n2) Switch power off/on"));
  ui->textBrowser->hide();
  ui->stackedWidget->setCurrentWidget(ui->finish);
}

void MainWindow::on_progressBar_valueChanged(int value) {
  int const cur_val = ui->progressBar->value();
  /*
  if (value > cur_val) {
    ui->progressBar->setValue(value);
  }
  */
  // If there are big jumps make sure the get rendered too
  if (cur_val + 1 < value) {
    for (int i = cur_val; i <= value; i++) {
      QThread::msleep(10);
      ui->progressBar->setValue(i);
    }
  } else if (cur_val + 1 == value) {
    ui->progressBar->setValue(value);
  }
}

void MainWindow::onInstallationFinished(int exitCode) {
  // TODO set the state
  if (exitCode == 0) {
    ui->lbl_state->setText(QString("Installation Finished"));
    ui->lbl_state->setStyleSheet("QLabel { color: green; font-weight: bold;}");
    ui->lbl_final_msg->setText(
        QString("1) Unplug the USB device \n\n2) Switch power off/on"));
    ui->textBrowser->hide();
    ui->stackedWidget->setCurrentWidget(ui->finish);
  } else {
    ui->lbl_state->setText(QString("Installation Failed"));
    ui->lbl_state->setStyleSheet("QLabel { color: red; font-weight: bold;}");
    ui->spacer_finished->changeSize(0, 0);
    ui->spacer_finished_2->changeSize(0, 0);
    ui->lbl_final_msg->setText(QString("Contact VDW"));
    ui->stackedWidget->setCurrentWidget(ui->finish);
  }
}

void MainWindow::onInstallationError(QByteArray error) {
  ui->textBrowser->append(error);
}

MainWindow::~MainWindow() { delete ui; }
