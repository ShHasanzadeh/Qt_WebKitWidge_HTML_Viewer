#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWebView;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
      ~MainWindow();  // Add this declaration

private slots:
    void loadReport();

private:
    QWebView *webView;
    QPushButton *showButton;
};

#endif // MAINWINDOW_H


