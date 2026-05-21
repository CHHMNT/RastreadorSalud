/**
 * @file LoginDialog.h
 * @brief Dialog window for user authentication.
 */
#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "Database.h"
#include "User.h"

/**
 * @class LoginDialog
 * @brief Presents a login/register form and validates credentials.
 */
class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(Database *db, QWidget *parent = nullptr);
    User getLoggedInUser() const;

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    Database    *db;           ///< Pointer to the shared database instance
    User         loggedInUser; ///< Stores the authenticated user on success

    QLineEdit   *usernameEdit;
    QLineEdit   *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLabel      *messageLabel;
};

#endif