#include <iostream>
#include <iomanip>
#include <vector>
typedef unsigned char byte;

byte gmul2(byte x){
    return (x & 0x80) ? (x << 1) ^ 0x1b : (x << 1);   
}

//MixColumns for a single column
void mixColumn(byte * col){
    byte a[4];
    for (int i=0; i<4; i++) a[i]=col[i];
    col[0]= gmul2(a[0]) ^ (gmul2(a[1]) ^ a[1]) ^ a[2] ^ a[3];
    col[1]= a[0] ^ gmul2(a[1]) ^ (gmul2(a[2]) ^ a[2]) ^ a[3];
    col[2]= a[0] ^ a[1] ^ gmul2(a[2]) ^ (gmul2(a[3]) ^ a[3]);
    col[3]= (gmul2(a[0]) ^ a[0]) ^ a[1] ^ a[2] ^ gmul2(a[3]);
}

void shiftRows (byte*state){
    byte tmp;
    
    //Row 1: shift left 1
    tmp= state[1]; state[1]=state[5]; state[5]=state[9]; state[9]=state[13]; state[13]=tmp;

    //Row 2: shift left 2
    tmp= state[2]; state[2]=state[10]; state[10]=tmp;
    tmp= state[6]; state[6]=state[14]; state[14]=tmp;
    
    //row 3: shift left 3
    tmp= state[15]; state[15]=state[11]; state[11]=state[7]; state[7]=state[3]; state[3]=tmp;
}

void printStateDiff(byte*s1, byte*s2){
    std::cout << "State Difference(XOR) Matrix:\n";
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            std::cout << std::hex << std::setw(2) << std::setfill('0') <<(int)(s1[i+4*j]) << " ";
        }
        std::cout << std::endl;
    }
}

int main(){
    byte p1[16]={0x00};
    byte p2[16]={0x00}; p2[0]=0x01;  

    //Round 1: MixColumns + ShiftRows
    for (int i=0; i<4; i++) {mixColumn(&p1[i*4]); mixColumn(&p2[i*4]);}
    shiftRows(p1); shiftRows(p2);

    //Round 2: MixColumns
    for (int i=0; i<4; i++) {mixColumn(&p1[i*4]); mixColumn(&p2[i*4]);}

    //Final verification
    printStateDiff(p1, p2);

    std::cout << "/nObservation: In every column, you will find two identical hex values!" << std::endl;
    return 0;
}