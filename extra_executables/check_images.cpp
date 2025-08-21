#include <iostream>
#include <fstream>
#include <xdevapi.h>
#include <string>

using namespace std;
using namespace mysqlx;



Row row;
ofstream outputFileStream;
void check_for_images(char * NAME)
{
    try {
        unique_ptr<Session> s2;
        s2 = make_unique<Session>("48.217.67.239", 33060, "esp", "Ulq24");

        SqlResult res = s2->sql("SELECT image_name, image_s FROM data.image_line  WHERE `Key` = '1111';").execute();

        row = res.fetchOne();
        cout << row[0] << endl;
    }
    catch (const mysqlx::Error &err) {
        cerr << err;
    }

    if (row[0].get<std::string>() != NAME) {
        cout << "New Picture Detected\n";
        auto bytes = row[1].getRawBytes();
        const char* data = reinterpret_cast<const char*>(bytes.first);
        outputFileStream.open("saved_images.png",std::ios::binary);
        outputFileStream.write(data, bytes.second);


    }
    else {
        cout << "No New Picture Detected";
    }
}