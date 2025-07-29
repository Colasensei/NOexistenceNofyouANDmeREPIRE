#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTimer>
#include <Windows.h>
#include <shlobj.h>
#include <filesystem>

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("不/存在的你，和我存档修复工具");
    setFixedSize(600, 400);

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->startButton->setEnabled(true);
    ui->cancelButton->setEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendLog(const QString& message)
{
    ui->logText->appendPlainText(message);
    QApplication::processEvents();
}

void MainWindow::on_startButton_clicked()
{
    ui->startButton->setEnabled(false);
    repairOperation();
}

void MainWindow::on_cancelButton_clicked()
{
    close();
}

// 文件操作
fs::path MainWindow::GetToolDirectory()
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    return fs::path(path).parent_path();
}

fs::path MainWindow::GetGameSaveDirectory()
{
    char userProfile[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, userProfile))) {
        fs::path savePath = fs::path(userProfile) / "AppData" / "LocalLow" / "Nino" / "TheNOexistenceNofyouANDme";
        return savePath;
    }
    return {};
}

bool MainWindow::ReplaceFolder(const fs::path& source, const fs::path& target)
{
    try {
        appendLog("准备替换:");
        appendLog(QString("源: %1").arg(QString::fromStdString(source.string())));
        appendLog(QString("目标: %1").arg(QString::fromStdString(target.string())));

        if (fs::exists(target)) {
            fs::path backup = target;
            backup += "_backup";

            if (fs::exists(backup)) {
                fs::remove_all(backup);
            }

            fs::rename(target, backup);
            appendLog(QString("已创建备份: %1").arg(QString::fromStdString(backup.string())));
        }

        if (!target.parent_path().empty() && !fs::exists(target.parent_path())) {
            appendLog(QString("创建父目录: %1").arg(QString::fromStdString(target.parent_path().string())));
            if (!fs::create_directories(target.parent_path())) {
                appendLog("错误: 无法创建父目录");
                return false;
            }
        }

        if (fs::exists(source)) {
            appendLog("正在复制文件...");
            fs::copy(source, target, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
            appendLog("复制完成");
            return true;
        }
        else {
            appendLog("错误: 源目录不存在");
            return false;
        }
    }
    catch (const std::exception& e) {
        appendLog(QString("异常错误: %1").arg(e.what()));
        return false;
    }
}

void MainWindow::repairOperation()
{
    // 显示警告对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, "警告", "将恢复存档，是否继续？",
                                 QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        appendLog("操作已取消");
        ui->startButton->setEnabled(true);
        return;
    }

    appendLog("开始存档修复...");
    ui->progressBar->setValue(30);

    fs::path toolDir = GetToolDirectory();
    fs::path fixedSavesDir = toolDir / "FixedSaves" / "TheNOexistenceNofyouANDme";

    if (!fs::exists(fixedSavesDir)) {
        appendLog(QString("错误: 修复文件夹不存在 (%1)").arg(QString::fromStdString(fixedSavesDir.string())));
        ui->progressBar->setValue(0);
        ui->startButton->setEnabled(true);
        return;
    }

    fs::path gameSaveDir = GetGameSaveDirectory();
    if (gameSaveDir.empty()) {
        appendLog("错误: 无法找到游戏存档目录");
        ui->progressBar->setValue(0);
        ui->startButton->setEnabled(true);
        return;
    }

    appendLog(QString("将修复存档从: %1").arg(QString::fromStdString(fixedSavesDir.string())));
    appendLog(QString("替换到: %1").arg(QString::fromStdString(gameSaveDir.string())));
    ui->progressBar->setValue(60);

    if (ReplaceFolder(fixedSavesDir, gameSaveDir)) {
        appendLog("存档修复成功完成!");
        ui->progressBar->setValue(100);


    }
    else {
        appendLog("存档修复失败!");
        ui->progressBar->setValue(0);
        ui->startButton->setEnabled(true);
    }
}
