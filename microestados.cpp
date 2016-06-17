/////////////////////////////////////////////////////////////
// Computes microstates of coordination complexes of d-block
//  metals.
//
// By Gerardo Gálvez.
//
// Thanks to Jorge Gálvez for guiding me through all the
//  chemistry behind this.
/////////////////////////////////////////////////////////////

#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
using namespace std;

//Number of ways in which electrons can be arragned in 5 orbitals
//  taking into consideration the spin.
const int NUM_COMBINACIONES_D[10] = {10, 45, 120, 210, 252, 210, 120, 45, 10, 1};

//Quantum magnetic number of each orbital
const int ORBITALES_D[5] = {2, 1, 0, -1, -2};

//Amount of orbitals in a d configuration
const int NUM_ORBITALES = 5;

//Object that represents a way in which electrons can be arranged in
//  5 orbitals.
//
// 3 electrons, [1][1][1][0][0] will be used as example in the comments
struct Combinacion{
    //Number of electrons in each orbital
    int electrones[NUM_ORBITALES];

    //Magnetic quantum number
    int ML;

    //Default constructor
    Combinacion();

    //Description for this method can be found before its implementation
    void setElectrones(int A, int B, int C, int D, int E);

    //Number of orbitals that hold only 1 electron (used to get the total spin)
    int ones;
};

//Sets the number of electrons in each orbital and counts the number
//  of orbitals that hold 1 electron
void Combinacion::setElectrones(int A, int B, int C, int D, int E){
    ones = 0;
    electrones[0]=A;
    electrones[1]=B;
    electrones[2]=C;
    electrones[3]=D;
    electrones[4]=E;
    for(int i=0; i<NUM_ORBITALES; i++){
        if(electrones[i]==1)
            ones++;
    }
}

//Set all 5 electrons to 0
Combinacion::Combinacion(){
    for(int i=0; i<NUM_ORBITALES; i++)
        electrones[i]=0;
}

//Gets the number of ways n given electrons can be
//  arranged in 5 orbitals without taking into
//  consideration their permutations.
//
//Ex. 1 electron -> 1 way: [1][0][0][0][0] (And its permutations)
//    2 electrons -> 2 ways: [1][1][0][0][0] AND [2][0][0][0][0] (And its permutations)
//
//These "ways" are obtained in setCombinaciones(Combinacion arrCombinacion[], int n)
int getNumCombinaciones(int n){
    int x;
    switch(n){
        case 1:
        case 9:
        case 10:
            x=1;
            break;
        case 2:
        case 3:
        case 7:
        case 8:
            x=2;
            break;
        case 4:
        case 5:
        case 6:
            x=3;
            break;
    }

    return x;
}

//Sets the possible ways in which given n electrons can be arranged
//  in 5 orbitals not taking into consideration their permutations.
void setCombinaciones(Combinacion arrCombinacion[], int n){
    switch(n){
        case 1:
            arrCombinacion[0].setElectrones(0,0,0,0,1);
            break;
        case 2:
            arrCombinacion[0].setElectrones(0,0,0,1,1);
            arrCombinacion[1].setElectrones(0,0,0,0,2);
            break;
        case 3:
            arrCombinacion[0].setElectrones(0,0,1,1,1);
            arrCombinacion[1].setElectrones(0,0,0,1,2);
            break;
        case 4:
            arrCombinacion[0].setElectrones(0,1,1,1,1);
            arrCombinacion[1].setElectrones(0,0,1,1,2);
            arrCombinacion[2].setElectrones(0,0,0,2,2);
            break;
        case 5:
            arrCombinacion[0].setElectrones(1,1,1,1,1);
            arrCombinacion[1].setElectrones(0,1,1,1,2);
            arrCombinacion[2].setElectrones(0,0,1,2,2);
            break;
        case 6:
            arrCombinacion[0].setElectrones(1,1,1,1,2);
            arrCombinacion[1].setElectrones(0,1,1,2,2);
            arrCombinacion[2].setElectrones(0,0,2,2,2);
            break;
        case 7:
            arrCombinacion[0].setElectrones(1,1,1,2,2);
            arrCombinacion[1].setElectrones(0,1,2,2,2);
            break;
        case 8:
            arrCombinacion[0].setElectrones(1,1,2,2,2);
            arrCombinacion[1].setElectrones(0,2,2,2,2);
            break;
        case 9:
            arrCombinacion[0].setElectrones(1,2,2,2,2);
            break;
        case 10:
            arrCombinacion[0].setElectrones(2,2,2,2,2);
            break;
    }
}


// Returns the binary representation of a positive base 10 number in an array.
// Ex. 6 -> [1][1][0]
void bin(int n, int arrOnes[], int ones){
    int i;
    int pos = 0;
    for (i = 1 << ones-1; i > 0; i = i / 2){
        arrOnes[pos]=(n & i)? 1:0;
        pos++;
    }
}

void run(ofstream &miArchivo){
    //Outputs permutations for 1...10 electrons
    for (int electrones=1; electrones<=10; electrones++){
        miArchivo << "Numero de electrones: " << electrones << endl;
        miArchivo << "Numero de combinaciones: " << NUM_COMBINACIONES_D[electrones-1] << endl;

        //Get number of ways in which the electrons can be arranged
        int posiblesCombinaciones = getNumCombinaciones(electrones);

        //Will the ways in which n electrons can be arranged (3 is the max number of ways)
        Combinacion arrCombinacion[3];

        //Save the different ways in which the electrons can be arranged
        setCombinaciones(arrCombinacion, electrones);

        for (int i=0; i<posiblesCombinaciones; i++){
            //For each permutation...
            do{
                //If there are orbitals that hold 1 electron... (spin needs to be computed, else it will be 0)
                if(arrCombinacion[i].ones>0){
                    /*
                    [1][1][1][0][0], each electron can have positive or negative spin, therefore,
                      the "spin" can be arranged in 8 different ways (2^(number of orbitals
                      with 1 electron))
                    
                    USING [1][1][1][0][0]
                    ---------------------
                    1. [-1][-1][-1][0][0]
                    2. [-1][-1][+1][0][0]
                    3. [-1][+1][-1][0][0]
                    4. [-1][+1][+1][0][0]
                    5. [+1][-1][-1][0][0]
                    6. [+1][-1][+1][0][0]
                    7. [-1][+1][+1][0][0]
                    8. [+1][+1][+1][0][0]
                    */
                    for(int n=0; n<pow(2, arrCombinacion[i].ones); n++){
                        //Magnetic quantum number for this permutation
                        int ML = 0;
                        int arrOnes[arrCombinacion[i].ones];

                        /*
                        Permutations to compute the spin (the ones stated above) are obtained using
                          the binary representation of decimal numbers. It tells if an electron's spin is
                          positive or negative (1 or 0)
                        
                         Ex.
                         [-1][-1][-1][0][0] follows the binary representation of 0 ([0][0][0])
                         [-1][-1][+1][0][0] follows the binary representation of 1 ([0][0][1])
                         and so on, where [-1] -> [0] and [+1] -> [1]
                        */
                        bin(n, arrOnes, arrCombinacion[i].ones);

                        int MS = 0;
                        int ones = 0;
                        for(int j=0; j<NUM_ORBITALES; j++){
                            if(arrCombinacion[i].electrones[j]==1){
                                if(arrOnes[ones]==1){
                                    miArchivo << "[+" << arrCombinacion[i].electrones[j] << ']';
                                }
                                else{
                                    miArchivo << "[-" << arrCombinacion[i].electrones[j] << ']';
                                }
                                ones++;
                            }
                            else{
                                miArchivo << '[' << arrCombinacion[i].electrones[j] << ']';
                            }
                            ML+=(arrCombinacion[i].electrones[j]*ORBITALES_D[j]);
                        }

                        //Compute total spin
                        //
                        //[1][1][1][0][0] -> spin: 3/2
                        for(int iter=0; iter<arrCombinacion[i].ones; iter++)
                            (arrOnes[iter] == 1)? MS++ : MS--;

                        //Set the total magnetic moment
                        arrCombinacion[i].ML = ML;

                        miArchivo << " ML: " << arrCombinacion[i].ML;
                        
                        if(MS%2==0)
                            miArchivo << " MS: " << MS/2 << endl;
                        else
                            miArchivo << " MS: " << MS << "/2" << endl;
                    }
                }
                //There are no orbitals that hold 1 electron, spin will be 0
                else{
                    //Magnetic quantum number for this permutation
                    int ML = 0;

                    //Spin for this permutation, it will remain in 0 because having
                    //  2 or 0 electrons in an orbital sets the spin to 0
                    int MS = 0;

                    for(int j=0; j<NUM_ORBITALES; j++){
                        miArchivo << '[' << arrCombinacion[i].electrones[j] << ']';

                        //Compute total magnetic moment multiplying number of electrons
                        //  times the respective magnetic number
                        ML+=(arrCombinacion[i].electrones[j]*ORBITALES_D[j]);
                    }

                    //Set the total magnetic moment
                    arrCombinacion[i].ML = ML;

                    miArchivo << " ML: " << arrCombinacion[i].ML << " MS: " << MS << endl;
                }
            }
            while(next_permutation(arrCombinacion[i].electrones, arrCombinacion[i].electrones+NUM_ORBITALES));
        }
        miArchivo << endl;
    }
}

int main(){
    ofstream miArchivo;
    miArchivo.open("MicroestadosElectronicos_D5.txt");
    run(miArchivo);
    miArchivo.close();
    return 0;
}
