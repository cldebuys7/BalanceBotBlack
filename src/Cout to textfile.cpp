#include <iostream>
#include <fstream>
using namespace std;



int main()

{
ofstream out_data("filename.dat");

// then just replace all the cout with out_data everywhere

out_data << "whatever..";

// i think thats what you want?



return 0;
}