#include "postmachineview.hpp"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

#include "./ui_postmachineview.h"
#include "postmachinecontroller.hpp"
#include "postmachinemodel.hpp"

PostMachineView::PostMachineView(PostMachineController *controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PostMachineView)
    , controller(controller)
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);
    m_timer->setSingleShot(true);
    ui->commands_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->commands_widget->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);

    connect(ui->tape_widget, &TapeWidget::valueChanged, this,
            &PostMachineView::tape_value_changed);

    connect(ui->about_action, &QAction::triggered, this, &PostMachineView::about);

    connect(ui->instruction_action, &QAction::triggered, this, &PostMachineView::instruction);

    connect(ui->exit_action, &QAction::triggered, this, &PostMachineView::exit);

    connect(ui->new_file_action, &QAction::triggered, this, &PostMachineView::new_file);

    connect(ui->save_action, &QAction::triggered, this, &PostMachineView::save_file);

    connect(ui->load_action, &QAction::triggered, this, &PostMachineView::load_file);

    connect(m_timer, &QTimer::timeout, this, &PostMachineView::timer);

    controller->normalSpeed();
}

PostMachineView::~PostMachineView()
{
    delete ui;
}

void PostMachineView::loadDataFromModel(const PostMachineModel &model)
{
    m_editing = true;

    auto status = model.getStatus();
    switch (status) {
      case PostMachine::NoError:
        break;
      case PostMachine::OutOfCommands:
        QMessageBox::warning(this, tr("Warning"),
                             tr("The Post Machine stopped due to switching to "
                                "a command that does not exist"));
        break;
      case PostMachine::EndCommand:
        QMessageBox::information(
            this, tr("Information"),
            tr("The Post Machine stopped at the stop command"));
        break;
      case PostMachine::InvalidCommand:
        QMessageBox::warning(
            this, tr("Warning"),
            tr("The Post Machine stopped due to an incorrect command"));
        break;
      case PostMachine::InvalidWrite:
        QMessageBox::warning(this, tr("Warning"),
                             tr("The Post Machine stopped due to an incorrect "
                                "write command (cell overwriting)"));
        break;
    }
    ui->tape_widget->loadFromTape(model.getTape());

    ui->tape_widget->setEnabled(!model.isRunning());
    ui->problem_edit->setEnabled(!model.isRunning());
    ui->insert_button->setEnabled(!model.isRunning());
    ui->remove_button->setEnabled(!model.isRunning());
    ui->scroll_left_button->setEnabled(!model.isRunning());
    ui->scroll_right_button->setEnabled(!model.isRunning());
    ui->run_button->setEnabled(!model.isRunning());
    ui->stop_button->setEnabled(model.isRunning());
    ui->step_button->setEnabled(!model.isRunning());
    ui->commands_widget->setEnabled(!model.isRunning());

    auto commands = model.getCommands();

    ui->commands_widget->setRowCount(commands.size());
    for (int i = 0; i < commands.size(); ++i) {
      auto command_item = new QTableWidgetItem(commands[i].getCommand());
      ui->commands_widget->setItem(i, 0, command_item);
      auto jump_item = new QTableWidgetItem(commands[i].getJumps());
      ui->commands_widget->setItem(i, 1, jump_item);
      auto comment_item = new QTableWidgetItem(commands[i].getComment());
      ui->commands_widget->setItem(i, 2, comment_item);
    }
    if (model.isRunning())
      ui->commands_widget->selectRow(model.getCommandIndex());

    m_editing = false;
}

void PostMachineView::on_scroll_left_button_clicked()
{
    controller->moveTapeLeft();
    loadDataFromModel(*controller->getModel());
}

void PostMachineView::on_scroll_right_button_clicked()
{
    controller->moveTapeRight();
    loadDataFromModel(*controller->getModel());
}

void PostMachineView::on_commands_widget_itemChanged(QTableWidgetItem *item)
{
    if (m_editing) return;

    auto row = item->row();
    auto command = ui->commands_widget->item(row, 0)->text();
    auto argc = ui->commands_widget->item(row, 1)->text();
    auto comment = ui->commands_widget->item(row, 2)->text();

    controller->commandEntered(row, command, argc, comment);

    if (Command(command, argc, comment).getType() == Command::Invalid)
      QApplication::beep();

    loadDataFromModel(*controller->getModel());
}

void PostMachineView::on_insert_button_clicked()
{
    controller->insertCommand(ui->commands_widget->currentRow());
    loadDataFromModel(*controller->getModel());
}

void PostMachineView::on_remove_button_clicked()
{
    controller->deleteCommand(ui->commands_widget->currentRow());
    loadDataFromModel(*controller->getModel());
}

void PostMachineView::tape_value_changed(Tape::index_t index)
{
    controller->changeTapeValue(index);
    loadDataFromModel(*controller->getModel());
}

void PostMachineView::on_run_button_clicked()
{
    controller->start();
    m_timer->start(controller->getModel()->getTimerDelay());
    loadDataFromModel(*controller->getModel());
}

void PostMachineView::on_stop_button_clicked()
{
    controller->stop();
    loadDataFromModel(*controller->getModel());
}

void PostMachineView::on_step_button_clicked()
{
    controller->step();
    ui->commands_widget->selectRow(controller->getModel()->getCommandIndex());
    loadDataFromModel(*controller->getModel());
    if (controller->getModel()->getStatus() != PostMachine::NoError)
      controller->getModel()->reset();
}

void PostMachineView::new_file()
{
    controller->cleanAll();
    loadDataFromModel(*controller->getModel());
}

void PostMachineView::load_file()
{
    controller->loadFile(
        QFileDialog::getOpenFileName(this, tr("Open file"), "", "*.post"));
    ui->problem_edit->setText(controller->getModel()->getProblem());
    loadDataFromModel(*controller->getModel());
}

void PostMachineView::save_file()
{
    controller->saveFile(
        QFileDialog::getSaveFileName(this, tr("Save file"), "", "*.post"));
    loadDataFromModel(*controller->getModel());
}

void PostMachineView::exit()
{
    QApplication::exit(0);
}

void PostMachineView::about()
{
    QMessageBox::about(
        this, tr("About"),
        tr("The program is designed for educational purposes, \n"
           "distributed under under the GNU GPL3 license. \n"
           "The author is not responsible for the use of the program. \n"
           "The source code can be downloaded from the link \n\n"
           "https://github.com/adidvar/PostMachine"));
}

void PostMachineView::instruction()
{
    QMessageBox::about(
        this, tr("Instruction"),
        tr("Instruction set:"
           "1 - writes to cell \n"
           "0 - erases cell \n"
           "< - step left \n"
           "> - step right \n"
           "? - jumps to first jump point if cell haven`t mark  \n"
           "else jumps to second jump point \n"
           "! stops machine \n"
           "Jump types: \n"
           "1) \"\" empty jumps moves you to the next command \n"
           "2) \"1\" jumps to the first command \n"
           "3) \"3,4\" jump for ? command, if cell is marked  \n"
           "jumps to 4 else jumps to 3 \n"
           "Stop types: \n"
           "1) stop after ! command \n"
           "2) stop after command ptr goes out from commands \n"
           "3) stop when program hits on incorrect commmand \n"));
}

void PostMachineView::timer()
{
    controller->timer();
    if (controller->getModel()->isRunning())
        m_timer->start(controller->getModel()->getTimerDelay());
    loadDataFromModel(*controller->getModel());
}

void PostMachineView::on_fast_speed_action_triggered()
{
    controller->highSpeed();
}

void PostMachineView::on_normal_speed_action_triggered()
{
    controller->normalSpeed();
}

void PostMachineView::on_slow_speed_action_triggered()
{
    controller->lowSpeed();
}

void PostMachineView::on_custom_speed_action_triggered()
{
    controller->customSpeed(QInputDialog::getInt(
        this, tr("Custom speed"), tr("Enter delay between commands in ms")));
}

void PostMachineView::on_problem_edit_textChanged() {
    if (m_editing) return;
    controller->problemTextChanged(ui->problem_edit->toPlainText());
    loadDataFromModel(*controller->getModel());
}
