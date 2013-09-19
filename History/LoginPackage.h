#ifndef LOGINPACKAGE_H
#define LOGINPACKAGE_H

struct LoginPackage
{
    LoginPackage(unsigned int loggedUin)
        : loggedUin(loggedUin)
    {

    }

    unsigned int loggedUin;
};

#endif // LOGINPACKAGE_H
