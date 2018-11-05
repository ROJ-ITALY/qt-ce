#include "MainWindow.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // Set the defualt application font
  int id = QFontDatabase::addApplicationFont(
      ":/resources/fonts/liberation-sans.regular.ttf");
  a.setFont(QFont(QFontDatabase::applicationFontFamilies(id).at(0), 24, 1));

  MainWindow w;

  w.showFullScreen();
  w.setCursor(Qt::BlankCursor);
  w.showMaximized();
  // For debugging
  //w.setGeometry(0,0,800,600);
  //w.show();

  return a.exec();
}
