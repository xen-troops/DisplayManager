#ifndef DBUSCONTROLADAPTEE_HPP
#define DBUSCONTROLADAPTEE_HPP

#include <stdint.h>

class DBusControlAdaptee {
public:
    virtual void userEvent(uint32_t event) = 0;
};

#endif /* DBUSCONTROLADAPTEE_HPP */
