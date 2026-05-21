/**
 * @file User.h
 * @brief Defines the User class for authentication and user data.
 */
#ifndef USER_H
#define USER_H

#include <QString>

/**
 * @class User
 * @brief Represents an authenticated user of the application.
 */
class User {
public:
    User();
    User(int id, const QString &username, const QString &password);

    int getId() const;
    QString getUsername() const;
    QString getPassword() const;

    void setId(int id);
    void setUsername(const QString &username);
    void setPassword(const QString &password);

private:
    int id;           ///< Unique user ID from the database
    QString username; ///< Login username
    QString password; ///< Hashed or plain password
};

#endif