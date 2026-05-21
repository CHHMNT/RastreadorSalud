#include "LoginDialog.h"

LoginDialog::LoginDialog(Database *db, QWidget *parent)
    : QDialog(parent), db(db) {}

User LoginDialog::getLoggedInUser() const { return loggedInUser; }
void LoginDialog::onLoginClicked()    {}
void LoginDialog::onRegisterClicked() {}
