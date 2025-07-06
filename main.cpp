#include <opencv2/opencv.hpp>
#include <iostream>
#include <mysql_connection.h>
// #include <image_recieve.h>

using namespace cv;
using namespace std;
using namespace sql;

int choices();
int * get_hsv_values();
char get_shape_value();
int initialize_database();


int main() 
{
  unique_ptr<Table> table;
  unique_ptr<Schema> data;
  unique_ptr<Session> s2;

  check_for_images("AAA");

  try
  {
  s2 = make_unique<Session>("48.217.67.239", 33060, "esp", "Ulq24");
  data = make_unique<Schema>(s2->getSchema("data"));
  table = make_unique<Table>(data->getTable("data_line"));
  }
  catch(const mysql::Error &err)
  {
    cout << err;
  }
  int choice;
  while(true)
  {
    cout << "This is a pretty simple interface to interact with data transfers\n";
    choice = choices();
    if(choice == 0)
    {
      cout << "Please give low and high HSV values to send data";
      int * hsv_values = get_hsv_values();
      try{
        table->
         update()
        .set("H1", *(hsv_values))
        .set("S1",*(hsv_values + 1))
        .set("V1",*(hsv_values + 2))
        .set("H2",*(hsv_values + 3))
        .set("S2", *(hsv_values + 4))
        .set("V2", *(hsv_values + 5))
        .set("SHAPE", "G")
        .where("key = 1111")
        .execute();
      }
      catch(const mysqlx::Error &err)
      {
      cout << err;
      }
      delete[] hsv_values;
    }
    else if(choice == 1)
    {
      cout << "Please choose from the following shapes you want to detect on the ESP32: " << "\n Rectangle(R)," << "\n Triangle(T)" << " \n circle(C)" << "\n Square(S)\n";
      char shape_choice = get_shape_value();
      cout << shape_choice << " in question";
      try{
        table->
         update()
        .set("H1", -1)
        .set("S1",-1)
        .set("V1",-1)
        .set("H2",-1)
        .set("S2", -1)
        .set("V2", -1)
        .set("SHAPE", shape_choice)
        .execute();
      }
      catch(const mysql::Error &err)
      {
      cout << err;
      }
  }
  }
return 0;
}

char get_shape_value()
{
  char choice;
  do
  {
    cout << "\nChoose for an shape value: ";
    cin >> choice;
  } while (choice != 'R' && choice != 'T' && choice != 'C' && choice != 'S');
  
  return choice;
}

int * get_hsv_values()
{
    int i; 
    int* hsv = new int[6];
    for(i = 0; i < 6; i ++)
    {
      do
      { 
        
      cout << "\nChoose for value " << i << ": ";
      cin >> hsv[i];

      }while(hsv[i] < 0 || hsv[i] > 255);
    }
  return hsv;
}

int choices()
{
  int choices;
  do
  {
    cout << "\n Please choose one or zero: ";
    cin >> choices;
  } while (choices != 0 && choices != 1);
  return choices;
}
