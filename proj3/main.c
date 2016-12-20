//
//  main.c
//  Project 3 For ECE 368
//  Version 3.0
//  Change log:
//  * Priority queue added
//
//  Created by Jiangshan Jing on 11/22/14.
//  Copyright (c) 2014 Isaac. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Header.h"
#define INFINITE 2100000000

int main(int argc, const char * argv[]) {
    const char * MapName = argv[1];
    int NumberOfNodes = 0;
    int NumberOfPaths = 0;
    int NumberOfQueries = 0;
    int count = 0;
    int temp;
    
    int Start = 0;
    int End = 0;
    
    ReadMapInfo(MapName, &NumberOfNodes, &NumberOfPaths);
    struct list Map[NumberOfNodes];
    ReadMap(MapName, Map, NumberOfNodes, NumberOfPaths);
    FILE * fh = fopen(argv[2], "r");
    temp = fscanf(fh, "%d", &NumberOfQueries);
    while(count++ < NumberOfQueries){
        //read the map file
        temp = fscanf(fh, "%d", &Start);
        temp = fscanf(fh, "%d", &End);
        //start!
        if(Start != End){
            Dijkstra(Map, Start, End, NumberOfNodes, NumberOfPaths);
        }else{
            printf("0\n%d\n", End);
        }
        
    }
    fclose(fh);
    return 0;
}

void ReadMapInfo(const char * FileName, int * NumberOfNodes, int * NumberOfPaths){
    FILE * fh;
    int temp;
    
    fh = fopen(FileName, "r");
    if(fh != NULL){
        //get information about the map
        temp = fscanf(fh, "%d", NumberOfNodes);
        temp = fscanf(fh, "%d", NumberOfPaths);
        fclose(fh);
    }else{
        printf("Error opening the input file! \n");
        fclose(fh);
    }
}

void ReadMap(const char * FileName, struct list Map[], int NumberOfNodes, int NumberOfPaths){
    int temp = 0;
    int count = 0;
    int value;
    int X;
    int Y;
    struct Next * NextP;
    FILE * fh = fopen(FileName, "r");
    temp = fscanf(fh, "%d", &value);    //NumberOfNodes and NumberOfPaths which we already know
    temp = fscanf(fh, "%d", &value);
    while(count < NumberOfNodes){
        Map[count].NextNodes = NULL;
        //Map[count].Position = NULL;
        count++;
    }
    count = 0;

    while(count < NumberOfNodes){
        temp = fscanf(fh, "%d", &value);
        temp = fscanf(fh, "%d", &X);
        temp = fscanf(fh, "%d", &Y);
        count++;
        
        Map[value].Position = malloc(sizeof(Nodes));
        Map[value].Position->X = X;
        Map[value].Position->Y = Y;
    }
    
    count = 0;
    while(count++ < NumberOfPaths){
        temp = fscanf(fh, "%d", &X);
        temp = fscanf(fh, "%d", &Y);
        NextP = Map[X].NextNodes;
        if(NextP == NULL){
            Map[X].NextNodes = malloc(sizeof(NextPosition));
            Map[X].NextNodes->Number = Y;
            Map[X].NextNodes->Distance = sqrt(pow(Map[X].Position->X - Map[Y].Position->X, 2) + pow(Map[X].Position->Y - Map[Y].Position->Y, 2));
            Map[X].NextNodes->NextPosition = NULL;
        }else{
            while(NextP->NextPosition != NULL){
                NextP = NextP->NextPosition;
            }
            NextP->NextPosition = malloc(sizeof(NextPosition));
            NextP->NextPosition->Number = Y;
            NextP->NextPosition->Distance = sqrt(pow(Map[X].Position->X - Map[Y].Position->X, 2) + pow(Map[X].Position->Y - Map[Y].Position->Y, 2));
            NextP->NextPosition->NextPosition = NULL;
        }
 
        NextP = Map[Y].NextNodes;
        if(NextP == NULL){
            Map[Y].NextNodes = malloc(sizeof(NextPosition));
            Map[Y].NextNodes->Number = X;
            Map[Y].NextNodes->Distance = sqrt(pow(Map[X].Position->X - Map[Y].Position->X, 2) + pow(Map[X].Position->Y - Map[Y].Position->Y, 2));
            Map[Y].NextNodes->NextPosition = NULL;
        }else{
            while(NextP->NextPosition != NULL){
                NextP = NextP->NextPosition;
            }
            NextP->NextPosition = malloc(sizeof(NextPosition));
            NextP->NextPosition->Number = X;
            NextP->NextPosition->Distance = sqrt(pow(Map[X].Position->X - Map[Y].Position->X, 2) + pow(Map[X].Position->Y - Map[Y].Position->Y, 2));
            NextP->NextPosition->NextPosition = NULL;
        }
    }
}

void Dijkstra(struct list Map[], int Start, int End, int NumberOfNodes, int NumberOfPaths){
    //Reference: en.wikipedia.org/wiki/Dijkstra's_algorithm
    int lcv = 0;
    int * Unvisited = malloc(sizeof(int) * NumberOfNodes);
    int NumberOfUnvisited = NumberOfNodes;
    int Next = 0;
    float distance = 0;
    NextPosition * New = NULL;
    int Finish = 0;
    int Path[100000];
    int NumberOfNodesNeeded = 0;
    //Initialization
    while(lcv < NumberOfNodes){
        
        Map[lcv].Position->Distance = INFINITE;
        Map[lcv].Position->Previous = -1;
        Unvisited[lcv] = lcv;
        Map[lcv].HeapIndex = lcv;
        lcv++;
    }
    Map[Start].Position->Distance = 0;
    Map[0].HeapIndex = Start;
    Map[Start].HeapIndex = 0;
    Unvisited[0] = Start;
    Unvisited[Start] = 0;
    
    
    //start
    while(NumberOfUnvisited > 0){
        Next = MinDistance(Map, Unvisited, NumberOfUnvisited);              //Find the smallest distances in Unvisited Set
        Map[Next].HeapIndex = -2;
        NumberOfUnvisited--;                                                //Mark that node as visited
        New = Map[Next].NextNodes;                                          //Neighbors of the node with smallest distance
        while(New != NULL && Map[New->Number].HeapIndex == -2){             //Make sure that the node is still in unvisited set
            New = New->NextPosition;
        }
        while(New != NULL){
            distance = Map[Next].Position->Distance + New->Distance;
            if(distance < Map[New->Number].Position->Distance){             //A shorter path has been found
                Map[New->Number].Position->Distance = distance;
                Map[New->Number].Position->Previous = Next;
                Upward_heapify(Unvisited, Map[New->Number].HeapIndex + 1, Map);
            }
            New = New->NextPosition;
            while(New != NULL && Map[New->Number].HeapIndex == -2){
                New = New->NextPosition;
            }
        }

    }
    
    //print the result:
    if(NumberOfUnvisited != -10){
        lcv = 0;
        printf("%.0f\n", Map[End].Position->Distance);
        Path[lcv] = End;
        lcv = 1;
        while(Finish == 0){
            Path[lcv] = Map[Path[lcv - 1]].Position->Previous;
            if(Path[lcv] == Start){
                Finish = 1;
            }
            lcv++;
        }
        NumberOfNodesNeeded = lcv - 1;
        lcv = 0;
        while(lcv < NumberOfNodesNeeded){
            printf("%d ", Path[NumberOfNodesNeeded - lcv]);
            lcv++;
        }
        printf("%d \n", End);
    }else{
        printf("Destination %d cannot be reached from source %d.\n", End, Start);
    }
}

int MinDistance(struct list Map[], int Unvisited[], int NumberOfUnvisited){
    int Cloest = Unvisited[0];
    Unvisited[0] = Unvisited[NumberOfUnvisited - 1];
    Map[Unvisited[NumberOfUnvisited - 1]].HeapIndex = 0;
    Downward_heapify(NumberOfUnvisited - 1, Unvisited, 1, Map);
    return Cloest;
}

void Downward_heapify(int NumberOfUnvisited, int MinHeap[], int i, struct list Map[]){
    int temp_ind = MinHeap[i - 1];
    int temp = Map[MinHeap[i - 1]].Position->Distance;
    int j;
    int Terminate = 0;
    while(i < NumberOfUnvisited / 2 && Terminate != 1){
        j = 2 * i;
        if(j < NumberOfUnvisited && Map[MinHeap[j - 1]].Position->Distance > Map[MinHeap[j]].Position->Distance){
            j++;
        }
        if(temp <= Map[MinHeap[j - 1]].Position->Distance){
            Terminate = 1;
        }else{
            MinHeap[i - 1] = MinHeap[j - 1];
            Map[MinHeap[i - 1]].HeapIndex = i - 1;
            i = j;
        }
    }
    MinHeap[i - 1] = temp_ind;
    Map[MinHeap[i - 1]].HeapIndex = i - 1;
}

void Upward_heapify(int MinHeap[], int n, struct list Map[]){
    int temp_ind = MinHeap[n - 1];
    int temp = Map[MinHeap[n - 1]].Position->Distance;
    int child = n - 1;
    int parent = (child - 1) / 2;
    
    while(Map[MinHeap[parent]].Position->Distance > temp && child > 0){
        MinHeap[child] = MinHeap[parent];
        Map[MinHeap[parent]].HeapIndex = child;
        child = parent;
        parent = (child - 1) / 2;
    }
    MinHeap[child] = temp_ind;
    Map[MinHeap[child]].HeapIndex = child;
}
