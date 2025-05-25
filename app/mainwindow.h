#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QtCharts>
#include <QPropertyAnimation>
#include "../SegmentFunction.hpp"
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void defineSegment();
    void calculateAt();
    void checkMonotonic();
    void checkContinuous();
    void clear();

    void updatePlot();
    void updateInfo();
    void addLogMessage(const QString& message);
private:
    Ui::MainWindow *ui;
    SegmentFunction<double> *segmentFunction;

    QChart *chart;
    QLineSeries *series;
    void setupChart();
};

#endif // MAINWINDOW_H
