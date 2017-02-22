#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->verticalSlider->setValue(ui->verticalSlider->maximum()/2);
    ui->horizontalSlider->setValue(ui->horizontalSlider->maximum()/2);

    x=0.0;
    y=0.0;
    prevX=x;
    prevY=y;
    livZoom=0;
    maxIt=100;
    dimX=1366;
    dimY=768;
    img=QImage(dimX,dimY,QImage::Format_Grayscale8);
    update();
    ui->lineEdit->setText(QString::number(x,'g',15));
    ui->lineEdit_2->setText(QString::number(y,'g',15));
    ui->lineEdit_3->setText(QString::number(livZoom));
}

MainWindow::~MainWindow(){delete ui;}

//MAP
double MainWindow::map(double valore, double da, double a,double obda,double oba){
    if(da<a){
        if(valore>=da && valore<=a){
            int range=a-da;
            double perc=(valore-da)/range;
            return obda+((oba-obda)*perc);
        }
        else
            return 0;
    }
    else if(da>a){
        if(valore<=da && valore>=a){
            int range=da-a;
            double perc=(valore-a)/range;
            return obda+((oba-obda)*perc);
        }
        else
            return 0;
    }
    else return 0;

}

//CONTROLLA COORD
void MainWindow::checkRestraint(){
    if(x<-2.0)
        x=-2.0;
    if(x>2.0)
        x=2.0;
    if(y<-1.12445)
        y=-1.12445;
    if(y>1.12445)
        y=1.12445;
}

bool MainWindow::isInCenter(double val, double min, double max){
    return val-min == max-val;
}

//TROVA MAX ITERAZIONI
minmaxIt MainWindow::trovaMinMaxIt(double minX, double minY) {
    minmaxIt temp;
    int inf=16;
    for(int i=0;i<dimX;i++){
        for(int j=0;j<dimY;j++){
            double corda=map(i,0,dimX,minX,minX+4.0/(pow(2,livZoom)));
            double cordb=map(j,0,dimY,minY+2.2489/(pow(2,livZoom)),minY);
            double newa=corda;
            double newb=cordb;
            double nnewa=0.00;
            double nnewb=0.00;
            bool diverge=false;
            int k=0;
            for(;k<maxIt && !diverge;k++){
                nnewa=(newa*newa)-(newb*newb);
                nnewb=(2.00*newa*newb);
                newa=nnewa+corda;
                newb=nnewb+cordb;
                if(newa>inf || newa<-inf || newb>inf || newb<-inf){
                    diverge=true;
                    if(k>temp.max)
                        temp.max=k;
                    if(k<temp.min);
                        temp.min=k;
                }
            }
        }
    }
    return temp;
}

//UPDATE
void MainWindow::update(){
    double minX=0.00;
    double minY=0.00;
    if(livZoom==0)
        minX=-2;
    else if(x+(2.0/pow(2.0,livZoom))>2.00)
        minX=2.00-(2.00/pow(2.0,livZoom));
    else if(x-(2.0/pow(2.0,livZoom))<-2.00)
        minX=-2;
        else minX=x-(2.00/pow(2.0,livZoom));
    if(livZoom==0)
        minY=-1.12445;
    else if(y+(1.12445/pow(2.0,livZoom))>1.2445)
        minY=1.12445-(1.12445/pow(2.0,livZoom));
    else if(y+(1.12445/pow(2.0,livZoom))>1.2445)
        minY=-1.12445;
        else minY=y-1.12445/pow(2.0,livZoom);
    double maxX=minX+4.0/(pow(2,livZoom));
    double maxY=minY+2.2489/(pow(2,livZoom));
    minmaxIt temp=trovaMinMaxIt(minX,minY);
    int MIt=temp.max;
    int mIt=temp.min;
    int inf=16;

    for(int i=0;i<dimX;i++){
        for(int j=0;j<dimY;j++){
            double corda=map(i,0,dimX,minX,maxX);
            double cordb=map(j,0,dimY,minY,maxY);
            double newa=corda;
            double newb=cordb;
            double nnewa=0.00;
            double nnewb=0.00;
            bool diverge=false;
            int k=0;
            for(;k<maxIt && !diverge;++k){
                nnewa=(newa*newa)-(newb*newb);
                nnewb=(2.00*newa*newb);
                newa=nnewa+corda;
                newb=nnewb+cordb;
                if(newa>inf || newa<-inf || newb>inf || newb<-inf)
                    diverge=true;
            }
            int col=255;
            if(diverge)
                col=map(k,mIt,MIt,0,255);
            img.setPixelColor(i,(dimY-1)-j,QColor(col,col,col));
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(img));
    ui->label->show();
}


//ZOOM IN
void MainWindow::on_pushButton_clicked()
{
    double minX=0.00;
    double minY=0.00;
    if(livZoom==0)
        minX=-2;
    else if(x+(2.0/pow(2.0,livZoom))>2.00)
        minX=2.00-(2.00/pow(2.0,livZoom));
    else if(x-(2.0/pow(2.0,livZoom))<-2.00)
        minX=-2;
        else minX=x-(2.00/pow(2.0,livZoom));
    if(livZoom==0)
        minY=-1.12445;
    else if(y+(1.12445/pow(2.0,livZoom))>1.2445)
        minY=1.12445-(1.12445/pow(2.0,livZoom));
    else if(y+(1.12445/pow(2.0,livZoom))>1.2445)
        minY=-1.12445;
        else minY=y-1.12445/pow(2.0,livZoom);
    double maxX=minX+4.0/(pow(2,livZoom));
    double maxY=minY+2.2489/(pow(2,livZoom));

    prevX=x;
    x=map(ui->horizontalSlider->value(),0,ui->horizontalSlider->maximum(),minX,maxX);
    prevY=y;
    y=map(ui->verticalSlider->value(),0,ui->verticalSlider->maximum(),minY,maxY);
    checkRestraint();

    ui->verticalSlider->setValue(ui->verticalSlider->maximum()/2);
    ui->horizontalSlider->setValue(ui->horizontalSlider->maximum()/2);


    livZoom++;
    update();
    ui->lineEdit->setText(QString::number(x,'g',15));
    ui->lineEdit_2->setText(QString::number(y,'g',15));
    ui->lineEdit_3->setText(QString::number(livZoom));
}

//ZOOM OUT
void MainWindow::on_pushButton_2_clicked()
{
    if(livZoom>0){

        livZoom--;
        x=prevX;
        y=prevY;
        update();
        ui->lineEdit->setText(QString::number(x,'g',15));
        ui->lineEdit_2->setText(QString::number(y,'g',15));
        ui->lineEdit_3->setText(QString::number(livZoom));
    }
    else{
        x=0.0;
        y=0.0;
        ui->lineEdit->setText(QString::number(x,'g',15));
        ui->lineEdit_2->setText(QString::number(y,'g',15));
        QMessageBox::critical(this,"Errore Zoom","Il Livello di Zoom e' gia' al Minimo");
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,"Reset","Sei sicuro di voler Resettare il livello di Zoom?",QMessageBox::Yes | QMessageBox::No);
    if(reply==QMessageBox::Yes){
        livZoom=0;
        x=0.0;
        y=0.0;
        update();
        ui->lineEdit->setText(QString::number(x,'g',15));
        ui->lineEdit_2->setText(QString::number(y,'g',15));
        ui->lineEdit_3->setText(QString::number(livZoom));
    }
}

void MainWindow::on_horizontalSlider_sliderPressed()
{
    QLine* line= new QLine(15+ui->horizontalSlider->value(),15+ui->verticalSlider->value(),15+ui->horizontalSlider->value(),15+768);
}
