#ifndef LINUXHANDLER_H
#define LINUXHANDLER_H

#include <QByteArray>
#include <QObject>
#include <QProcess>
#include <QString>

class LinuxHandler : public QObject {
  Q_OBJECT

public:
  // Constructor
  explicit LinuxHandler(QObject *parent = nullptr);

  // Fucntions
  void start_instalation();

  // Static functions
  static QString get_new_version();
  static QString get_current_version();

private:
  static QByteArray run_script(const QString arg);

signals:
  void installation_progress(int value);
  void installation_finished(int exitCode);
  void installation_error(QByteArray error);

private slots:
  void process_output();
  void process_finished(int exitCode);

private:
  QProcess handle;
  const static QString installation_args;
  const static QString cmd;
  const static QString current_version_args;
  const static QString new_version_args;
};

#endif // LINUXHANDLER_H
