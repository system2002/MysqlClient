#include <iostream>
#include "tmc.h"
#include "simple.h"
#include <string>
#include <map>

using namespace std;

int main()
{

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



        /*
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
    */
    }
    else
    {
        cout << s.lastErrorText() << endl;
        //cout << client.status() << endl;
    }


    /*
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
*/
    /*
    if (!client.changeUser("a", "b", "c"))
    {
        cout << client.lastErrorText() << endl;
    }
    cout << "Server Up time" << client.serverUptime() << endl;
*/
    /*
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
    return 0;
}
