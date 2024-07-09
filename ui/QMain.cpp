#include "ui/QMainWindowVMTK.h"
#include "ui/QSplashScreenCopyright.h"
#include <QThread>
#include <QApplication>
#include <QSplashScreen>
#ifdef enableOldQGL
    #include <QDesktopWidget>
#endif
#include <QStyle>
#include <fstream>
#include <QFontDatabase>
#include <QMessageBox>

#ifdef _CREATE_WEB_SERVER
    #include "web/QWebSocketServerThread.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int idFont = QFontDatabase::addApplicationFont(":/font_main");

    if(idFont==-1){
        QMessageBox::information(nullptr,"Alert message","The font file is not loaded.");
        return 0;
    }
#endif
    QSplashScreenCopyright s;
    s.setSecondsAutoClose(6);
    s.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint); //Set window to fixed size
    s.setWindowFlags(Qt::CustomizeWindowHint); //Set window with no title bar


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        s.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, s.size(), qApp->desktop()->availableGeometry()));
#else
        s.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, s.size(), qApp->primaryScreen()->availableGeometry()));
#endif

    s.show();
    a.exec();

//    std::ofstream out("out.txt");
//    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
//    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

//    std::ofstream cerr_out("cerr.txt");
//    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
//    std::cerr.rdbuf(cerr_out.rdbuf()); //redirect std::cout to out.txt!

//    if (!QGLFormat::hasOpenGL()) { /** OpenGl support or not? */
//        std::cout << "This system has no OpenGL support" << std::endl;
//        return 1;
//    }
//    std::cout << "This system has OpenGL support" << std::endl;

#ifdef _CREATE_WEB_SERVER
    QWebSocketServerThread *server = new QWebSocketServerThread();
//    server->run();
    server->start();
#endif

    QMainWindowVMTK w;

    w.show();

    return a.exec();
}
