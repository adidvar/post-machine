#include "postmachinemodel.hpp"

#include <QFile>
#include <QTextStream>

PostMachineModel::PostMachineModel() {}

bool PostMachineModel::isRunning() const
{
    return m_is_working;
}

void PostMachineModel::moveTape(Tape::index_t delta)
{
    auto head = getTape().getHead();
    getTape().setHead(head + delta);
}

void PostMachineModel::run()
{
    m_is_working = true;
}

void PostMachineModel::stop()
{
    m_is_working = false;
}

int PostMachineModel::getTimerDelay()
{
    return m_timer_delay;
}

void PostMachineModel::setTimerDelay(int delay)
{
    m_timer_delay = delay;
}

QString PostMachineModel::getProblem() const { return m_problem; }

void PostMachineModel::setProblem(QString problem) { m_problem = problem; }

void PostMachineModel::saveToFile(QString url) const {
    QFile file(url);
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    if (!file.isOpen()) {
      file.close();
      return;
    }
    QTextStream io(&file);

    io << m_problem << '\n';
    io << "<CODE>" << '\n';

    for (auto value : getCommands()) {
      io << value.getCommand() << '\t' << value.getJumps() << '\t'
         << value.getComment().remove('\t') << '\n';
    }
    io << "<TAPE>" << '\n';
    getTape().saveToDevice(io);

    file.close();
}

void PostMachineModel::loadFromFile(QString url) {
    QFile file(url);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!file.isOpen()) {
      file.close();
      return;
    }
    getCommands().clear();

    QTextStream io(&file);

    m_problem.clear();

    QString line;
    do {
      m_problem += line;
      line = io.readLine();
    } while (!line.contains("<CODE>"));

    while (!io.atEnd()) {
      line = io.readLine();
      if (line.contains("<TAPE>")) break;
      auto parts = line.split('\t');
      getCommands().push_back(Command(parts[0], parts[1], parts[2]));
    }
    getTape().loadFromDevice(io);

    file.close();
}
