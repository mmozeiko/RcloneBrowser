#include "main_window.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    app.setApplicationDisplayName("Rclone Browser");
    app.setApplicationName("rclone-browser");
    app.setOrganizationName("rclone-browser");
    app.setWindowIcon(QIcon(":/icons/icon.png"));

    MainWindow w;
    w.show();

    return app.exec();
}
