#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

/**
 * @brief Constructs the LoginDialog with username/password fields.
 * @param db Pointer to the shared Database instance
 * @param parent Parent widget
 */
LoginDialog::LoginDialog(Database *db, QWidget *parent)
    : QDialog(parent), db(db) {

    setWindowTitle("Rastreador Personal de Salud — Login");
    setMinimumWidth(350);

    // Widgets
    usernameEdit  = new QLineEdit(this);
    passwordEdit  = new QLineEdit(this);
    loginButton   = new QPushButton("Login", this);
    registerButton= new QPushButton("Register", this);
    messageLabel  = new QLabel("", this);

    passwordEdit->setEchoMode(QLineEdit::Password);
    messageLabel->setStyleSheet("color: red;");

    // Layout
    QFormLayout *form = new QFormLayout();
    form->addRow("Username:", usernameEdit);
    form->addRow("Password:", passwordEdit);

    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addWidget(loginButton);
    buttons->addWidget(registerButton);

    QVBoxLayout *main = new QVBoxLayout(this);
    main->addLayout(form);
    main->addLayout(buttons);
    main->addWidget(messageLabel);

    // Connections
    connect(loginButton,    &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);
}

/**
 * @brief Returns the user that successfully logged in.
 */
User LoginDialog::getLoggedInUser() const {
    return loggedInUser;
}

/**
 * @brief Validates credentials against the database and accepts the dialog.
 */
void LoginDialog::onLoginClicked() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        messageLabel->setText("Please enter username and password.");
        return;
    }

    User u = db->getUserByUsername(username);

    if (u.getId() == -1) {
        messageLabel->setText("User not found.");
        return;
    }

    if (u.getPassword() != password) {
        messageLabel->setText("Incorrect password.");
        return;
    }

    loggedInUser = u;
    accept(); // Closes dialog with QDialog::Accepted
}

/**
 * @brief Registers a new user and automatically logs them in.
 */
void LoginDialog::onRegisterClicked() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        messageLabel->setText("Please enter username and password.");
        return;
    }

    User existing = db->getUserByUsername(username);
    if (existing.getId() != -1) {
        messageLabel->setText("Username already exists.");
        return;
    }

    if (db->insertUser(username, password)) {
        loggedInUser = db->getUserByUsername(username);
        accept();
    } else {
        messageLabel->setText("Registration failed.");
    }
}