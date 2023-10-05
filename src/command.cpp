#include "command.hpp"

#include <QList>

Command::Command() {}

Command::Command(QString command, QString jumps, QString comment)
    : m_comment(comment) {
  QChar character = ' ';
  if (command.simplified().size() > 0) character = command.simplified()[0];
  if (character == '<')
    m_type = MoveLeft;
  else if (character == '>')
    m_type = MoveRight;
  else if (character == '0')
    m_type = WriteZero;
  else if (character == '1')
    m_type = WriteOne;
  else if (character == '?')
    m_type = MoveIF;
  else if (character == '!')
    m_type = End;
  else
    m_type = Invalid;

  bool ok = false;
  if (jumps.contains(',') && m_type == MoveIF) {
    auto vectors = jumps.split(QString(","));
    m_jump1 = vectors[0].toInt(&ok);
    if (!ok) m_jump1 = kinvalid_jump;
    m_jump2 = vectors[1].toInt(&ok);
    if (!ok) m_jump2 = kinvalid_jump;
  } else if (m_type != MoveIF) {
    m_jump1 = jumps.toInt(&ok);
    if (!ok) m_jump1 = kinvalid_jump;
  } else {
    m_jump1 = kinvalid_jump;
    m_jump2 = kinvalid_jump;
  }
}

Command::Type Command::getType() const
{
    return m_type;
}

size_t Command::getJump1() const
{
    return m_jump1;
}

size_t Command::getJump2() const
{
    return m_jump2;
}

bool Command::checkCommand(QString command) {
  QChar character = ' ';
  if (command.simplified().size() > 0) character = command.simplified()[0];
  if (character == '<')
    return true;
  else if (character == '>')
    return true;
  else if (character == '0')
    return true;
  else if (character == '1')
    return true;
  else if (character == '?')
    return true;
  else if (character == '!')
    return true;
  else
    return false;
}

bool Command::checkJumps(QString command, QString jumps) {
  Type m_type;
  QChar character = ' ';
  if (command.simplified().size() > 0) character = command.simplified()[0];
  if (character == '<')
    m_type = MoveLeft;
  else if (character == '>')
    m_type = MoveRight;
  else if (character == '0')
    m_type = WriteZero;
  else if (character == '1')
    m_type = WriteOne;
  else if (character == '?')
    m_type = MoveIF;
  else if (character == '!')
    m_type = End;
  else
    m_type = Invalid;

  bool ok = false;
  if (jumps.contains(',') && m_type == MoveIF) {
    auto vectors = jumps.split(QString(","));
    vectors[0].toInt(&ok);
    if (!ok) return false;
    vectors[1].toInt(&ok);
    if (!ok) return false;
  } else if (m_type != MoveIF) {
    jumps.toInt(&ok);
    if (!ok) return false;
  }
  return true;
}
QString Command::getCommand() {
  switch (m_type) {
    case MoveLeft:
      return "<";
    case MoveRight:
      return ">";
    case WriteZero:
      return "0";
    case WriteOne:
      return "1";
    case MoveIF:
      return "?";
    case End:
      return "!";
    case Invalid:
      return "";
      break;
  }
}

QString Command::getJumps()
{
    QString string;
    if (m_jump1 != kinvalid_jump) {
        string += QString::number(m_jump1);
        if (m_jump2 != kinvalid_jump) {
            string += ',';
            string += QString::number(m_jump2);
        }
    }
    return string;
}

QString Command::getComment()
{
    return m_comment;
}
