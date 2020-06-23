#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load Mesh", QDir::homePath());
    if(fileName.isEmpty())
        return;
    if(!OpenMesh::IO::read_mesh(mesh, fileName.toStdString()))
        return;

    alignMesh();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(mesh.n_vertices() == 0)
        return;
    QString fileName = QFileDialog::getSaveFileName(this, "Load Mesh", QDir::homePath());
    if(fileName.isEmpty())
        return;
    OpenMesh::IO::write_mesh(mesh, fileName.toStdString());
}

void MainWindow::alignMesh()
{
    OpenMesh::Vec3f cm(0.0, 0.0, 0.0);
    for(MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
    {
        cm += mesh.point(*v_it);
    }
    cm /= mesh.n_vertices();

    // translate to origin
    for(MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
    {
        mesh.point(*v_it) -= cm;
    }

    // determin radius
    double rMax = 0.0;
    for(MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
    {
        rMax = rMax > mesh.point(*v_it).length() ? rMax : mesh.point(*v_it).length();
    }

    // rescale to unit sphere
    for(MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
    {
        mesh.point(*v_it) /= rMax;
    }
}
