#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//Sypher004 permutation
int permutation[]={0,4,8,6,19,2,7,14,3,11,2,13};

int main(){
    ofstream lpFile("sypher004_2round.lp");

//Minimize active s-boxes
lpFile << "Minimize\n obj : ";
for (int r=0; r<2; ++r){
    for (int s=0; s<4; ++s){
        lpFile << "a_" << r << "_" << s << ((r==1 && s==3) ? "" : "+");
    }
}

//Constraints
lpFile << "\n\nSubject to\n";
int count=1;
for (int r=0; r<2; ++r){
    for (int s=0; s<4; ++s){
        //Input bits for s-box
        lpFile << "c" << count++ << ": ";
        for(int b=0; b<4; b++) lpFile << "x_" << r << "_" << (s*4+b) << "+";
        for(int b=0; b<4; b++) lpFile << "y_" << r << "_" << (s*4+b) << "-5 a_" <<  r << "_" << s <<" >=0\n";

        //Each bit must be <= the active variable
        for(int b=0; b<4; b++){
            lpFile << " c" << count++ << ": a_" << r << "_" << s << " - x_" << r << "_" << (s*4+b) << " >= 0\n";
            lpFile << " c" << count++ << ": a_" << r << "_" << s << " - y_" << r << "_" << (s*4+b) << " >= 0\n";
        }
    }

    //Connect round r's y to round r+1's x
    if (r<1){
        for (int i=0; i<16; i++){
            lpFile << " c" << count++ << ": y_" << r << "_" << i << " - x_" << "_" << permutation[i] << " 0\n";

        }
    }
}

//Atleast one input bit must be 1
lpFile << "non-zero: ";
for (int i=0; i<16; i++) lpFile << "x_0_" << i << (i==15 ? "" : "+");
lpFile << ">=1\n";

//Variable definitions
lpFile << "/nBinary/n";
for (int r=0; r<2; r++){
    for (int i=0; i<16; i++) lpFile << "x_" << r << "_" << i << "y_" << r << "_" << i;
    for (int s=0; s<4; s++) lpFile<< "a_" << r << "_" << s;
}

lpFile << "/nEnd/n";
lpFile.close();
cout << "MILP model 'sypher004_2round.lp' generated successfully!" << endl;
return 0;
}