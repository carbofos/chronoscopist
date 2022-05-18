#pragma once

#include <vector>

#include <mysql/mysql.h>
#include <mysql/errmsg.h>
#include <mysql/mysqld_error.h>

#include "config.h"

class Db
{
    public:
        typedef MYSQL db_type;
        typedef MYSQL_RES result_type;
        typedef MYSQL_ROW row_type;
        
    private:
        Db() = delete;
        static std::vector<std::string> mysql_oligate_options;
        static db_type dbcondata;
        static db_type* dbconptr;
    public:
        static db_type* init_db();
        static int mysql_errorcode;
        static std::string mysql_error_msg;
        static result_type * query_select(const std::string &query);
        static row_type fetch_row(result_type * mysql_res);
        static unsigned int num_fields(result_type * mysql_res);
        static void close();
        static bool have_obligate_options();
};
