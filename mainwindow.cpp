#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDoubleValidator>
#include <QDebug>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , Vinitial(0), Vfinal(0), Acceleration(0), Height(0)
    , formula("")
{
    ui->setupUi(this);
    this->setWindowTitle("Final Project - Madeline Annabel");

    ui->Vinitial->setText("0");
    ui->Vfinal->setText("0");
    ui->Acceleration->setText("0");
    ui->Height->setText("0");
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getInput()
{
    bool ok;

    // Replace ',' with '.' and parse as double
    Vinitial = ui->Vinitial->text().replace(",", ".").toDouble(&ok);
    if (!ok) {
        qDebug() << "Invalid input for Vinitial";
        Vinitial = 0; // Default if invalid
        }

        Vfinal = ui->Vfinal->text().replace(",", ".").toDouble(&ok);
        if (!ok) {
            qDebug() << "Invalid input for Vfinal";
            Vfinal = 0;
        }

        Acceleration = ui->Acceleration->text().replace(",", ".").toDouble(&ok);
        if (!ok) {
            qDebug() << "Invalid input for Acceleration";
            Acceleration = 0;
        }

        Height = ui->Height->text().replace(",", ".").toDouble(&ok);
        if (!ok) {
            qDebug() << "Invalid input for Height";
            Height = 0;
        }

        qDebug() << "Vinitial:" << Vinitial << "Vfinal:" << Vfinal
                 << "Acceleration:" << Acceleration << "Height:" << Height;

}

double MainWindow::Calculate()
{
    double EstimatedTime = -1;

    if (Vinitial != 0 && Acceleration != 0 && Height > 0) {
        // Use the formula: H = V₀t + 0.5at²
        double a = 0.5 * Acceleration;
        double b = Vinitial;
        double c = -Height;

        if (a != 0) { // Avoid division by zero when Acceleration is very small or zero
            double discriminant = b * b - 4 * a * c; // Calculate discriminant
            if (discriminant >= 0) {
                // Quadratic formula: t = (-b ± sqrt(b² - 4ac)) / (2a)
                double t1 = (-b + sqrt(discriminant)) / (2 * a);
                double t2 = (-b - sqrt(discriminant)) / (2 * a);

                // Select the positive root as the valid time
                if (t1 > 0 && t2 > 0) {
                    EstimatedTime = std::min(t1, t2); // Choose the smaller positive time
                } else if (t1 > 0) {
                    EstimatedTime = t1;
                } else if (t2 > 0) {
                    EstimatedTime = t2;
                }
            }
        } else {
            // If a == 0, the equation reduces to a linear form: H = V₀t
            EstimatedTime = Height / Vinitial;
        }
    } else if (Vinitial != 0 && Acceleration != 0 && Vfinal != 0) {
        // Use the formula: V = V₀ + at
        EstimatedTime = (Vfinal - Vinitial) / Acceleration;
    }

    return EstimatedTime;
}


void MainWindow::Display(double EstimatedTime)
{
    if (Vinitial != 0 && Acceleration != 0 && Height > 0) {
        formula = "H = V₀t + 0.5at²";
    } else if (Vinitial != 0 && Acceleration != 0 && Vfinal != 0) {
        formula = "V = V₀ + at";
    } else {
        formula = "Invalid information to determine a formula.";
    }

    ui->textBrowser->setText("With the given information, we can use: \n" + formula);

    if (EstimatedTime >= 0) {
        ui->textBrowser->append("The estimated time for the object to touch the ground is: "
                                + QString::number(EstimatedTime, 'f', 2) + " seconds.");
    } else {
        ui->textBrowser->append("Unable to calculate the estimated time due to invalid data.");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("The Instruction to Calculate Estimated Time (numbers only).");
    msgBox.setInformativeText("Input the given informations \n"
                              "Use '.' for decimal. \n"
                              "Press the calculate button. \n"
                              "The text browser will display the time it takes for the object to free fall.");
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Close);
    int ret = msgBox.exec();
}

void MainWindow::on_pushButton_clicked()
{
    getInput();
    double EstimatedTime = Calculate();
    Display(EstimatedTime);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->Vinitial->setText("0");
    ui->Vfinal->setText("0");
    ui->Acceleration->setText("0");
    ui->Height->setText("0");
    ui->textBrowser->clear();
}

