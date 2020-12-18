#include <windows.h> 
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <array>
#include <random>
#include "time.h"

using namespace std;

const int popsize = 10;
const double good = 0.9;
const int length = 7;
const double mutProb = 0.2;
const int maxSim = 1000;


array<string,popsize> initPop(){
    array<string,popsize> pop;
    srand (time(NULL));
    for(int i=0; i<popsize; i++){
        string s = "";
        for (int j=0; j<length; j++){
            int r = round(((double) rand() / (RAND_MAX)));
            r==0? s.append("0") : s.append("1");
        }
        pop.at(i)=s;
    }
    return pop;
}

int fitness(string element){
    return count(element.begin(), element.end(), '1');
}

int popFitness(array<string,popsize> pop){
    int tot = 0;
    for(int i = 0; i < popsize; i++){
        tot += fitness(pop.at(i));
    }
    return tot;
}

array<string,popsize> generatePop(array<string,popsize> pop){
    array<string,popsize> newPop;
    int totF = popFitness(pop);
    for(int i=0; i<popsize; i++){
        int S = rand() % totF;
        int P = 0;
        int j = 0;
        P += fitness(pop.at(j));
        while (P < S && j < popsize-1){
            j++;
            P += fitness(pop.at(j));
        }
        newPop.at(i) = pop.at(j);
    }
    return newPop;
}

array<string,popsize> crossover(array<string,popsize> pop){
    int i = 0;
    string s1, s2;
    string s1_1, s1_2, s2_1, s2_2;
    srand (time(NULL));
    while(i < popsize){
        s1 = pop.at(i);
        i++;
        if(i < popsize){
            int p = rand() % length;
            s2 = pop.at(i);
            s1_1 = s1.substr(0,p);
            s2_1 = s2.substr(0,p);
            s1_2 = s1.substr(p,length-p);
            s2_2 = s2.substr(p,length-p);
            s1 = s1_1;
            s2 = s2_1;
            s1.append(s2_2);
            s2.append(s1_2);
            pop.at(i-1) = s1;
            pop.at(i) = s2;
            i++;
        }
    }
    return pop;
}

array<string,popsize> mutation(array<string,popsize> pop){
    for(int i = 0; i <popsize; i++){
        for(int j = 0; j<length; j++){
            int p = rand() % 100;
            if (p <= mutProb*100){
                if(pop.at(i).at(j) == '0'){
                    pop.at(i).at(j) = '1';
                }
                else{
                    pop.at(i).at(j) = '0';
                }
            }
        }
    }
    return pop;
}

void printPop(array<string,popsize> pop, int gen){
    cout<<"Generation " << gen << "("<< popFitness(pop) << ")\n";
    for(int i = 0; i<popsize; i++){
        cout << pop.at(i) << "\n";
    }
    cout<<"\n";
}

int main(){
    array<string, popsize> population = initPop();
    array<string, popsize> newPop;
    int gen = 1;
    printPop(population, gen);
    while(popFitness(population) < good*length*popsize && gen < maxSim){
        gen++;
        newPop = generatePop(population);
        newPop = crossover(newPop);
        newPop = mutation(newPop);
        int fNew = popFitness(newPop);
        int f = popFitness(population);
        if(fNew >= f){
            Sleep(50);
            population = newPop;
            printPop(population,gen);
        }
        else{
            Sleep(50);
            cout<<"Generation "<<gen<<" ABORTED\n\n";
        }
    }
    cout<<"Last Generation: "<<gen<<", fitness: "<<fixed<<setprecision(2) <<(double)popFitness(population)*100/(length*popsize)<<"%\n";
}