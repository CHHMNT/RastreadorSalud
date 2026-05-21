#include <QApplication>
#include <QDir>
#include "Database.h"
#include "LoginDialog.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Open the database in the user's home directory
    Database db;
    if (!db.open(QDir::homePath() + "/rastreador.db")) {
        return 1;
    }

    // Show login dialog
    LoginDialog login(&db);
    if (login.exec() != QDialog::Accepted) {
        return 0; // User closed the dialog
    }

    // Launch main window with the logged-in user
    MainWindow window(&db, login.getLoggedInUser());
    window.show();

    return app.exec();
}