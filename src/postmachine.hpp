#ifndef POSTMACHINE_HPP
#define POSTMACHINE_HPP

#include <QVector>

#include "command.hpp"
#include "tape.hpp"

class PostMachine {
 public:
  enum Status {
    NoError,
    OutOfCommands,
    EndCommand,
    InvalidCommand,
    InvalidWrite,
  };

  Status getStatus() const;

  void nextStep();
  void reset();

  size_t getCommandIndex() const;
  void setCommandIndex(size_t index);

  const QVector<Command> &getCommands() const;
  QVector<Command> &getCommands();
  void setCommands(const QVector<Command> &commands);

  const Tape &getTape() const;
  Tape &getTape();
  void setTape(const Tape &tape);

 private:
  Status m_status = NoError;

  size_t m_command_index = 0;
  QVector<Command> m_commands;

  Tape m_tape;
};

#endif
