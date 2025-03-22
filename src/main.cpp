#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "postmachinecontroller.hpp"
#include "postmachinemodel.hpp"
#include "postmachineview.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QApplication::setOrganizationName("adidvar");
  QApplication::setApplicationName("post-machine");

  PostMachineModel *model = new PostMachineModel();
  PostMachineController *controller = new PostMachineController(model);
  PostMachineView *view = new PostMachineView(controller);
  view->loadDataFromModel(*model);
  view->show();

  return a.exec();
}
