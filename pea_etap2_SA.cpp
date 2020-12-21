#include <iostream>
#include <fstream>
#include<string>
#include<cstdlib>
#include <sstream>
#include <cstring>
#include<time.h>
#include<math.h>
#include<ctime>
#include<vector>
using namespace std;

void randomPath(int* path, int citiesNum) {
    vector<int> tempNums;
    for (int i = 1; i < citiesNum; i++)
        tempNums.push_back(i);

    path[0] = 0;
    path[citiesNum] = 0;
    int currentVertex;
    vector<int> cleaningVector;
    for (int i = 1; i < citiesNum; i++) {
        currentVertex = rand() % tempNums.size();
        vector<int>::iterator it;
        it = tempNums.begin() + currentVertex;
        path[i] = tempNums.at(currentVertex);
        tempNums.erase(it);

    }
}

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
    cost += cityMatrix[path[citiesNum]][path[0]];
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
double startingT(int* path,int citiesNum,int** cityMatrix) {
    double temperature;
    int temp = 0;
    int* tempPath = new int[citiesNum + 1];
    copyArray(tempPath, path, citiesNum + 1);
    double solution=0;
    int counter = 0;
    for (int i = 1; i < citiesNum; i++) {
        for (int j = temp; j < citiesNum; j++) {
            swap(tempPath[i], tempPath[j]);
            solution+=countCost(path,citiesNum,cityMatrix)-countCost(tempPath, citiesNum, cityMatrix);
            counter++;
            copyArray(tempPath, path, citiesNum + 1);

        }

        temp++;
    }
    delete[]tempPath;
    int tempSolution = solution / counter;
    if (tempSolution < 0)
        tempSolution *= -1;
    temperature = log(tempSolution / 0.92);
    return temperature;

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
        double startingTemperature;  
        double temperature;
        double stopTemperature = 0.00001;
        int bestCost, tempCost,currentCost;
        int* bestPath = new int[citiesNum + 1];
        int* currentPath = new int[citiesNum + 1];
        int* tempPath = new int[citiesNum + 1];
        double annealing = 0.90;
        double PRD;

        /* inicjalizacja sciezki poczatkowej */
        for (int i = 0; i <= citiesNum; i++) {
            if (i == citiesNum)
                currentPath[citiesNum] = 0; 
            else
                currentPath[i] = i; 
        }
        
        randomPath(currentPath, citiesNum);
        startingTemperature = startingT(currentPath,citiesNum,cityMatrix);
        temperature = startingTemperature;
        copyArray(bestPath, currentPath, citiesNum + 1);
        printPath(currentPath, citiesNum+1);
        cout << "Koszt sciezki poczatkowej : " << countCost(currentPath, citiesNum, cityMatrix) << endl;
        double deltaT;
        int L = citiesNum*citiesNum*citiesNum;
        int it = 0;
        clock_t start = clock();
        /*   tutaj zaczyna sie algorytm SA     */
        do{
            for (int i = 0; i < L; i++) {
                copyArray(tempPath, currentPath, citiesNum + 1);
                newPath(currentPath, citiesNum);
                if (countCost(tempPath, citiesNum, cityMatrix) < countCost(bestPath, citiesNum, cityMatrix)) {
                    copyArray(bestPath, tempPath, citiesNum + 1);
                    PRD = (double)countCost(bestPath, citiesNum, cityMatrix) / hamiltonOpt;
                    cout << it << " " << countCost(bestPath, citiesNum, cityMatrix) << " PRD = " << PRD * 100 << " %" << endl;
                    cout << "----------------" << endl;
                }
                deltaT = (double)countCost(tempPath, citiesNum, cityMatrix) -(double) countCost(currentPath, citiesNum, cityMatrix);
                if (deltaT < 0)
                    copyArray(currentPath, tempPath, citiesNum + 1);
                else {
                    double randomX = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
                    if (randomX < eulerFun(deltaT, temperature))
                        copyArray(currentPath, tempPath, citiesNum + 1);
                }   
               // it++;
            }
            if (temperature <= 0.01*startingTemperature)
                annealing = 0.96;
           temperature =  temperatureFunction(temperature, annealing);
           it++;
        } while (temperature >= stopTemperature);
              
        
        
        /*   tutaj konczy  sie algorytm SA      */

        clock_t stop = clock();
        double  elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
        cout << elapsed << " [s]" << endl;

        PRD = (double)countCost(bestPath, citiesNum, cityMatrix) / hamiltonOpt;
        cout << "PRD = " << PRD * 100 << " %" << endl;
        printPath(bestPath, citiesNum + 1);

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
