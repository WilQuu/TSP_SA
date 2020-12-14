#include <iostream>
#include <fstream>
#include<string>
#include<cstdlib>
#include <sstream>
#include <cstring>
#include<time.h>
#include<math.h>
#include<ctime>
using namespace std;

void printPath(int arr[], int size) {

    for (int i = 0; i < size; i++) {
        if (i == size - 1) {
            cout << arr[i] << endl;
        }
        else
            cout << arr[i] << " -> ";

    }
}
double temperatureFunction(double t,double r) {
    return t * r;
}
void copyArray(int* arrA, int* arrB, int size) {
    for (int i = 0; i < size; i++)
        arrA[i] = arrB[i];
}
double eulerFun(double deltaT, double t) {
    return exp((-1.0) * deltaT / t);
}

int countCost(int* path, int citiesNum, int** cityMatrix) {
    int cost = 0;
    for (int i = 0; i < citiesNum; i++) {
        cost += cityMatrix[path[i]][path[i+1]];
    }
    cost += cityMatrix[citiesNum-1][path[0]];
    return cost;
}
void newPath(int* path,int citiesNum) {
    int randA, randB;
    randA = rand() % citiesNum;
    randB = rand() % citiesNum;
    if (randA == randB || randA == 0 || randB == 0 )
        newPath(path, citiesNum);
    else
        swap(path[randA], path[randB]);
   // cout << "RandomA = " << randA << endl;
   // cout << "RandomB = " << randB << endl;
    //cout << "---------------------------------" << endl;
 }


int main() {
    srand(time(NULL));
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

        /*    deklarowanie zmiennych potrzebnych do wykonania algorytmu   */
        double startingTemperature = 10000000.0;
        double temperature = startingTemperature;
        double stopTemperature = 0.00001;
        int bestCost, tempCost,currentCost;
        int* bestPath = new int[citiesNum + 1];
        int* currentPath = new int[citiesNum + 1];
        int* tempPath = new int[citiesNum + 1];
        double annealing = 0.9;

        /* inicjalizacja sciezki poczatkowej */
        for (int i = 0; i <= citiesNum; i++) {
            if (i == citiesNum)
                currentPath[citiesNum] = 0; 
            else
                currentPath[i] = i; 
        }
        bestPath = currentPath;
        printPath(currentPath, citiesNum+1);
        cout << "Koszt sciezki : " << countCost(currentPath, citiesNum, cityMatrix) << endl;
        double deltaT;
        int L = citiesNum*citiesNum*citiesNum;
        clock_t start = clock();
        /*   tutaj zaczyna sie algorytm SA     */

        do{
            for (int i = 0; i < L; i++) {
                copyArray(tempPath, currentPath, citiesNum + 1);
                newPath(currentPath, citiesNum);
                if (countCost(tempPath, citiesNum, cityMatrix) < countCost(bestPath, citiesNum, cityMatrix))
                    copyArray(bestPath, tempPath,citiesNum+1);
                deltaT = (double)countCost(tempPath, citiesNum, cityMatrix) -(double) countCost(currentPath, citiesNum, cityMatrix);
                if (deltaT < 0)
                    currentPath = tempPath;
                else {
                    double randomX = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
                    if (randomX < eulerFun(deltaT, temperature))
                        copyArray(currentPath, tempPath, citiesNum + 1);
                }   

            }
            
           temperature =  temperatureFunction(temperature, annealing);
        } while (temperature >= stopTemperature);
              
        
        double PRD = (double)countCost(bestPath,citiesNum ,cityMatrix) / hamiltonOpt;
        cout << "Koszt bestPath :" << countCost(bestPath, citiesNum, cityMatrix) << endl;
        cout << "PRD = " << PRD*100 <<" %"<< endl;
        printPath(bestPath, citiesNum+1);
        /*   tutaj konczy  sie algorytm SA      */

        clock_t stop = clock();
        double  elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
        cout << elapsed << " [s]";

        /*        zwalnianie pamieci               */  
        for (int i = 0; i < citiesNum; i++)
            delete[] cityMatrix[i];
        delete[]cityMatrix;

        delete[]bestPath;
        delete[]currentPath;
        delete[]tempPath;

    }
    else cerr << "Dostep do pliku zostal zabroniony!" << endl;



    file.close();
    return 0;
}
