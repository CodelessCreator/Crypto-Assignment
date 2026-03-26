#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
#include <numeric>

//Function to genenrate DDT
std::vector <std::vector<int>> generateDDT(const std::vector<int> & sbox) {
    int size=sbox.size();
    std::vector<std::vector<int>> ddt(size, std::vector<int>(size, 0));

    for(int deltaX=0; deltaX<size; ++deltaX) {
        for(int x=0; x<size; ++x) {
            int y1=sbox[x];
            int y2=sbox[x^deltaX];
            int deltaY=y1^y2;
            ddt[deltaX][deltaY]++;
        }
    }
    return ddt;
}

//Function that reports its differential uniformity
int getDifferentialUniformity(const std::vector<std::vector<int>> & ddt){
    int maxVal=0;
    int size=ddt.size();
    for(int i=0; i<size; ++i){
        for(int j=0; j<size; ++j){
            if(ddt[i][j]>maxVal){
                maxVal=ddt[i][j];
            }
        }
    }
    return maxVal;
}

int main(){

    //Generate a random sbox
    std::vector<int> sbox (16);
    std::iota(sbox.begin(), sbox.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(sbox.begin(), sbox.end(), g);

    //Compute the DDT
    auto ddt=generateDDT(sbox);
    int uniformity=getDifferentialUniformity(ddt);

    //Display results
    std::cout<<"Random 4-bit sbox : \n";
    for(int x:sbox) std::cout << x << " ";
    std::cout << "\n DDT : \n";
    for(const auto & row:ddt){
        for(int val:row){
            std::cout << std::setw(3) << val;
        }
        std::cout << "\n";
    }
    std::cout << "/nDifferential Uniformity : " << uniformity << std::endl;
    return 0;
}