#include "Window.h"

#include <QFileDialog>
#include <QLayout>
#include <QMenu>
#include <QStatusBar>
#include <QGroupBox>
#include <QDebug>

// #include "CImg.h"

// #include "img.h"
// #include "mathematics.h"

Window::Window()
{
    if (this->objectName().isEmpty())
        this->setObjectName("mainWindow");
    this->resize(929, 891);

    viewer = new TextureViewer(this);

    QWidget * tectureWidget = new QWidget(this);
    QGridLayout * gridLayout = new QGridLayout(tectureWidget);

    gridLayout->addWidget(viewer, 0, 1, 1, 1);

    QAction * actionLoad3Dimage = new QAction("Load 3D image", this);
    //QAction * actionLoadCimage = new QAction("Load Cimage", this);
    QAction * actionLoadOffMesh = new QAction("Load 3D Mesh (OFF)", this);
    QAction * recompileShaders = new QAction("Recompile shaders", this);
    QAction * actionRecale = new QAction("Recaler", this);


    QMenu * menuFile = new QMenu("File", this);

    menuFile->addAction(actionLoad3Dimage);
    menuFile->addAction(actionLoadOffMesh);


    connect(actionLoad3Dimage, SIGNAL(triggered()), this, SLOT(open3DImage()));
    //connect(actionLoadCimage, SIGNAL(triggered()), this, SLOT(openCImage()));
    connect(actionLoadOffMesh, &QAction::triggered, viewer, &TextureViewer::openMesh);
    connect(actionRecale, &QAction::triggered, viewer, &TextureViewer::alignMeshWithPointCloud);
    connect(recompileShaders, &QAction::triggered, viewer, &TextureViewer::recompileShaders);

    QGroupBox * viewerGroupBox = new QGroupBox ("Texture viewer", this);
    QHBoxLayout * viewerLayout = new QHBoxLayout (viewerGroupBox);
    viewerLayout->addWidget (tectureWidget);

    madDockWidget = new TextureDockWidget(this);

    this->addDockWidget(Qt::RightDockWidgetArea, madDockWidget);

    //TODO : Connect madDockWidget signals to viewer slots

    //TextureDockWidget * dockWidget = new TextureDockWidget(this);  // Si ce n'est pas déjà créé
    connect(madDockWidget, &TextureDockWidget::xValueChanged, viewer, &TextureViewer::setXCut);
    connect(madDockWidget, &TextureDockWidget::yValueChanged, viewer, &TextureViewer::setYCut);
    connect(madDockWidget, &TextureDockWidget::zValueChanged, viewer, &TextureViewer::setZCut);

    connect(madDockWidget, &TextureDockWidget::xInvert, viewer, &TextureViewer::invertXCut);
    connect(madDockWidget, &TextureDockWidget::yInvert, viewer, &TextureViewer::invertYCut);
    connect(madDockWidget, &TextureDockWidget::zInvert, viewer, &TextureViewer::invertZCut);

    connect(madDockWidget, &TextureDockWidget::xDisplay, viewer, &TextureViewer::setXCutDisplay);
    connect(madDockWidget, &TextureDockWidget::yDisplay, viewer, &TextureViewer::setYCutDisplay);
    connect(madDockWidget, &TextureDockWidget::zDisplay, viewer, &TextureViewer::setZCutDisplay);



    this->setCentralWidget(viewerGroupBox);

    QMenuBar * menubar = new QMenuBar(this);

    menubar->addAction(menuFile->menuAction());
    menubar->addAction(recompileShaders);
    menubar->addAction(actionRecale);

    this->setMenuBar(menubar);

    statusbar = new QStatusBar(this);

    this->setStatusBar(statusbar);

    this->setWindowTitle("Texture Viewer");
}


    

void Window::open3DImage(){

    QString selectedFilter, openFileNameLabel;
    QString fileFilter = "Known Filetypes (*.dim *.nii *.mhd);;IMA (*.dim);;NIFTI (*.nii);;MHD (*.mhd)";

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select an input 3D image"),
                                                    openFileNameLabel,
                                                    fileFilter,
                                                    &selectedFilter);

    // In case of Cancel
    if ( fileName.isEmpty() ) {
        return;
    }

    statusBar()->showMessage("Opening 3D image...");
    if(fileName.endsWith(".dim") || fileName.endsWith(".nii") || fileName.endsWith(".mhd") ){
        viewer->open3DImage(fileName);
        statusBar()->showMessage("3D image opened");

    }

}
