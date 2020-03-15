#include <iostream>
#include "tmc.h"
#include <string>

using namespace std;

int main()
{
    tmc client;
    if (!client.connect("192.168.0.5", "system", "1234", "log"))
    {
        cout << client.lastErrorText() << endl;
    }

    //cout <<"col count"<< client.colCount() << endl;
    cout <<"change base: " << client.changeBase("term1") << endl;


    //cout << client.lastErrorText() << endl;
    cout << "Server stattus:" <<client.getServerStatus() << endl;
    cout << "Server info" << client.getServerInfo() << endl;
    cout << "Server Up time" << client.serverUptime() << endl;

    /*
    if (!client.changeUser("a", "b", "c"))
    {
        cout << client.lastErrorText() << endl;
    }
    cout << "Server Up time" << client.serverUptime() << endl;
*/
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
    return 0;
}
