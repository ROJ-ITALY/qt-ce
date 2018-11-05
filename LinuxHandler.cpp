#include "LinuxHandler.h"

// COMMANDS AND SCRIPTS //
const QString LinuxHandler::cmd = QString("/bin/bash");
const QString LinuxHandler::current_version_args =
    QString("/home/root/current_version.sh");
const QString LinuxHandler::new_version_args =
    QString("/home/root/new_version.sh");
const QString LinuxHandler::installation_args =
    QString("/home/root/install_image.sh");

LinuxHandler::LinuxHandler(QObject *parent) : QObject(parent) {}

// FUNCTIONS //
void LinuxHandler::start_instalation() {
  connect(&handle, &QProcess::readyReadStandardOutput, this,
          &LinuxHandler::process_output);
  connect(&handle, SIGNAL(finished(int)), this, SLOT(process_finished(int)));

  handle.start(cmd, {installation_args});
}

// SLOTS //
void LinuxHandler::process_output() {
  QByteArray trimmed = handle.readLine().trimmed();
  int index = trimmed.indexOf('%');

  // This happens when you unplug the usb stick during installation
  if ( trimmed.contains("Input/output error") ) {
      handle.kill();
      emit installation_error(handle.readAllStandardError());
  }

  if (index > 0) {
    QByteArray res = trimmed.mid(index - 2, 2);
    // Add one beceause rsync never ends at 100%
    emit installation_progress(res.toInt());
  }
}

void LinuxHandler::process_finished(int exitCode) {
  // read stderr from handle
  emit installation_finished(exitCode);
  if (exitCode != 0) {
    emit installation_error(handle.readAllStandardError());
  }
}

// STATIC FUNCTIONS //
QString LinuxHandler::get_new_version() {
  return LinuxHandler::run_script(new_version_args);
}

QString LinuxHandler::get_current_version() {
  return LinuxHandler::run_script(current_version_args);
}

QByteArray LinuxHandler::run_script(const QString arg) {
  QProcess handle;

  handle.start(cmd, {arg});
  handle.waitForFinished();

  // TODO check exit codes
  if (handle.exitCode() != 0) {
  }

  return handle.readAllStandardOutput().trimmed();
}
