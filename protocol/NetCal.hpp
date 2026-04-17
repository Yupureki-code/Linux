#pragma once

#include "protocol.hpp"

class cal
{
public:
    Reponse func(Request &req)
    {
        auto x = req.get_x();
        auto y = req.get_y();
        char oper = req.get_oper();
        switch (oper)
        {
        case '+':
            return Reponse(x + y, 0);
        case '-':
            return Reponse(x - y, 0);
        case '*':
            return Reponse(x * y, 0);
        case '/':
            if (y == 0)
                return Reponse(0, 1);
            else
                return Reponse(x / y, 0);
        case '%':
            return Reponse(x % y, 0);
        default:
            return Reponse(0, 2);
        }
    }
};
