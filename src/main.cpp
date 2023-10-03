#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "postmachinecontroller.hpp"
#include "postmachinemodel.hpp"
#include "postmachineview.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QTranslator translator;

  if (translator.load(QLocale::system(), "postmachine", ".", "."))
    QApplication::installTranslator(&translator);
  else if (translator.load(QLocale::system(), "postmachine", ".",
                           "/translations"))
    QApplication::installTranslator(&translator);

  PostMachineModel *model = new PostMachineModel();

  PostMachineController *controller = new PostMachineController(model);

  PostMachineView *view = new PostMachineView(controller);

  view->loadDataFromModel(*model);

  view->show();

  return a.exec();
}
