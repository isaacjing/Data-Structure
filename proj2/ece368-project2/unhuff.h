//
//  header.h
//  proj2
//
//  Created by Jiangshan Jing on 10/10/14.
//  Copyright (c) 2014 Isaac. All rights reserved.
//

#ifndef proj2_huff_h
#define proj2_huff_h

struct list {
    int ASCII;
    long count;
    unsigned long key;
    int length;
};

typedef struct leaf {
    int ASCII;
    long Count;
    int Connected;              //if it is in the forest, value will be 0; if it has already been connected, value will be 1;
    //for top of the connection node, value will be 0; for lower level of connection node, value will be -1;
    struct leaf *left;
    struct leaf *right;
} HuffNode;


void CountASCII(struct list[256], const char * FileName);
void BuildHeader(struct list[256], const char * FileName);
int SortArray(struct list[256]);
HuffNode * BuildTree(struct list ASCII[256], int);
HuffNode * BuildLeaf(int value, long Count);
HuffNode * ConnectLeaves(HuffNode * left, HuffNode * right);
void UpdateASCII(struct list[256], HuffNode * Head, char * Key);
void SecondSortArray(struct list[256]);
void Compressing(struct list[256], const char * FileName);

FILE * ReadHeader(struct list[256], const char * FileName);
void Decompressing(HuffNode * Head, FILE *, const char * FileName);
#endif
