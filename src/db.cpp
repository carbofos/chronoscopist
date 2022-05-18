#include <mysql/mysql.h>
#include <mysql/errmsg.h>
#include <mysql/mysqld_error.h>
#include <iostream>

#include "db.h"

using namespace std;

Db::db_type* Db::init_db()
{  
    // if (dbconptr != nullptr)
    //     return dbconptr; // already connected
    
    if ( ! have_obligate_options() )
        {
            std::cout << "DB obligate option was not found in config file " << std::endl;
            return nullptr;
        }

    const char * host     = Config::options.at(mysql_oligate_options[0]).c_str();
    const char * user     = Config::options.at(mysql_oligate_options[1]).c_str();
    const char * password = Config::options.at(mysql_oligate_options[2]).c_str();
    const char * db       = Config::options.at(mysql_oligate_options[3]).c_str();
    const int port        = std::stoi(Config::options.at(mysql_oligate_options[4]).c_str() );

    if ( (host == nullptr) || (user == nullptr) || (password == nullptr) || (db == nullptr ) )
        return nullptr;

    mysql_init(&dbcondata);
    dbconptr = mysql_real_connect(
        &dbcondata,
        host,
        user,
        password,
        db,
        port,
        nullptr, 
        0);
    return dbconptr;
}

bool Db::have_obligate_options()
{
    for (const auto &option : mysql_oligate_options)
        if (Config::options.find(option) == Config::options.end() )
            return false;
    return true;
}

Db::result_type * Db::query_select(const std::string &query)
{
    mysql_errorcode = mysql_query(&dbcondata, query.c_str() );
    if (mysql_errorcode)
        mysql_error_msg = mysql_error(&dbcondata);
    return mysql_store_result(&dbcondata);
}

Db::row_type Db::fetch_row(result_type * mysql_res)
{
    return mysql_fetch_row(mysql_res);
}

unsigned int Db::num_fields(result_type * mysql_res)
{
    return mysql_num_fields(mysql_res);
}

std::vector<std::string> Db::mysql_oligate_options = 
{
    "mysql_host",
    "mysql_user",
    "mysql_password",
    "mysql_db",
    "mysql_port",
};

void Db::close()
{
    mysql_close(dbconptr);
}

Db::db_type Db::dbcondata;
Db::db_type* Db::dbconptr { nullptr };
std::string Db::mysql_error_msg;
int Db::mysql_errorcode;

