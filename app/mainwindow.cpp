#include "mainwindow.h"
#include "./ui_mainwindow.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    segmentFunction(new SegmentFunction<double>()),
    chart(new QChart()),
    series(new QLineSeries())
{
    ui->setupUi(this);
    setStyleSheet(
        "QMainWindow {"
        "   background: #f8fafc;"
        "   font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QGroupBox {"
        "   border: 1px solid #e2e8f0;"
        "   border-radius: 10px;"
        "   margin-top: 15px;"
        "   padding-top: 15px;"
        "   background: white;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 4px;"
        "   color: #a44040;"
        "   font-weight: 600;"
        "   font-size: 14px;"
        "}"
        "QPushButton {"
        "   background: white;"
        "   color: #a44040;"
        "   border: 1px solid #e2e8f0;"
        "   padding: 4px 8px;"
        "   border-radius: 6px;"
        "   font-weight: 500;"
        "   transition: all 0.2s ease;"
        "}"
        "QPushButton:hover {"
        "   background: #a44040;"
        "   color: white;"
        "   border-color: #a44040;"
        "}"
        "QLineEdit, QTextEdit, QComboBox, QSpinBox, QDoubleSpinBox {"
        "   padding: 4px 6px;"
        "   border: 1px solid #e2e8f0;"
        "   border-radius: 6px;"
        "   background: white;"
        "   selection-background-color: #a44040;"
        "   selection-color: white;"
        "}"
        "QLineEdit:focus, QTextEdit:focus, QComboBox:focus {"
        "   border-color: #a44040;"
        "   outline: none;"
        "}"
        "QComboBox::drop-down {"
        "   border: none;"
        "   width: 24px;"
        "}"
        "QScrollBar:vertical {"
        "   border: none;"
        "   background: #f1f5f9;"
        "   width: 10px;"
        "   margin: 0;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #cbd5e1;"
        "   min-height: 20px;"
        "   border-radius: 5px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "   border: none;"
        "   background: none;"
        "   height: 0;"
        "}"
        "QTabWidget::pane {"
        "   border: 1px solid #e2e8f0;"
        "   border-radius: 6px;"
        "   padding: 6px;"
        "   margin-top: -1px;"
        "}"
        "QTabBar::tab {"
        "   padding: 8px 16px;"
        "   border: none;"
        "   color: #64748b;"
        "}"
        "QTabBar::tab:selected {"
        "   color: #a44040;"
        "   border-bottom: 2px solid #a44040;"
        "}"
        "QMessageBox, QInputDialog, QDialog {"
        "background: #f8fafc;"
        "font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QMessageBox QLabel {"
        "   color: #334155;"
        "}"
    );
    setupChart();
    updateInfo();

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::defineSegment);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::calculateAt);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::checkMonotonic);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::checkContinuous);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::clear);
}

MainWindow::~MainWindow() {
    delete segmentFunction;
    delete series;
    delete chart;
    delete ui;
}

void MainWindow::setupChart() {
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("График кусочной функции");
    chart->legend()->hide();

    chart->axisX()->setTitleText("x");
    chart->axisY()->setTitleText("f(x)");
    chart->axisX()->setRange(-10, 10);
    chart->axisY()->setRange(-10, 10);

    chart->setTitleFont(QFont("Segoe UI", 12, QFont::Bold));
    chart->setTitleBrush(QBrush(QColor("#1a202c")));
    chart->setBackgroundBrush(QBrush(QColor("#ffffff")));
    chart->setBackgroundRoundness(10);
    chart->setDropShadowEnabled(false);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->setStyleSheet(
        "QChartView {"
        "   margin-top: 15;"
        "   border: 1px solid #e2e8f0;"
        "   border-radius: 10px;"
        "   background: white;"
        "   padding: 10px;"
        "}"
    );
    QLayoutItem *oldItem = ui->gridLayout_6->takeAt(0);
    if (oldItem) delete oldItem->widget();
    delete oldItem;
    ui->gridLayout_6->addWidget(chartView);
    chartView->setContentsMargins(5, 5, 5, 5);
    chart->setAnimationOptions(QChart::AllAnimations);
}

void MainWindow::updatePlot() {
    chart->removeAllSeries();
    double step = 0.01, minX = -10.0, maxX = 10.0, minY = -10.0, maxY = 10.0;
    series = new QLineSeries();
    bool flag = false;
    for (int i = 0; i < segmentFunction->GetSize(); i++) {
        for (double x = segmentFunction->Get(i).start; x < segmentFunction->Get(i).end; x+=step) {
            try {
                double y = segmentFunction->CalculateAt(x);
                if (isfinite(y)) {
                    series->append(x, y);
                    if (flag) {
                        if (y < minY) minY = y;
                        if (y > maxY) maxY = y;
                    } else {
                        minY = maxY = y;
                        flag = true;
                    }
                } else {
                    if (series->count() > 0) {
                        chart->addSeries(series);
                        series = new QLineSeries();
                    }
                }
            } catch (...) {
                if (series->count() > 0) {
                    chart->addSeries(series);
                    series = new QLineSeries();
                }
            }
        }
        chart->addSeries(series);
        series = new QLineSeries();
    }
    delete series;
    QList<QAbstractSeries*> seriesList = chart->series();
    for (QAbstractSeries* s : seriesList) {
        QLineSeries* ls = qobject_cast<QLineSeries*>(s);
        if (ls) {
            ls->attachAxis(chart->axisX());
            ls->attachAxis(chart->axisY());
        }
    }
    if (flag) {
        double yPadding = (maxY - minY) * 0.1;
        minY -= yPadding;
        maxY += yPadding;
    }
    chart->axisX()->setRange(minX, maxX);
    chart->axisY()->setRange(minY, maxY);
    chart->update();
}

void MainWindow::updateInfo() {
    ui->label_6->setText("Количество сегментов: " + QString::number(segmentFunction->GetSize()));
}

void MainWindow::addLogMessage(const QString& message) {
    QString time = QDateTime::currentDateTime().toString("[hh:mm:ss]");
    ui->textEdit->append(time + " " + message);
}

void MainWindow::clear() {
    segmentFunction->Clear();
    updatePlot();
    updateInfo();
    addLogMessage("Функция очищена");
}

void MainWindow::defineSegment() {
    try {
        double start = ui->doubleSpinBox->value();
        double end = ui->doubleSpinBox_2->value();
        if (start >= end) {
            QMessageBox::warning(this, QString::fromStdString("Ошибка"), QString::fromStdString("Неправильные аргументы!"));
        } else {
            QString functionType = ui->comboBox->currentText();
            function<double(double)> func;
            if (functionType == "Константная f(x)=c") {
                bool ok;
                double c = QInputDialog::getDouble(this, "Коэффициенты", "Введите значение константы c:", 0.0, -1e9, 1e9, 2, &ok);
                if (ok) {
                    func = [c](double){return c;};
                    segmentFunction->Define(start, end, func);
                    updatePlot();
                    updateInfo();
                    addLogMessage(QString("Добавлен сегмент [%1, %2] с функцией: %3").arg(start).arg(end).arg(functionType));
                }
            } else if (functionType == "Линейная f(x)=ax+b") {
                bool ok1, ok2;
                double a = QInputDialog::getDouble(this, "Коэффициенты", "Введите коэффициент a:", 1.0, -1e6, 1e6, 2, &ok1);
                double b = QInputDialog::getDouble(this, "Коэффициенты", "Введите коэффициент b:", 0.0, -1e6, 1e6, 2, &ok2);
                if (ok1 && ok2) {
                    func = [a, b](double x){return a*x+b;};
                    segmentFunction->Define(start, end, func);
                    updatePlot();
                    updateInfo();
                    addLogMessage(QString("Добавлен сегмент [%1, %2] с функцией: %3").arg(start).arg(end).arg(functionType));
                }
            } else if (functionType == "Квадратичная f(x)=ax²+bx+c") {
                bool ok1, ok2, ok3;
                double a = QInputDialog::getDouble(this, "Коэффициенты", "Введите коэффициент a:", 1.0, -1e6, 1e6, 2, &ok1);
                double b = QInputDialog::getDouble(this, "Коэффициенты", "Введите коэффициент b:", 0.0, -1e6, 1e6, 2, &ok2);
                double c = QInputDialog::getDouble(this, "Коэффициенты", "Введите коэффициент c:", 0.0, -1e6, 1e6, 2, &ok3);
                if (ok1 && ok2 && ok3) {
                    func = [a, b, c](double x){return a*x*x+b*x+c;};
                    segmentFunction->Define(start, end, func);
                    updatePlot();
                    updateInfo();
                    addLogMessage(QString("Добавлен сегмент [%1, %2] с функцией: %3").arg(start).arg(end).arg(functionType));
                }
            } else if (functionType == "Гиперболическая f(x)=ᵏ⁄ₓ₊ₐ+b") {
                bool ok1, ok2, ok3;
                double k = QInputDialog::getDouble(this, "Коэффициенты", "Введите коэффициент k:", 1.0, -1e6, 1e6, 2, &ok1);
                double a = QInputDialog::getDouble(this, "Коэффициенты", "Введите коэффициент a:", 1.0, -1e6, 1e6, 2, &ok2);
                double b = QInputDialog::getDouble(this, "Коэффициенты", "Введите коэффициент b:", 0.0, -1e6, 1e6, 2, &ok3);
                if (ok1 && ok2 && ok3) {
                    func = [k, a, b](double x){return k/(x+a)+b;};
                    segmentFunction->Define(start, end, func);
                    updatePlot();
                    updateInfo();
                    addLogMessage(QString("Добавлен сегмент [%1, %2] с функцией: %3").arg(start).arg(end).arg(functionType));
                }
            } else if (functionType == "Степенная f(x)=xⁿ") {
                bool ok;
                double n = QInputDialog::getDouble(this, "Коэффициенты", "Введите коэффициент n:", 1.0, -1e6, 1e6, 2, &ok);
                if (ok) {
                    func = [n](double x){return pow(x, n);};
                    segmentFunction->Define(start, end, func);
                    updatePlot();
                    updateInfo();
                    addLogMessage(QString("Добавлен сегмент [%1, %2] с функцией: %3").arg(start).arg(end).arg(functionType));
                }
            } else if (functionType == "Синусоида f(x)=asin(bx+c)+d") {
                bool ok1, ok2, ok3, ok4;
                double a = QInputDialog::getDouble(this, "Коэффициенты", "Введите амплитуду a:", 1.0, -1e6, 1e6, 2, &ok1);
                double b = QInputDialog::getDouble(this, "Коэффициенты", "Введите частоту b:", 1.0, -100, 100, 2, &ok2);
                double c = QInputDialog::getDouble(this, "Коэффициенты", "Введите фазу c:", 0.0, -6.28, 6.28, 2, &ok3);
                double d = QInputDialog::getDouble(this, "Коэффициенты", "Введите смещение d:", 0.0, -1e6, 1e6, 2, &ok4);
                if (ok1 && ok2 && ok3 && ok4) {
                    func = [a, b, c, d](double x){return a*sin(b*x+c)+d;};
                    segmentFunction->Define(start, end, func);
                    updatePlot();
                    updateInfo();
                    addLogMessage(QString("Добавлен сегмент [%1, %2] с функцией: %3").arg(start).arg(end).arg(functionType));
                }
            }
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::calculateAt() {
    try {
        double x = ui->doubleSpinBox_3->value();
        double result = segmentFunction->CalculateAt(x);
        QString message = QString("f(%1) = %2").arg(x).arg(result);
        addLogMessage(message);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::checkMonotonic() {
    bool isMonotonic = segmentFunction->IsMonotonic();
    QString message = isMonotonic ? "Функция монотонна" : "Функция не монотонна";
    QMessageBox::information(this, "Результат", message);
    addLogMessage("Проверка монотонности: " + message);
}

void MainWindow::checkContinuous() {
    bool isContinuous = segmentFunction->IsContinuous();
    QString message = isContinuous ? "Функция непрерывна" : "Функция не непрерывна";
    QMessageBox::information(this, "Результат", message);
    addLogMessage("Проверка непрерывности: " + message);
}
