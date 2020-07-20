#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QDir"
#include "QTextStream"
#include "QFile"
#include "QMessageBox"
#include "QFontDialog"
#include "QFont"
#include "QColorDialog"
#include "QColor"
#include "QPrintDialog"
#include "QPrinter"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    filename = "";
    isSaved = true;
    this->setCentralWidget(ui->textEdit); // spaces from side are gone
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionNew_triggered()
{
    if(!isSaved){
        int ret = QMessageBox::question(this,"Unsaved Changes","Your changes will be lost if you don't save them! Do you want to open a new file?",
                                        QMessageBox::Cancel,
                                        QMessageBox::Save,
                                        QMessageBox::Yes);
        if(ret == QMessageBox::Cancel) return;
        else if(ret == QMessageBox::Yes){
            filename = "";
            ui->textEdit->clear();
        }
        else{
            if(!filename.length()) MainWindow::on_actionSave_As_triggered();
            else MainWindow::on_actionSave_triggered();

            filename = "";
            ui->textEdit->clear();
        }
    }else{
        filename = "";
        ui->textEdit->clear();
    }
}

void MainWindow::on_actionOpen_triggered()
{
   QString filter = "All Files (*.*) ;; Text File (*.txt) ;; XML File (*.xml)";
   filename = QFileDialog::getOpenFileName(this, "Open a file",QDir::homePath(),filter); // static function
   QFile file(filename);

   // open the file
   if(!file.open(QFile::ReadOnly | QFile::Text)){
       // if the file didn't open, then display the error
       QMessageBox::warning(this,"title","File not open");
   }

   QTextStream in(&file);
   QString text = in.readAll();
   ui->textEdit->setPlainText(text);
   file.close();
}

void MainWindow::save_file(){
    QFile file(filename);

    // open the file
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        // if the file didn't open, then display the error
        QMessageBox::warning(this,"Error","Error saving the file!");
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.flush();
    file.close();

    isSaved = true;
}

void MainWindow::on_actionSave_triggered()
{
    if(!filename.length()){
        MainWindow::on_actionSave_As_triggered();
        return;
    }
    MainWindow::save_file();

}

void MainWindow::on_actionSave_As_triggered()
{
    filename = QFileDialog::getSaveFileName(this, "Create a new file",QDir::homePath());
    MainWindow::save_file();
}

void MainWindow::on_actionExit_triggered()
{
    if(!isSaved){
        int ret = QMessageBox::question(this,"Unsaved Changes","Your changes will be lost if you don't save them! Do you want to exit?",
                                        QMessageBox::Cancel,
                                        QMessageBox::Save,
                                        QMessageBox::Yes);
        if(ret == QMessageBox::Cancel) return;
        else if(ret == QMessageBox::Yes) QApplication::exit();
        else{
            if(!filename.length()) MainWindow::on_actionSave_As_triggered();
            else MainWindow::on_actionSave_triggered();
            QApplication::exit();
        }
    }else{
        QApplication::exit();
    }
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionAbout_Notepad_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_textEdit_textChanged()
{
    isSaved = false;
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,this);
    if(ok) ui->textEdit->setFont(font);
    else return;
}

void MainWindow::on_actionColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black,this,"Pallet");
    if(color.isValid()) ui->textEdit->setTextColor(color);
    else return;
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("");
    QPrintDialog dialog(&printer, this);
    if(dialog.exec()==QDialog::Rejected) return; // printer does not exist
    ui->textEdit->print((&printer));
}
