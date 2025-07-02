#include <QApplication>
#include <QFile>

#include "UI/Mainwindow.h"
#include "UI/LoginDialog.h"
#include "UI/NumberKeyboard.h"
#include "System/BanklightControl.h"

using namespace UI;
using namespace System::Screen;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    //加载QSS样式表
    QFile qss(":/qss/Style.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();

    BanklightControl banklightControl;
    banklightControl.SetPower(0);  //开启背光电源
    banklightControl.SetBrightness(7);//调亮

    CNumberKeyboard::Instance()->hide();

    LoginDialog loginDialog(0,180);
    loginDialog.exec();

    MainWindow w;
    w.show();

    return a.exec();
}
