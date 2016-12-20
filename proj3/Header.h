//
//  Header.h
//  Project 3
//
//  Created by Jiangshan Jing on 11/22/14.
//  Copyright (c) 2014 Isaac. All rights reserved.
//

#ifndef Project_3_Header_h
#define Project_3_Header_h

typedef struct Nodes {
    int X;
    int Y;
    int Previous;
    int Distance;
} Nodes;

typedef struct Next {
    int Number;
    struct Next * NextPosition;
} NextPosition;

struct list{
    Nodes * Position;
    NextPosition * NextNodes;
};

/*struct heap{
    int Node;
    int Distance;
};*/

void ReadMapInfo(const char * FileName, int *NumberOfNodes, int *NumberOfPaths);
void ReadMap(const char * FileName, struct list Map[], int NumberOfNodes, int NumberOfPaths);
void ReadQuery(FILE * fh, int * Query[]);
void Dijkstra(struct list Map[], int Start, int End, int NumberOfNodes, int NumberOfPaths);
int MinDistance(struct list Map[], int Unvisited[], int NumberOfUnvisited);
//void Downward_heapify(int NumberOfUnvisited, int *MinHeap[], int i);
#endif
