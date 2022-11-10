#include <iostream>

#include "../headers/Database.h"
#include "../headers/colors.h"

Database::Database()
{
    std::cout << std::endl << Color::GREEN;
    std::cout << " Connected to PostgreSQL: " << this->_conn.dbname() << std::endl;
    std::cout << Color::RESET << std::endl;
}

Database::~Database()
{

}

std::string Database::getPasswordSN(const char* serial_number)
{
    return "";
}

JSONB Database::getPasswordIP(const char* ip)
{
    pqxx::work txn{this->_conn};
    pqxx::result r{txn.exec("SELECT PASSWORDS "
                            "FROM SCAN_HISTORY "
                            "WHERE PASSWORDS IS NOT NULL AND "
                            "IP_ADDRESS = TRIM(" + txn.quote(ip) + ") " +
                            "ORDER BY DATETIME DESC;")};
    JSONB res;

    for (auto row: r)
    {
        res.parse(row["PASSWORDS"].c_str());
    }
    
    r.clear();
    return res;
}
