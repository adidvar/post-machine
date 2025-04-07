#include "postmachineview.hpp"

#include <QDesktopServices>
#include <QDirIterator>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QTranslator>

#include "./ui_postmachineview.h"
#include "postmachinecontroller.hpp"
#include "postmachinemodel.hpp"

const QStringList locations_dirs = {"share/postmachine/locales", ".",
                                    "/usr/share/postmachine/locales"};

PostMachineView::PostMachineView(PostMachineController *controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PostMachineView)
    , controller(controller)
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);

    translator.reset(new QTranslator());

    if (settings.contains("language")) {
      translator.reset(new QTranslator());
      if (translator.data()->load(settings.value("language").toString()))
        QApplication::installTranslator(translator.data());
    }

    m_timer->setSingleShot(true);
    ui->commands_widget->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->tape_widget, &TapeWidget::valueChanged, this,
            &PostMachineView::tape_value_changed);

    connect(ui->about_action, &QAction::triggered, this, &PostMachineView::about);

    connect(ui->instruction_action, &QAction::triggered, this, &PostMachineView::instruction);

    connect(ui->exit_action, &QAction::triggered, this, &PostMachineView::exit);

    connect(ui->new_file_action, &QAction::triggered, this, &PostMachineView::new_file);

    connect(ui->save_action, &QAction::triggered, this, &PostMachineView::save_file);

    connect(ui->load_action, &QAction::triggered, this, &PostMachineView::load_file);

    connect(ui->load_tape_action, &QAction::triggered, this, &PostMachineView::load_tape);

    connect(ui->save_tape_action, &QAction::triggered, this, &PostMachineView::save_tape);

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
                             tr("Post machine stopped because found a command "
                                "that don`t exist"));
        break;
      case PostMachine::EndCommand:
        QMessageBox::information(
            this, tr("Information"),
            tr("Post machine stopped because found a stop command"));
        break;
      case PostMachine::InvalidCommand:
        QMessageBox::warning(
            this, tr("Warning"),
            tr("Post machine stopped because found a invalid command"));
        break;
      case PostMachine::OverrideError:
        QMessageBox::warning(
            this, tr("Warning"),
            tr("Post machine stopped because tape is overrided"));
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
    ui->reset_button->setEnabled(!model.isRunning());

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

    // if (item->column() == 0)

    controller->commandEntered(row, command, argc, comment);

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
    loadDataFromModel(*controller->getModel());
    ui->commands_widget->selectRow(controller->getModel()->getCommandIndex());
    if (controller->getModel()->getStatus() != PostMachine::NoError)
      controller->getModel()->reset();
}

void PostMachineView::save_tape()
{
    controller->saveTape(
        QFileDialog::getSaveFileName(this, tr("Save file"), "", "*.posttape"));
    loadDataFromModel(*controller->getModel());
}

void PostMachineView::load_tape()
{
    controller->loadTape(
        QFileDialog::getOpenFileName(this, tr("Open file"), "", "*.posttape"));
    loadDataFromModel(*controller->getModel());
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
         "https://github.com/adidvar/post-machine"));
}

void PostMachineView::instruction()
{
  QDesktopServices::openUrl(
      QUrl("https://github.com/adidvar/post-machine/blob/master/GUIDE.md"));
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

void PostMachineView::on_actionFeedback_triggered() {
  QDesktopServices::openUrl(
      QUrl("https://github.com/adidvar/post-machine/issues"));
}

void PostMachineView::on_actionLanguage_triggered() {
  QStringList localizations = {tr("default")};
  for (qsizetype i = 0; i < locations_dirs.size(); ++i) {
    QDir dir(locations_dirs[i]);

    if (!dir.exists()) continue;

    QDirIterator it(dir.path(), {"postmachine.*.qm"}, QDir::Files);
    while (it.hasNext()) {
      localizations.push_back(it.next());
    }
  }

  auto language = QInputDialog::getItem(nullptr, tr("Choose your language"),
                                        tr("Language:"), localizations, 0);
  if (language.isEmpty()) return;

  translator.reset(new QTranslator());
  if (language != tr("default") && translator->load(language)) {
    QApplication::removeTranslator(translator.data());
    settings.setValue("language", language);
    QApplication::installTranslator(translator.data());
  }
}

void PostMachineView::changeEvent(QEvent *event) {
  static auto is_ready = true;

  if (event->type() == QEvent::LanguageChange && is_ready) {
    ui->retranslateUi(this);
    this->show();
  }
}

void PostMachineView::on_reset_button_clicked() {
  controller->reset();
  loadDataFromModel(*controller->getModel());
}
