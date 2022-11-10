#include <iostream>

#include "../headers/Database.h"
#include "../headers/colors.h"

Database::Database()
{
	try
	{
		std::cout << std::endl;
		this->_conn = new pqxx::connection{"postgresql://fady@localhost/jojo?connect_timeout=10"};
		std::cout << Color::GREEN << " Connected to PostgreSQL: " << this->_conn->dbname() << std::endl;
		
	}
	catch(const pqxx::broken_connection& e)
	{
		std::cout << Color::RED << " Failed to connect to PostgreSQL" << std::endl;
		this->_conn = NULL;
	}
	std::cout << Color::RESET << std::endl;
}

Database::~Database()
{
	if(this->_conn != NULL)
		delete this->_conn;
}

std::string Database::getPasswordSN(const char* serial_number)
{
    return "";
}

JSONB Database::getPasswordIP(const char* ip)
{
	JSONB res;
	if(this->_conn != NULL)
	{
		pqxx::work txn{*this->_conn};
		pqxx::result r{txn.exec("SELECT PASSWORDS "
								"FROM SCAN_HISTORY "
								"WHERE PASSWORDS IS NOT NULL AND "
								"IP_ADDRESS = TRIM(" + txn.quote(ip) + ") " +
								"ORDER BY DATETIME DESC;")};
		
	
		for (auto row: r)
		{
			res.parse(row["PASSWORDS"].c_str());
		}
		
		r.clear();
	}
    return res;
}
