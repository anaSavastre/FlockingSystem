#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NGLScene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *m_ui;
    //the NGLScene widget
    NGLScene *m_gl;

    ///number of boids
     int m_numBoids=1000;
     ///radius
     float m_rad;


private slots :
    void resetBoidsButton();

    void updateNumberBoids();

    void resetSize();

    void resetBehavior();

    void resetVelocity();

};

#endif // MAINWINDOW_H
