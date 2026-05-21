#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QLabel label("Rastreador Personal de Salud");
    label.show();
    return app.exec();
}
