#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include "../headers/utils.hpp"

#include <pqxx/pqxx>

class Database
{
    public:
        Database();
        ~Database();
        std::string getPasswordSN(const char*); 
        JSONB getPasswordIP(const char*);
    private:
        pqxx::connection* _conn = NULL;
};
#endif
