#include "postmachinecontroller.hpp"
#include "QDebug"

PostMachineController::PostMachineController(PostMachineModel *model)
    : m_model(model)
{}

void PostMachineController::moveTapeRight()
{
    m_model->moveTape(1);
}

void PostMachineController::moveTapeLeft()
{
    m_model->moveTape(-1);
}

void PostMachineController::commandEntered(size_t index,
                                           QString data,
                                           QString params,
                                           QString comment)
{
    m_model->getCommands()[index] = Command(data, params, comment);
}

void PostMachineController::insertCommand(size_t index)
{
    if (m_model->getCommands().size() < index)
        m_model->getCommands().append(Command());
    else
        m_model->getCommands().insert(index, Command());
}

void PostMachineController::deleteCommand(size_t index)
{
    if (m_model->getCommands().size() > index)
        m_model->getCommands().remove(index);
    else if (!m_model->getCommands().empty())
        m_model->getCommands().pop_back();
}

void PostMachineController::changeTapeValue(Tape::index_t index)
{
    m_model->getTape().setValue(index, !m_model->getTape().getValue(index));
}

void PostMachineController::start()
{
    m_model->run();
}

void PostMachineController::stop()
{
    m_model->stop();
}

void PostMachineController::step() { m_model->nextStep(); }

void PostMachineController::cleanAll()
{
    m_model->setCommands({});
    m_model->getTape().clear();
}

void PostMachineController::loadFile(QString url)
{
    m_model->loadFromFile(url);
}

void PostMachineController::saveFile(QString url)
{
    m_model->saveToFile(url);
}

void PostMachineController::loadTape(QString url)
{
    m_model->getTape().loadFromFile(url);
}

void PostMachineController::saveTape(QString url)
{
    m_model->getTape().saveToFile(url);
}

void PostMachineController::timer()
{
    if (m_model->getStatus() != PostMachine::NoError) {
        m_model->reset();
        m_model->stop();
    }
    if (m_model->isRunning()) m_model->nextStep();
}

void PostMachineController::reset() {
    m_model->reset();
}

void PostMachineController::highSpeed()
{
    m_model->setTimerDelay(100);
}

void PostMachineController::normalSpeed() { m_model->setTimerDelay(500); }

void PostMachineController::lowSpeed() { m_model->setTimerDelay(1200); }

void PostMachineController::customSpeed(int speed)
{
    m_model->setTimerDelay(speed);
}

void PostMachineController::problemTextChanged(QString text) {
    m_model->setProblem(text);
}

PostMachineModel *PostMachineController::getModel() const
{
    return m_model;
}
