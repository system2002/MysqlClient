#include <iostream>
#include <cstring>
#include "tmc.h"
#include "simple.h"
#include "mquery.h"
#include <string>
#include <map>
#include <chrono>
#define SELECT_SAMPLE "SELECT 'abcdy', 12, 'rrr', 'sdsdsd', 12.3, now() "
using namespace std;

int main()
{
    bool test = true;

    if (test)
    {
        MQuery q("dev-mysql", "igp", "123", "test");
        if (!q.isConnected())
        {
            cout << q.lastErrorText() << endl;
            exit(1);
        }
        cout << "connect Ok" << endl;
        if (!q.exec(SELECT_SAMPLE))
        {
            cout << q.lastErrorText() << endl;
            exit(2);
        }
        cout << "select Ok" << endl;
        cout << "col count:" << q.colCount() << endl;
        for (unsigned int i = 0; i < q.colCount(); i++)
        {
            cout <<"col["<< i << "] name:" << q.column(i).name() << endl;
            cout <<"col["<< i << "] type str:" << q.column(i).typeString() << endl;
            cout <<"col["<< i << "] type:" << q.column(i).type() << endl;
            cout <<"col["<< i << "] length:" << q.column(i).length() << endl;
            cout <<"col["<< i << "] max length:" << q.column(i).maxLength()<< endl;
            cout <<"col["<< i << "] pk:" << q.column(i).isPrimaryKey() << endl;
            cout <<"col["<< i << "] unsigned:" << q.column(i).isUnsigned() << endl;
            cout <<"col["<< i << "] isNotNull:" << q.column(i).isNotNul() << endl;
        }
        while (q.next())
        {
            //cout <<"# " << endl;
            //cout << q.column(0).maxLength() << endl;
            for (unsigned int i = 0; i < q.colCount(); i++)
            {
                /*
                std::chrono::time_point<std::chrono::system_clock> now;
                    now = q.value(0).toChronoTimePoint();
                    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                    std::cout << "One day ago, the time was "
                              << std::ctime(&now_c);
                    tm *tt = localtime(&now_c);
*/
                cout <<q.value(i).toString() <<" | ";

                /*
                time_t time = q.value(0).toCTime();;
                cout << ctime(&time) << endl;
                tm *t = localtime(&time);
                //cout << t->tm_mon << endl;

                char buffer[100];
                std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", t);
                cout << buffer<<endl;
                */

            }
            cout << endl;
        }
//        cout << q.lastErrorText()<< endl;

    }
    else
    {
    MYSQL *mysql = mysql_init(nullptr);
    MYSQL_STMT    *stmt;
    MYSQL_BIND    bind[4];
    MYSQL_RES     *prepare_meta_result;
    MYSQL_TIME    ts;
    unsigned long length[4];
    int           param_count, column_count, row_count;
    short         small_data;
    int           int_data;
    char          str_data[50];
    my_bool       is_null[4];
    my_bool       error[4];
    if (!mysql_real_connect(mysql, "dev-mysql", "igp", "123", "test", 3306, nullptr, 0))
    {
        cout << mysql_error(mysql);
    }
    else
    {
        cout << "Connect ok" << endl;
        stmt = mysql_stmt_init(mysql);
        if (!stmt)
        {
            fprintf(stderr, "mysql_stmt_init(), out of memory\n");
            exit(0);
        }
        cout << "stmt init ok" << endl;
        char* sql = "SELECT 1, 'abc', 11, now()";
        if (mysql_stmt_prepare(stmt, sql, strlen(sql)) !=0 )
        {
            cout << "mysql_stmt_prepare(), SELECT failed\n" << endl;
            cout << mysql_stmt_error(stmt) << endl;
            //fprintf(stderr, "mysql_stmt_prepare(), SELECT failed\n");
            //fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
            exit(0);
        }
        else
        {
            fprintf(stdout, " prepare, SELECT successful\n");
            param_count= mysql_stmt_param_count(stmt);
            fprintf(stdout, " total parameters in SELECT: %d\n", param_count);

            if (param_count != 0) /* validate parameter count */
            {
              fprintf(stderr, " invalid parameter count returned by MySQL\n");
              exit(0);
            }
            prepare_meta_result = mysql_stmt_result_metadata(stmt);
            if (prepare_meta_result == nullptr)
            {
              fprintf(stderr,
                     " mysql_stmt_result_metadata(), \
                       returned no meta information\n");
              fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
              exit(0);
            }

            /* Get total columns in the query */
            column_count= mysql_num_fields(prepare_meta_result);
            fprintf(stdout,
                    " total columns in SELECT statement: %d\n",
                    column_count);

            if (column_count != 4) /* validate column count */
            {
              fprintf(stderr, " invalid column count returned by MySQL\n");
              exit(0);
            }

            if (mysql_stmt_execute(stmt))
            {
              fprintf(stderr, " mysql_stmt_execute(), failed\n");
              fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
              exit(0);
            }

            memset(bind, 0, sizeof(bind));

            bind[0].buffer_type= MYSQL_TYPE_LONG;
            bind[0].buffer= (char *)&int_data;
            bind[0].is_null= &is_null[0];
            bind[0].length= &length[0];
            bind[0].error= &error[0];

            /* STRING COLUMN */
            bind[1].buffer_type= MYSQL_TYPE_STRING;
            bind[1].buffer= (char *)str_data;
            bind[1].buffer_length= 50;
            bind[1].is_null= &is_null[1];
            bind[1].length= &length[1];
            bind[1].error= &error[1];

            /* SMALLINT COLUMN */
            bind[2].buffer_type= MYSQL_TYPE_SHORT;
            bind[2].buffer= (char *)&small_data;
            bind[2].is_null= &is_null[2];
            bind[2].length= &length[2];
            bind[2].error= &error[2];

            /* TIMESTAMP COLUMN */
            bind[3].buffer_type= MYSQL_TYPE_TIMESTAMP;
            bind[3].buffer= (char *)&ts;
            bind[3].is_null= &is_null[3];
            bind[3].length= &length[3];
            bind[3].error= &error[3];

            if (mysql_stmt_bind_result(stmt, bind))
            {
              fprintf(stderr, " mysql_stmt_bind_result() failed\n");
              fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
              exit(0);
            }

            /* Now buffer all results to client (optional step) */
            if (mysql_stmt_store_result(stmt))
            {
              fprintf(stderr, " mysql_stmt_store_result() failed\n");
              fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
              exit(0);
            }


            row_count= 0;
            fprintf(stdout, "Fetching results ...\n");
            while (!mysql_stmt_fetch(stmt))
            {
              row_count++;
              fprintf(stdout, "  row %d\n", row_count);

              /* column 1 */
              fprintf(stdout, "   column1 (integer)  : ");
              if (is_null[0])
                fprintf(stdout, " NULL\n");
              else
                fprintf(stdout, " %d(%ld)\n", int_data, length[0]);

              /* column 2 */
              fprintf(stdout, "   column2 (string)   : ");
              if (is_null[1])
                fprintf(stdout, " NULL\n");
              else
                fprintf(stdout, " %s(%ld)\n", str_data, length[1]);

              /* column 3 */
              fprintf(stdout, "   column3 (smallint) : ");
              if (is_null[2])
                fprintf(stdout, " NULL\n");
              else
                fprintf(stdout, " %d(%ld)\n", small_data, length[2]);

              /* column 4 */
              fprintf(stdout, "   column4 (timestamp): ");
              if (is_null[3])
                fprintf(stdout, " NULL\n");
              else
                fprintf(stdout, " %04d-%02d-%02d %02d:%02d:%02d (%ld)\n",
                                 ts.year, ts.month, ts.day,
                                 ts.hour, ts.minute, ts.second,
                                 length[3]);
              fprintf(stdout, "\n");
            }

            /* Validate rows fetched */
            fprintf(stdout, " total rows fetched: %d\n", row_count);
            if (row_count != 2)
            {
              fprintf(stderr, " MySQL failed to return all rows\n");
              exit(0);
            }

            /* Free the prepared result metadata */
            mysql_free_result(prepare_meta_result);


            /* Close the statement */
            if (mysql_stmt_close(stmt))
            {
              /* mysql_stmt_close() invalidates stmt, so call          */
              /* mysql_error(mysql) rather than mysql_stmt_error(stmt) */
              fprintf(stderr, " failed while closing the statement\n");
              fprintf(stderr, " %s\n", mysql_error(mysql));
              exit(0);
            }


        }


    }



    /*
    Simple s("dev-mysql", "igp", "123", "test");
    if (!s.isConnected())
    {
        cout << s.lastErrorText() << endl;
    }

    if (s.exec("SELECT * FROM port_params"))
    {
        cout <<"col count: "<< s.colCount() << endl;
        for (unsigned int i = 0; i < s.colCount(); i++)
        {

            cout <<"col["<< i << "] name:" << s.column(i).name() << endl;
            cout <<"col["<< i << "] type:" << s.column(i).typeString() << endl;
            cout <<"col["<< i << "] length:" << s.column(i).length() << endl;
            cout <<"col["<< i << "] pk:" << s.column(i).isPrimaryKey() << endl;
            cout <<"col["<< i << "] unsigned:" << s.column(i).isUnsigned() << endl;
            cout <<"col["<< i << "] isNotNull:" << s.column(i).isNotNul() << endl;
        }
        uint32_t count = s.colCount();

        std::vector<std::vector<std::string>> arr = s.getAllrowString();


        for (const std::vector<std::string> & v : arr)
        {
            for (const std::string & value : v)
            {
                cout << value << "\t|";
            }
            cout << endl;
        }




        while (s.next())
        {
            //cout <<"|";

            vector<string> vs = s.rowVS();
            {
                //cout << endl<<"#"<< i <<"#\t";
                cout << vs[i] << "\t|";
                //cout << s.row()[i] << "\t|";
            }

            map<string, string> ms = s.rowMS();


            for (const pair<string, string> & value : ms)
            {
                cout << value.first << ":"<< value.second << "\t|";
            }

            cout << endl;
        }

    }
    else
    {
        cout << s.lastErrorText() << endl;
        //cout << client.status() << endl;
    }



    tmc client;
    if (!client.connect("dev-mysql", "igp", "123", "test"))
    {
        cout << client.lastErrorText() << endl;
    }

    //cout <<"col count"<< client.colCount() << endl;
    //cout <<"change base: " << client.changeBase("term1") << endl;


    //cout << client.lastErrorText() << endl;
    cout << "Server stattus:" <<client.getServerStatus() << endl;
    cout << "Server info" << client.getServerInfo() << endl;
    cout << "Server Up time" << client.serverUptime() << endl;


    if (!client.changeUser("a", "b", "c"))
    {
        cout << client.lastErrorText() << endl;
    }
    cout << "Server Up time" << client.serverUptime() << endl;


    if (!client.exec("SHOW TABLES"))
    {
        cout << client.lastErrorText() << endl;
        //cout << client.status() << endl;
    }
    else
    {
        cout <<"col count: "<< client.colCount() << endl;
        for (unsigned int i = 0; i < client.colCount(); i++)
        {

            cout <<"col["<< i << "] name:" << client.column(i).name() << endl;
            cout <<"col["<< i << "] type:" << client.column(i).typeString() << endl;
            cout <<"col["<< i << "] length:" << client.column(i).length() << endl;
            cout <<"col["<< i << "] pk:" << client.column(i).isPrimaryKey() << endl;
            cout <<"col["<< i << "] isNotNull:" << client.column(i).isNotNul() << endl;
        }
        uint32_t count = client.colCount();
        while (client.next())
        {
            cout <<"|";
            for (uint32_t i = 0; i < count; i++)
            {
                cout << client.value(i) << "\t|";
            }
            cout << endl;
        }
    }
    */
    }
    return 0;
}
