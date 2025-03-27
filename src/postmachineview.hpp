#ifndef POSTMACHINEVIEW_HPP
#define POSTMACHINEVIEW_HPP

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QTimer>

#include "tapewidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class PostMachineView; }
QT_END_NAMESPACE

class PostMachineModel;
class PostMachineController;

class PostMachineView : public QMainWindow
{
    Q_OBJECT

public:
    PostMachineView(PostMachineController *controller, QWidget *parent = nullptr);
    ~PostMachineView();

    void loadDataFromModel(const PostMachineModel &model);

private slots:
    void on_scroll_left_button_clicked();
    void on_scroll_right_button_clicked();

    void on_commands_widget_itemChanged(QTableWidgetItem *item);

    void on_insert_button_clicked();
    void on_remove_button_clicked();

    void tape_value_changed(Tape::index_t);

    void on_run_button_clicked();
    void on_stop_button_clicked();
    void on_step_button_clicked();
    void on_reset_button_clicked();

    void save_tape();
    void load_tape();

    void new_file();
    void load_file();
    void save_file();

    void exit();
    void about();
    void instruction();

    void timer();

    void on_fast_speed_action_triggered();
    void on_normal_speed_action_triggered();
    void on_slow_speed_action_triggered();
    void on_custom_speed_action_triggered();

    void on_problem_edit_textChanged();

   private:
    Ui::PostMachineView *ui;

    PostMachineController *controller;

    QTimer *m_timer;

    ///< flag for locking event while data loading to view
    bool m_editing = false;
};
#endif // POSTMACHINEVIEW_HPP
