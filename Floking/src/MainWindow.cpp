#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_gl=new NGLScene(m_numBoids, this);

    //setting the values of the UI to the default scene construction
    m_ui->m_nrBoidsSB->setValue(m_numBoids);
    m_ui->m_boidSize->setValue(m_gl->getBoidRadius());
    m_rad=m_ui->m_boidSize->value();
    m_ui->m_seekRad->setValue(m_gl->getSeekCoef());
    m_ui->m_alignRad->setValue(m_gl->getAlignCoef());
    m_ui->m_sepRad->setValue(m_gl->getSepCoef());
    m_ui->m_velocity->setValue(m_gl->getVelocity());

    m_ui->s_mainWindowGridLayout->addWidget(m_gl, 0, 0, 3, 4);
    connect(m_ui->m_ResetButton,SIGNAL(clicked()),this,SLOT(resetBoidsButton()));
    connect(m_ui->m_nrBoidsSB, SIGNAL (valueChanged(int)), this, SLOT(updateNumberBoids()));

    connect(m_ui->m_boidSize, SIGNAL (valueChanged(int)), this, SLOT(resetSize()));

    //behavior sliders
    connect(m_ui->m_seekRad, SIGNAL (valueChanged(int)), this, SLOT(resetBehavior()));
    connect(m_ui->m_alignRad, SIGNAL (valueChanged(int)), this, SLOT(resetBehavior()));
    connect(m_ui->m_sepRad, SIGNAL (valueChanged(int)), this, SLOT(resetBehavior()));

    connect(m_ui->m_velocity, SIGNAL (valueChanged(int)), this, SLOT(resetVelocity()));



}

MainWindow::~MainWindow()
{

    delete m_ui;
}

void MainWindow::resetBoidsButton()
{
    m_gl->resetBoids();
}
void MainWindow:: updateNumberBoids()
{
    int currentBoidsNub=m_ui->m_nrBoidsSB->value();
    std::cout<<"number boids was called "<<m_numBoids<<"\n";
    //if the current numb introduced in the spinBox is higher
    //than the numb of boids => call ADD_Boids else REMOVE_Boids
    if (currentBoidsNub>m_numBoids)
    {
        //the number of itteration will be
        //the difference between the two numbers
        int itterationNumb=currentBoidsNub-m_numBoids;
        for (int i=0; i<itterationNumb; i++)
        {
            std::cout<<"Add\n";
            m_gl->addBoid();
            m_numBoids++;
        }

    }
    else
    {
        //the number of itteration will be
        //the difference between the two numbers
        int itterationNumb=m_numBoids-currentBoidsNub;
        for (int i=0; i<itterationNumb; i++)
        {
            std::cout<<"Remove\n";
            m_gl->removeBoid();
            m_numBoids--;
        }
    }
}
void MainWindow::resetSize()
{
    int _rad=m_ui->m_boidSize->value();
    m_gl->resizeBoid(_rad);

}

void MainWindow::resetBehavior()
{

    int _seekRad, _alignRad, _sepRad;
    _seekRad=m_ui->m_seekRad->value();
    _alignRad=m_ui->m_alignRad->value();
    _sepRad=m_ui->m_sepRad->value();

    m_gl->resetBehavior(float(_seekRad), float(_alignRad), float(_sepRad) );
}

void MainWindow::resetVelocity()
{
    std::cout<<"in reset velocity \n";

    int _velocity;
    _velocity=m_ui->m_velocity->value();
    m_gl->resetVelocity(float (_velocity));
}
