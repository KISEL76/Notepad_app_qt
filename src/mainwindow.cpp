#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget::setWindowTitle("Безымянный – Блокнот");
    title = QWidget::windowTitle();
    path = "Безымянный";
    size = 0;
    scale = 100;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_create_triggered()
{
    if(title.contains("*"))
    {
        QMessageBox::StandardButton reply;
        QString question = "Вы хотите сохранить изменения в файле?";


        reply = QMessageBox::question(this,"Блокнот", question,
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            on_action_save_triggered();
            if(path == "Безымянный")
            {
                return;
            }
            else
            {
                ui->plainTextEdit->clear();
                QWidget::setWindowTitle("Безымянный – Блокнот");
                title = "Безымянный – Блокнот";
                path = "Безымянный";
                edited = false;
                saved = false;
            }
        }
        else if(reply == QMessageBox::No)
        {
            ui->plainTextEdit->clear();
            QWidget::setWindowTitle("Безымянный – Блокнот");
            title = "Безымянный – Блокнот";
            path = "Безымянный";
            edited = false;
            saved = false;
        }
        else if(reply == QMessageBox::Cancel)
        {
            return;
        }
    }
    else
    {
        ui->plainTextEdit->clear();
        QWidget::setWindowTitle("Безымянный – Блокнот");
        title = "Безымянный – Блокнот";
        path = "Безымянный";
        edited = false;
        saved = false;
    }
}

void MainWindow::on_action_open_triggered()
{
    if(title.contains("*"))
    {
        QMessageBox::StandardButton reply;
        QString question = "Вы хотите сохранить изменения в файле?";

        reply = QMessageBox::question(this,"Блокнот", question,
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            //part 1: save
            on_action_save_triggered();
            if(path == "Безымянный")
            {
                return;
            }
            else
            {
                //part 2: open
                QString new_path = QFileDialog::getOpenFileName(this,"Открытие",
                                                    QString(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)),
                                                    "Текстовые документы (*.txt)");

                if(!new_path.isEmpty())
                {
                    QFile file_new(new_path);
                    file_new.open(QIODevice::ReadOnly | QIODevice::Text);
                    file_title = get_file_title(new_path);

                    if(file_new.isOpen())
                    {
                        QByteArray data;
                        data = file_new.readAll();
                        ui->plainTextEdit->setPlainText(QString(data));

                        QWidget::setWindowTitle(file_title + " – Блокнот");
                        path = new_path;
                        title = file_title + " – Блокнот";
                        edited = false;
                        saved = true;
                    }
                    else
                    {
                        QMessageBox::information(this,"Блокнот","Ошибка открытия файла");
                        file_title = get_file_title(path);
                        return;
                    }
                }
                else
                {
                    QMessageBox::information(this,"Блокнот","Ошибка открытия файла");
                    file_title = get_file_title(path);
                    return;
                }
            }
        }
        else if(reply == QMessageBox::No)
        {
            QString new_path = QFileDialog::getOpenFileName(this,"Открытие",
                                                QString(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)),
                                                "Текстовые документы (*.txt)");

            if(!new_path.isEmpty())
            {
                QFile file(new_path);
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                file_title = get_file_title(new_path);

                if(file.isOpen())
                {
                    QByteArray data;
                    data = file.readAll();
                    ui->plainTextEdit->setPlainText(QString(data));

                    QWidget::setWindowTitle(file_title + " – Блокнот");
                    path = new_path;
                    title = file_title + " – Блокнот";
                    edited = false;
                    saved = true;
                }
                else
                {
                    QMessageBox::information(this,"Блокнот","Ошибка открытия файла");
                    file_title = get_file_title(path);
                    return;
                }
            }
            else
            {
                QMessageBox::information(this,"Блокнот","Ошибка открытия файла");
                file_title = get_file_title(path);
                return;
            }
        }
        else if(reply == QMessageBox::Cancel)
        {
            return;
        }
    }
    else
    {
        QString new_path = QFileDialog::getOpenFileName(this,"Открытие",
                                            QString(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)),
                                            "Текстовые документы (*.txt)");

        if(!new_path.isEmpty())
        {
            QFile file(new_path);
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            file_title = get_file_title(new_path);

            if(file.isOpen())
            {
                QByteArray data;
                data = file.readAll();
                ui->plainTextEdit->setPlainText(QString(data));

                QWidget::setWindowTitle(file_title + " – Блокнот");
                title = file_title + " – Блокнот";
                path = new_path;
                edited = false;
                saved = true;
            }
            else
            {
                QMessageBox::information(this,"Блокнот","Ошибка открытия файла");
                file_title = get_file_title(path);
                return;
            }
        }
        else
        {
            QMessageBox::information(this,"Блокнот","Ошибка открытия файла");
            file_title = get_file_title(path);
            return;
        }
    }
}

void MainWindow::on_action_save_triggered()
{
    if(saved)
    {
        QString new_path = path;
        if(new_path == "Безымянный")
        {
            return;
        }

        if(!new_path.isEmpty())
        {
            QFile file(new_path);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file_title = get_file_title(new_path);

            if(file.isOpen())
            {
                issue = false;
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                stream << ui->plainTextEdit->toPlainText();
                file.close();
                saved = true;
                QWidget::setWindowTitle(file_title + " – Блокнот");
                path = new_path;
                title = file_title + " – Блокнот";
                edited = false;
            }
            else
            {
                issue = true;
                QMessageBox::information(this,"Блокнот","Ошибка сохранения файла");
                file_title = get_file_title(path);
                return;
            }
        }
        else
        {
            issue = true;
            QMessageBox::information(this,"Блокнот","Ошибка сохранения файла");
            file_title = get_file_title(path);
            return;
        }
    }
    else
    {
        QString new_path = QFileDialog::getSaveFileName(this,"Сохранение",
                                            QString(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)),
                                            "Текстовые документы (*.txt)");

        if(!new_path.isEmpty())
        {
            QFile file(new_path);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file_title = get_file_title(new_path);

            if(file.isOpen())
            {
                issue = false;
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                stream << ui->plainTextEdit->toPlainText();
                file.close();
                saved = true;
                QWidget::setWindowTitle(file_title + " – Блокнот");
                path = new_path;
                title = file_title + " – Блокнот";
                edited = false;
            }
            else
            {
                issue = true;
                QMessageBox::information(this,"Блокнот","Ошибка сохранения файла");
                file_title = get_file_title(path);
                return;
            }
        }
        else
        {
            issue = true;
            QMessageBox::information(this,"Блокнот","Ошибка сохранения файла");
            file_title = get_file_title(path);
            return;
        }
    }
}

void MainWindow::on_action_saveas_triggered()
{
    QString new_path = QFileDialog::getSaveFileName(this,"Сохранение",
                                        QString(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)),
                                        "Текстовые документы (*.txt)");

    if(!new_path.isEmpty())
    {
        QFile file(new_path);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file_title = get_file_title(new_path);

        if(file.isOpen())
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream << ui->plainTextEdit->toPlainText();
            file.close();
            saved = true;
            QWidget::setWindowTitle(file_title + " – Блокнот");
            path = new_path;
            title = file_title + " – Блокнот";
            edited = false;
        }
        else
        {
            QMessageBox::information(this,"Блокнот","Ошибка сохранения файла");
            file_title = get_file_title(path);
            return;
        }
    }
    else
    {
        QMessageBox::information(this,"Блокнот","Ошибка сохранения файла");
        file_title = get_file_title(path);
        return;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(title.contains("*"))
    {
        QMessageBox::StandardButton reply;
        QString question = "Вы хотите сохранить изменения в файле?";

        reply = QMessageBox::question(this,"Блокнот", question,
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            on_action_save_triggered();
            if(issue)
            {
                event->ignore();
            }
            else
            {
                event->accept();
            }
        }
        else if(reply == QMessageBox::No)
        {
            event->accept();
        }
        else if(reply == QMessageBox::Cancel)
        {
            event->ignore();
        }
    }
    else
    {
        event->accept();
    }
}

void MainWindow::on_action_quit_triggered()
{
    close();
}

void MainWindow::on_plainTextEdit_textChanged()
{
    file_text = ui->plainTextEdit->toPlainText();
    size = file_text.size();
    if(size == 0)
    {
        ui->action_find->setDisabled(true);
        ui->action_replace->setDisabled(true);
    }
    else
    {
        ui->action_find->setEnabled(true);
        ui->action_replace->setEnabled(true);
    }

    if(title == "*Безымянный – Блокнот" and size == 0)
    {
        QWidget::setWindowTitle("Безымянный – Блокнот");
        title = "Безымянный – Блокнот";
        edited = false;
    }
    else
    {
        if(!edited)
        {
            QWidget::setWindowTitle('*' + title);
            title = "*" + title;
            edited = true;
        }
    }
    ui->action_undo->setEnabled(true);
}

QString MainWindow::get_file_title(QString path)
{

    if(path.contains("/"))
    {
        int len = path.size();
        int pos = path.lastIndexOf("/");
        QString current_file_title = path.right(len - pos - 1);
        return current_file_title;
    }
    else
    {
        return "Безымянный";
    }
}



