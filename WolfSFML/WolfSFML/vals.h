#pragma once
int poolSize = 500;
int layers = 4; // layers inc input + output.
int pick = 50;
int INPT = 6;
int LYR = 6;
int LYR2 = 6;
int OUT = 4;
int champion = 0; 
int Width = 700;
int Height = 500;


float results[2000]; // takes score
float v_matrix[4][5]; 
float w_matrix[2000][3][8][8];//attempts//layers//nodes//inherits
float mutants[2000][3][8][8];
int survivors[1000];