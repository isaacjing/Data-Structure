//
//  unhuff.c
//  proj2
//
//  Created by Jiangshan/Users/jiangshanjing/Desktop/unhuff.c Jing on 10/15/14.
//  Copyright (c) 2014 Isaac. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unhuff.h"

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000000
#endif

int main(int argc, const char * argv[]) {
    double cstart, cend;                //for the timer
    struct list ASCII[256];
    cstart = (double) clock();
    int TotalChar = 0;
    HuffNode * Head;
    //argv[1] = "/tmp/tet1.txt";
    const char * FILENAME = argv[1];
    FILE * fh = ReadHeader(ASCII, FILENAME);
    TotalChar = SortArray(ASCII);
    Head = BuildTree(ASCII, TotalChar);

    Decompressing(Head, fh, FILENAME);
    
    cend = (double) clock();
    printf("Decompressing Time (sec): %f\n", (cend - cstart)/CLOCKS_PER_SEC);
    return 0;
}

FILE * ReadHeader(struct list ASCII[256], const char * FileName)
{
    FILE * fh = fopen(FileName, "r");
    int i = 0;
    int c;
    long temp;
    if (fh!= NULL) {
        //Update the corresponding ASCII counter while reading the file
        while(i < 256)
        {
            c = fscanf(fh, "%li", &temp);
            ASCII[i].count = temp;
            ASCII[i].ASCII = i;
            i++;
        }
        i = 0;
    }
    else
    {
        printf("/n/nERROR OPENING THE COMPRESSED FILE!/n/n");
        fclose(fh);
    }
    return(fh);
}

HuffNode * BuildTree(struct list ASCII[256], int Counter)
{
    int i = 0;
    HuffNode * Temp = malloc(sizeof(HuffNode));
    HuffNode * Least = NULL;
    HuffNode * Special = NULL;
    int LeastIndex = 0;
    HuffNode * SecondLeast = NULL;
    int SecondLeastIndex = 0;
    int CopyCounter = Counter;          //copy of total number of nodes
    int ConnectionNodeIndex = Counter;  //starting location of connection nodes
    Temp = malloc(sizeof(* Temp) * (2+Counter*2));
    Temp[1+Counter*2].Count = 2147483647;
    Temp[1+Counter*2].Connected = 1;
    while(i < Counter)
    {
        Temp[i] = *BuildLeaf(ASCII[i].ASCII, ASCII[i].count);
        Temp[i].Connected = 0;
        i++;
    }
    while(i < Counter * 2)
    {
        Temp[i] = *BuildLeaf(300, 0);
        Temp[i].Connected = -1;
        i++;
    }
    i = 0;
    Least = &Temp[1+CopyCounter*2];
    SecondLeast = &Temp[1+CopyCounter*2];
    while(Counter > 1)
    {
        //Find 2 nodes with smallest frequencies
        
        while(i < ConnectionNodeIndex)
        {
            if(Temp[i].Count < SecondLeast->Count && Temp[i].Connected == 0)
            {
                if(Temp[i].Count < Least->Count)
                {
                    if(Least->Count != 2147483647){
                        SecondLeast = &Temp[LeastIndex];
                        SecondLeastIndex = LeastIndex;
                        Least = &Temp[i];
                        LeastIndex = i;
                    }
                    Least = &Temp[i];
                    LeastIndex = i;
                }
                else{
                    SecondLeast = &Temp[i];
                    SecondLeastIndex = i;
                }
            }
            i++;
        }
        //combine those two nodes, put them in the SecondLeast position
        Special = ConnectLeaves(SecondLeast, Least);
        Temp[ConnectionNodeIndex].Count = Special->Count;
        Temp[ConnectionNodeIndex].left = Special->left;
        Temp[ConnectionNodeIndex].right = Special->right;
        Temp[ConnectionNodeIndex].ASCII = -1;
        
        Temp[ConnectionNodeIndex].Connected = 0;
        ConnectionNodeIndex++;
        
        //Change the frequence of the LeastNode to the biggest integers
        Temp[LeastIndex].Connected = 1;
        Temp[SecondLeastIndex].Connected = 1;
        //Reduce problem size by 1
        Counter--;
        if(Counter == 2){
            int temp0 = 0;
            temp0++;
        }
        i = 0;
        //        Temp[1+CopyCounter*2].Count = 2147483647;
        //        Temp[1+CopyCounter*2].Connected = -5;
        Least = &Temp[1+CopyCounter*2];
        SecondLeast = &Temp[1+CopyCounter*2];
        SecondLeastIndex = 1+CopyCounter*2;
        LeastIndex = 1+CopyCounter*2;
    }
    return &Temp[ConnectionNodeIndex - 1];
}


int SortArray(struct list ASCII[256])
{
    int Total = 256;
    int ind2 = 0;
    int ind3 = 0;
    struct list temp;
    for (ind2 = 0 ; ind2 < (Total - 1); ind2++)
    {
        for (ind3 = 0 ; ind3 < Total - ind2 - 1; ind3++)
        {
            if (ASCII[ind3].count < ASCII[ind3+1].count)
            {
                temp = ASCII[ind3];
                ASCII[ind3] = ASCII[ind3 + 1];
                ASCII[ind3 + 1] = temp;
            }
        }
    }
    ind2 = 0;
    while(ind2 < 256)
    {
        if(ASCII[ind2].count == 0){
            Total--;
        }
        ind2++;
    }
    return Total;
}
HuffNode * BuildLeaf(int value, long Count)
{
    HuffNode * New = malloc(sizeof(HuffNode));
    New->ASCII = value;
    New->Count = Count;
    New->left = NULL;
    New->right = NULL;
    return New;
}

HuffNode * ConnectLeaves(HuffNode * left, HuffNode * right)
{
    HuffNode * New = malloc(sizeof(HuffNode));
    New->Count = left->Count + right->Count;
    New->ASCII = -1;
    New->left = left;
    New->right = right;
    return New;
}

void Decompressing(HuffNode * Head, FILE * CompressedFile, const char * FILENAME)
{
    char * NEWNAME = strdup(FILENAME);
    strcat(NEWNAME, ".unhuff");
    FILE * DecompressedFile = fopen(NEWNAME, "w");
    int terminate = 0;
    int temp = 0;
    HuffNode * CopyHead = Head;
    int CurrentPosition = 8;
    int KeyBuffer = 0;
    char ByteBuffer;
    int Filter[9] = {0, 1, 0b10, 0b100, 0b1000, 0b10000, 0b100000, 0b1000000, 0b10000000};
    temp = fscanf(CompressedFile, "%c", &ByteBuffer);
    while(!terminate){
        //get a byte from the file
        temp = fscanf(CompressedFile, "%c", &ByteBuffer);
        
        while(CurrentPosition >= 1)
        {
            //get a bit from the byte
            temp = Filter[CurrentPosition] & ByteBuffer;
            
            //Trace the tree to find the value
            if(temp != 0)           //Move right
            {
                Head = Head->right;
            }
            else                    //Move left
            {
                Head = Head->left;
            }
            CurrentPosition--;      //Decrement the bits left
            KeyBuffer++;
            if(Head->ASCII > 0){
                if(Head->ASCII != 19){
                    temp = fprintf(DecompressedFile, "%c", (Head->ASCII));
                    Head = CopyHead;
                }
                else{
                    terminate = 1;
                    CurrentPosition = -100;
                }
            }
            
        }
        CurrentPosition = 8;        //Ready to get a new byte!
    }
    fclose(CompressedFile);
    fclose(DecompressedFile);
}