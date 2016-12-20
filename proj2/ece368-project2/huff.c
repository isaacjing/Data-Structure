//
//  huff.c
//  proj2
//
//  Created by Jiangshan Jing on 10/10/14.
//  Copyright (c) 2014 Isaac. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <math.h>

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
    int TotalChar = 0;
    unsigned long int Key = 0;
    HuffNode * Head;
    const char * FILENAME = argv[1];
    cstart = (double) clock();
    CountASCII(ASCII, FILENAME);
    BuildHeader(ASCII, FILENAME);
    TotalChar = SortArray(ASCII);
    Head = BuildTree(ASCII, TotalChar);
    SecondSortArray(ASCII);
    UpdateASCII(ASCII, Head, Key, 0);
    Compressing(ASCII, FILENAME);
    
    cend = (double) clock();
    printf("Compressing Time (sec): %f\n", (cend - cstart)/CLOCKS_PER_SEC);
    return 0;
}

void CountASCII(struct list ASCII[256], const char * FILENAME)
{
    FILE * fh;
    int i = 0;
    int c = 1;
	int temp = 0;
    char value;
    //Create and initialize the ASCII array:
    for(i = 0; i < 256; i++)
    {
        ASCII[i].count = 0;
        ASCII[i].ASCII = i;
        ASCII[i].key = 0;
        ASCII[i].length = 0;
    }
    //Open and read the external file:
    fh = fopen(FILENAME, "r");
    if (fh!= NULL) {
        //Update the corresponding ASCII counter while reading the file
        while(c == 1)
        {
            c = fscanf(fh, "%c", &value);
			temp = value;
            ASCII[temp].count = ASCII[temp].count + 1;
        }
        ASCII[19].count = 1;
        fclose(fh);
    }
    else
    {
        printf("ERROR OPENING THE INPUT FILE! \n");
        fclose(fh);
    }
}

void BuildHeader(struct list ASCII[256], const char * FILENAME)
{
    int i = 0;
    char * NEWNAME = strdup(FILENAME);
    strcat(NEWNAME, ".huff");
    FILE * fh = fopen(NEWNAME, "w");
    for(i = 0; i < 256; i++){
        fprintf(fh, "%ld\n", ASCII[i].count);
    }
    fclose(fh);
    free(NEWNAME);
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
void SecondSortArray(struct list ASCII[256])
{
    int Total = 256;
    int ind2 = 0;
    int ind3 = 0;
    struct list temp;
    for (ind2 = 0 ; ind2 < (Total - 1); ind2++)
    {
        for (ind3 = 0 ; ind3 < Total - ind2 - 1; ind3++)
        {
            if (ASCII[ind3].ASCII > ASCII[ind3+1].ASCII)
            {
                temp = ASCII[ind3];
                ASCII[ind3] = ASCII[ind3 + 1];
                ASCII[ind3 + 1] = temp;
            }
        }
    }
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

void UpdateASCII(struct list ASCII[256], HuffNode * Head, unsigned long int Key, int length)
{
    //char * TempKey;
    if (Head->left == NULL && Head->right == NULL)
    {
        //Leaf Node
        ASCII[Head->ASCII].key = Key;
        ASCII[Head->ASCII].length = length;
        return;
    }
    else
    {
        unsigned long int TempKey;
        TempKey = Key;                   //Temporarily store the Key value
        Key =  Key * 10;
        length++;
        UpdateASCII(ASCII, Head->left, Key, length);
        Key = TempKey * 10 + 1;
        UpdateASCII(ASCII, Head->right, Key, length);
        //free(TempKey);
    }
}

void Compressing(struct list ASCII[256], const char * FILENAME)
{
    int temp = 1;
    int NotFull = 1;
    unsigned long int BitsTemp = 0;
    unsigned long int ByteBuffer = 0;                 //The temp byte that will be written to the disk
    char CharValue;
    int SIZE = 8;
    unsigned long int IntValue = 0;
    unsigned long int Tempvalue;
    int length = 0;                 //length of current key
    int Remain = SIZE;                 //bits remaining in current byte
    char * NEWNAME = strdup(FILENAME);
    int count = 0;
	int temp2 = 0;
    strcat(NEWNAME, ".huff");
    FILE * CompressedFile = fopen(NEWNAME, "a");
    FILE * OriginalFile = fopen(FILENAME, "r");
    long int shift[19] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000, 1000000000000000, 10000000000000000, 100000000000000000, 1000000000000000000};
    while(temp == 1)
    {
        if(NotFull)
        {
            count ++;
            temp = fscanf(OriginalFile, "%c", &CharValue);
			temp2 = CharValue;
            IntValue = ASCII[temp2].key;
            length = ASCII[temp2].length;
        }
        if(temp == -1)
        {
            IntValue = ASCII[19].key;
            length = ASCII[19].length;
        }
        if(length < Remain)
        {
            Tempvalue = IntValue * shift[Remain - length];
            ByteBuffer = ByteBuffer + Tempvalue;
            Remain = Remain - length;
            NotFull = 1;
        }
        else if(length == Remain)
        {
            ByteBuffer = ByteBuffer + IntValue;
            ByteBuffer = BinaryToDecimal(ByteBuffer);
            fprintf(CompressedFile, "%c", ASCII[ByteBuffer].ASCII);
            ByteBuffer = 0;
            Remain = SIZE;
            NotFull = 1;
        }
        else                      //length > Remain length = 11, Remain = 2
        {
            BitsTemp = IntValue % shift[length - Remain];
            ByteBuffer = ByteBuffer + (IntValue / shift[length - Remain]);
            ByteBuffer = BinaryToDecimal(ByteBuffer);
            fprintf(CompressedFile, "%c", ASCII[ByteBuffer].ASCII);
            ByteBuffer = 0;
            IntValue = BitsTemp;
            length = length - Remain;//17
            Remain = 8;
            NotFull = 0;
        }
    }
    while(IntValue){
        if(length < Remain)
        {
            Tempvalue = IntValue * shift[Remain - length];
            ByteBuffer = ByteBuffer + Tempvalue;
            ByteBuffer = BinaryToDecimal(ByteBuffer);
            fprintf(CompressedFile, "%c", ASCII[ByteBuffer].ASCII);
            IntValue = 0;
        }
        else if(length == Remain)
        {
            ByteBuffer = ByteBuffer + IntValue;
            ByteBuffer = BinaryToDecimal(ByteBuffer);
            fprintf(CompressedFile, "%c", ASCII[ByteBuffer].ASCII);
            IntValue = 0;
        }
        else                      //length > Remain length = 11, Remain = 2
        {
            BitsTemp = IntValue % shift[length - Remain];
            ByteBuffer = ByteBuffer + (IntValue / shift[length - Remain]);
            ByteBuffer = BinaryToDecimal(ByteBuffer);
            fprintf(CompressedFile, "%c", ASCII[ByteBuffer].ASCII);
            ByteBuffer = 0;
            IntValue = BitsTemp;
            length = length - Remain;//17
            Remain = 8;
            NotFull = 0;
        }

    }
    ByteBuffer = BinaryToDecimal(ByteBuffer);
    fprintf(CompressedFile, "%c", ASCII[ByteBuffer].ASCII);

    fclose(CompressedFile);
    fclose(OriginalFile);
}

int BinaryToDecimal(unsigned long int input)  // This function converts a decimal to binary
{
    int decimal = 0;
    int i = 0;
    int rem;
    while (input != 0)
    {
        rem = input % 10;
        input = input / 10;
        decimal = decimal + rem * pow(2,i);
        ++i;
    }
    return decimal;
}

int CountDigits(int input)
{
    int count = 0;
    while(input!=0)
    {
        input/=10;
        ++count;
    }
    return count;
}
