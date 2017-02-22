#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QColor>
#include <QDebug>
#include <math.h>
#include <QMessageBox>
#include <minmaxit.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage img;
    double prevX;
    double prevY;
    double x;
    double y;
    int livZoom;
    int maxIt;
    int dimX;
    int dimY;

    double map(double, double, double,double, double);
    minmaxIt trovaMinMaxIt(double, double);
    void update();
    void checkRestraint();
    bool isInCenter(double, double, double);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_horizontalSlider_sliderPressed();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
