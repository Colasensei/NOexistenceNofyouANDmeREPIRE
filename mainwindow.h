#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <filesystem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::MainWindow *ui;
    void appendLog(const QString& message);
    void repairOperation();

    // 文件操作函数 (保持原逻辑)
    std::filesystem::path GetToolDirectory();
    std::filesystem::path GetGameSaveDirectory();
    bool ReplaceFolder(const std::filesystem::path& source, const std::filesystem::path& target);
};

#endif // MAINWINDOW_H
