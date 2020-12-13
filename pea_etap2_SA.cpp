#include <iostream>
#include <fstream>
#include<string>
#include<cstdlib>
#include <sstream>
#include <cstring>
#include<time.h>
#include<math.h>
using namespace std;

double temperature(double t,double r) {
    return t * r;
}

double randomChoosing(double deltaT, double t) {
    return exp((-1) * deltaT / t);
}

int main() {
    fstream file;
    string tempLine;
    int hamiltonOpt;
    int citiesNum;
    string fileName;
    cout << "Name of instance : ";
    cin >> fileName;
    file.open(fileName, std::ios::in | std::ios::out);
    if (file.is_open()) {
        getline(file, tempLine);
        getline(file, tempLine);
        citiesNum = atoi(tempLine.c_str());
        int** cityMatrix = new int* [citiesNum];
        for (int i = 0; i < citiesNum; i++)
            cityMatrix[i] = new int[citiesNum];


        for (int i = 0; i < citiesNum; i++) {
            getline(file, tempLine);
            stringstream ss;
            ss << tempLine;
            int found;
            string temp;
            int y = 0;
            while (!ss.eof()) {
                ss >> temp;
                if (stringstream(temp) >> found) {
                    cityMatrix[i][y] = found;
                    y++;
                }
            }

            temp = "";
        }
        getline(file, tempLine);
        hamiltonOpt = atoi(tempLine.c_str());

        cout << "------------city matrix------------" << endl;

        for (int i = 0; i < citiesNum; i++) {
            for (int j = 0; j < citiesNum; j++)
                cout << *(*(cityMatrix + i) + j) << " ";
            cout << endl;
        }


        double startingTemperature = 99999999999999999999999999999999999999999.0;
        double temperature = startingTemperature;
        double stopTemperature = 0.00001;
        int bestCost;
        int* bestPath = new int[citiesNum + 1];


        clock_t start = clock();
        /*   tutaj zaczyna sie algorytm SA     */

       
        

        

        





        /*   tutaj konczy  sie algorytm SA      */

        clock_t stop = clock();
        double elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
        cout << elapsed << " [s]";

        /*        zwalnianie pamieci               */   
        for (int i = 0; i < citiesNum; i++)
            delete[] cityMatrix[i];
        delete[]cityMatrix;

        delete[]bestPath;

        

    }
    else cerr << "Dostep do pliku zostal zabroniony!" << endl;



    file.close();
    return 0;
}
