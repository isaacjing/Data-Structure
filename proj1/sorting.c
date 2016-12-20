#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long *Load_File (char *Filename, int *Size)
{
	int temp_N = 0;							//Loop control variable
	long *data;								//array
	long number;							//temp variable to store the value
	FILE *fh;
	fh = fopen(Filename, "r");
	if (fh == NULL)
	{
		fclose (fh);
		return NULL;
	}
	fscanf(fh, "%d", Size);
	
	data = malloc(sizeof (long)*(*Size));
	while(temp_N < (*Size))
	{
		fscanf(fh, "%ld", &number);
		data[temp_N] = number;
		temp_N ++;
	}
	
	fclose(fh);
	return data;
}


int Save_File (char *Filename, long *Array, int Size) 
{
	FILE * fout;
	fout = fopen(Filename, "w");
	int lcv = 0;
	fprintf(fout, "%d\n", Size);
	while (lcv < Size)
	{
		fprintf(fout, "%ld\n", Array[lcv]);
		lcv++;
	}
	fclose(fout);
	return (lcv);
}

void Shell_Insertion_Sort (long *Array, int Size, double *N_Comp, double *N_Move)
{
	int * seq1 = malloc(sizeof (int) * Size);
    //Save_Seq1 ("sequence1.txt", Size);
	int len = seq_gen_1(seq1, Size);
	//sort the sequence
    
	int ind2 = 0;
	int ind3 = 0;
	int temp = 0;
	for (ind2 = 0 ; ind2 < (len - 1); ind2++)
	{
		for (ind3 = 0 ; ind3 < len - ind2 - 1; ind3++)
		{
			if (seq1[ind3] < seq1[ind3+1])
			{
				temp = seq1[ind3];
				seq1[ind3] = seq1[ind3 + 1];
				seq1[ind3 + 1] = temp;
			}
		}
	}
	//testing for sequence generator and sorting the seq1
	/*
	FILE * fout;
	fout = fopen("seq1.txt", "w");
	int lcv = 0;
	while (lcv < len)
	{
		fprintf(fout, "%d\n", seq1[lcv]);
		lcv++;
	}
	fclose(fout);*/
	
	
	//	Shell Insertion Sort
	int j;
	int i;
	long temp_seq1; 
	*N_Comp = 0;
	*N_Move = 0;
	ind2 = 0;
	while(ind2 < len)
	{
		ind3 = seq1[ind2]; //K
		j = ind3;
		while(j <= Size - 1)
		{
			temp_seq1 = Array[j];
			(*N_Move) += 2;
			i = j;
			while (i >= ind3 && Array[i - ind3] > temp_seq1)
			{
				Array[i] = Array[i - ind3];
				(*N_Move) += 1;
				(*N_Comp) += 1;
				i = i - ind3;
			}
			if(i >= ind3)
			{
				(*N_Comp) += 1;
			}
			Array[i] = temp_seq1;
			j++;
		}
		ind2 ++;
	}
	free(seq1);
}
int seq_gen_1(int * seq1, int Size)
{
//Sequence generator 1
/*
	int p = 0;
	int q = 0;
	int PPlusQ = 0;									//sum of p and q
	int ind = 0;									//index of the sequence 1
	seq1[0] = 1;
	
	while(seq1[ind - 1] < Size)
	{
		while(seq1[ind - 1] < Size && p <= PPlusQ)
		{	
			q = PPlusQ - p;
			seq1[ind] = pow(2, p) * pow(3, q);
			if(seq1[ind] > Size)
			{
				return ind;
			}
			ind ++;
			p ++;
			
		}
		PPlusQ ++;
		p = 0;
	}
	ind--;
	return ind;
*/
	int pointer2 = 0;
	int pointer3 = 0;
	int ind = 1;
	seq1[0] = 1;
	while (1)
	{
		if(seq1[pointer2] * 2 > seq1[pointer3] * 3)
		{
			if(seq1[pointer3] * 3 > Size)
			{
				return(ind - 1);
			}
			else
			{
				seq1[ind] = seq1[pointer3] * 3;
				pointer3 ++;
				ind++;
			}
		}
		if(seq1[pointer2] * 2 < seq1[pointer3] * 3)
		{
			if(seq1[pointer2] * 2 > Size)
			{
				return(ind - 1);
			}
			else
			{
				seq1[ind] = seq1[pointer2] * 2;
				pointer2 ++;
				ind++;
			}
		}
		if(seq1[pointer2] * 2 == seq1[pointer3] * 3)
		{
			if(seq1[pointer2] * 2 > Size)
			{
				return(ind - 1);
			}
			else
			{
				seq1[ind] = seq1[pointer2] * 2;
				pointer2 ++;
				pointer3 ++;
				ind++;
			}
		}
	}
}



void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move)
{
	int * seq2 = malloc(sizeof (int) * Size);
    //Save_Seq2("sequence2.txt", Size);
	int len = seq_gen_2(seq2, Size);
    
    
	/*testing the sequence generator 2
	FILE * fout;
	fout = fopen("seq1.txt", "w");
	int lcv = 0;
	while (lcv < len)
	{
		fprintf(fout, "%d\n", seq2[lcv]);
		lcv++;
	}
	fclose(fout);
	*/
	
	int j;
	int i;
	int m;
	long temp_seq2; 
	*N_Comp = 0;
	*N_Move = 0;
	int ind2 = 0;
	int ind3 = 0;
	while(ind2 < len)
	{
		ind3 = seq2[ind2]; 				//K
		j = ind3;
		for(m = Size; m >= ind3; m = m-ind3){
		while(j <= m - 1)
		{
			temp_seq2 = Array[j];
			(*N_Move) += 2;
			i = j;
			*N_Comp += 1;
			if (i >= ind3 && Array[i - ind3] > temp_seq2)
			{
				Array[i] = Array[i - ind3];
				i = i - ind3;
				(*N_Move) += 1;
			}
			//(*N_Move) += 1;
			Array[i] = temp_seq2;
			j++;
		}
		}
		ind2 ++;
	}
	free(seq2);
}

int seq_gen_2(int * seq2, int Size)
{
	seq2[0] = (int) (Size / 1.3);
	int ind = 1;
	float temp;
	do
	{
		temp = seq2[ind - 1] / 1.3;
		seq2[ind] = (int) (temp);
		if(seq2[ind] == 9 || seq2[ind] == 10)
		{
			seq2[ind] = 11;
		}
		ind++;
	}while(seq2[ind - 1] != 1);
	return ind;
}

void Save_Seq1 (char *Filename, int Size)
{
    int * seq1 = malloc(sizeof (int) * Size);
    int len;
    len = seq_gen_1(seq1, Size);
    
    
    //sort the sequence
    //This part of the function was originally in the shell sort function.
    int ind2 = 0;
    int ind3 = 0;
    int temp = 0;
    for (ind2 = 0 ; ind2 < (len - 1); ind2++)
    {
        for (ind3 = 0 ; ind3 < len - ind2 - 1; ind3++)
        {
            if (seq1[ind3] < seq1[ind3+1])
            {
                temp = seq1[ind3];
                seq1[ind3] = seq1[ind3 + 1];
                seq1[ind3 + 1] = temp;
            }
        }
    }
    
    
    
    //Save the sequence
    FILE * fout;
    fout = fopen(Filename, "w");
    int lcv = 0;
    fprintf(fout, "%d\n", Size);
    while (lcv < len)
    {
        fprintf(fout, "%d\n", seq1[lcv]);
        lcv++;
    }
    fclose(fout);
}

void Save_Seq2 (char *Filename, int Size)
{
    int * seq2 = malloc(sizeof (int) * Size);
    int len;
    len = seq_gen_2(seq2, Size);
    
    
    //sort the sequence
    //This part of the function was originally in the bubble sort function.
    int ind2 = 0;
    int ind3 = 0;
    int temp = 0;
    for (ind2 = 0 ; ind2 < (len - 1); ind2++)
    {
        for (ind3 = 0 ; ind3 < len - ind2 - 1; ind3++)
        {
            if (seq2[ind3] < seq2[ind3+1])
            {
                temp = seq2[ind3];
                seq2[ind3] = seq2[ind3 + 1];
                seq2[ind3 + 1] = temp;
            }
        }
    }
    
    
    
    //Save the sequence
    FILE * fout;
    fout = fopen(Filename, "w");
    int lcv = 0;
    fprintf(fout, "%d\n", Size);
    while (lcv < len)
    {
        fprintf(fout, "%d\n", seq2[lcv]);
        lcv++;
    }
    fclose(fout);

}