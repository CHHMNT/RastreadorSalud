#include "User.h"

User::User() : id(-1), username(""), password("") {}

User::User(int id, const QString &username, const QString &password)
    : id(id), username(username), password(password) {}

int     User::getId()       const { return id; }
QString User::getUsername() const { return username; }
QString User::getPassword() const { return password; }

void User::setId(int i)                  { id = i; }
void User::setUsername(const QString &u) { username = u; }
void User::setPassword(const QString &p) { password = p; }
