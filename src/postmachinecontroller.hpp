#ifndef POSTMACHINECONTROLLER_HPP
#define POSTMACHINECONTROLLER_HPP

#include "postmachinemodel.hpp"

class PostMachineController {
 public:
  PostMachineController(PostMachineModel *model);

  void moveTapeRight();
  void moveTapeLeft();

  void commandEntered(size_t index, QString data, QString params,
                      QString comment);
  void insertCommand(size_t index);
  void deleteCommand(size_t index);

  void changeTapeValue(Tape::index_t index);

  void start();
  void stop();
  void step();
  void reset();

  void cleanAll();
  void loadFile(QString url);
  void saveFile(QString url);
  void loadTape(QString url);
  void saveTape(QString url);

  void timer();

  void highSpeed();
  void normalSpeed();
  void lowSpeed();
  void customSpeed(int speed);

  void problemTextChanged(QString text);

  PostMachineModel *getModel() const;

 private:
  PostMachineModel *m_model;
};

#endif
