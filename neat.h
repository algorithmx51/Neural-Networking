/**
  *
  * Official Copy of NEAT.h library
  * Copywrite (c) Algorithm X, Greece
  * Last edit : --/--/2019
  * Description : UNKNOWN
  *
  * Note : this is an outdated-old version of the neat2.h library
  * 	   Also it may be broken in some systems
  *
  **/

#ifndef _NEAT_H_

#define _NEAT_H_

#include<iostream> /// For debugging reasons
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h> /// Linux time library - for Windows library type #include<time.h>
#include<string.h>


float randomfloat(void)
{

    int b = rand() % 2;
    int a = (rand() % 9999) + 999;

    // printf("a : %d - b : %d - 1/a = %f\n", a, b,(float) 999/a);

    if(b == 0)
        return (float) 999/a;
    else
        return (float) -999/a;

}

typedef struct in
{

    float weigth;
    float input;

}IN;

class neuron
{

public:

    float output_v;

    in *input_v;

};

class layer
{

public:

    neuron *l;

};

class network
{

public:

    int l;
    int *nmap;
    layer *n;

};

/// Neuron functions

float calcOutput(network *Net, int L, int N)
{

    // std::cout << "a" << "\n";

    float sum = 0;

    if(L == 0)
    {

        for(int i = 0; i < 1; i++)
        {
            sum += Net->n[L].l[N].input_v[i].input * Net->n[L].l[N].input_v[i].weigth;
            //  std::cout << i << " " << sum << " " << Net.n[L].l[N].input_v[i].input << " " << Net.n[L].l[N].input_v[i].weigth << "\n";
        }


    }

    else
    {

        for(int i = 0; i < Net->nmap[L-1]; i++)
        {
            sum += Net->n[L].l[N].input_v[i].input * Net->n[L].l[N].input_v[i].weigth;
            //  std::cout << i << " " << sum << " " << Net.n[L].l[N].input_v[i].input << " " << Net.n[L].l[N].input_v[i].weigth << "\n";
        }

    }

    // std::cout << "c" << "\n";

    return tanh(sum);

}

void feedforward(network *Net)
{

    for(int i = 0; i < Net->nmap[1]; i++)
    {
        for(int j = 0; j < Net->nmap[0]; j++)
            Net->n[1].l[i].input_v[j].input = calcOutput(Net, 0, j);
    }

    for(int i = 1; i < Net->l-1; i++)
    {
        for(int j = 0; j < Net->nmap[i+1]; j++)
        {
            for(int l = 0; l < Net->nmap[i]; l++)
            {
                Net->n[i+1].l[j].input_v[l].input = calcOutput(Net, i, l);
            }
        }
    }

    for(int i = 0; i < Net->nmap[Net->l-1]; i++)
    {
        Net->n[Net->l-1].l[i].output_v = calcOutput(Net, Net->l-1, i);
    }

}

void setRand(network *Net)
{

    srand(time(NULL));

    for(int i = 0; i < Net->nmap[0]; i++)
    {
        Net->n[0].l[i].input_v[0].weigth = 1;
        //  std::cout << Net->n[0].l[i].input_v[0].weigth << "\n";
    }

    for(int i = 1; i < Net->l; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            for(int k = 0; k < Net->nmap[i-1]; k++)
            {
                Net->n[i].l[j].input_v[k].weigth = randomfloat();

		if(Net->n[i].l[j].input_v[k].weigth > 1) Net->n[i].l[j].input_v[k].weigth = 1;
		else if(Net->n[i].l[j].input_v[k].weigth < -1) Net->n[i].l[j].input_v[k].weigth = -1;
		//  std::cout << Net->n[i].l[j].input_v[k].weigth << "\n";
            }
        }
    }

}


/// Network functions

void confNet(network *Net, int layers, int nmap[])
{

    Net->n = (layer*) malloc(sizeof(layer) * layers);
    Net->nmap = (int *) malloc (sizeof (nmap));
    Net->l = layers;

    for(int i = 0; i < Net->l; i++){

	Net->nmap[i] = nmap[i];

    }

    //std::cout <<"\n"; /// Do not remove -- bug solver (malloc top corruption)

    for(int i = 0; i < layers; i++)
    {

        Net->n[i].l = (neuron*) malloc(sizeof(neuron) * nmap[i]);

    }

    for(int i = 0; i < nmap[0]; i++)
    {
        Net->n[0].l[i].input_v = (in*) malloc(sizeof(in) * 1);
    }

    for(int i = 1; i < layers; i++)
    {

        for(int j = 0; j < nmap[i]; j++)
        {
            Net->n[i].l[j].input_v = (in*) malloc(sizeof(in) * ( sizeof(Net->n[i-1]) / sizeof(layer) ) );
        }

    }


}

void mutateNet(network *Net, int layers, int nmap[])
{

    for(int i = 1; i < Net->l; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            for(int k = 0; k < Net->nmap[i-1]; k++)
            {
                if(rand() % 50 == 0)
                    Net->n[i].l[j].input_v[k].weigth += randomfloat();

                //  std::cout << Net->n[i].l[j].input_v[k].weigth << "\n";
            }
        }
    }

}

int printNet(network *Net, int f)
{

    printf("PrintNet --------------- %d\n", f);

    if(f == 1)
    {

        for(int i = 0; i < Net->nmap[0]; i++)
        {
            printf("Net->n[0].l[%d].input_v[0].input = %f\nNet->n[0].l[%d].input_v[0].weigth = %f\n", i, Net->n[0].l[i].input_v[0].input, i, Net->n[0].l[i].input_v[0].weigth);
        }

        for(int i = 1; i < Net->l; i++)
        {
            for(int j = 0; j < Net->nmap[i]; j++)
            {

                for(int k = 0; k < Net->nmap[i-1]; k++)
                    printf("Net->n[%d].l[%d].input_v[%d].input = %f\nNet->n[%d].l[%d].input_v[%d].weigth = %f\n", i, j, k, Net->n[i].l[j].input_v[k].input, i, j, k, Net->n[i].l[j].input_v[k].weigth);

            }
        }
    }

    printf("\nOutput : ---------------------\n");

    for(int i = 0; i < Net->nmap[Net->l-1]; i++)
            printf("Net->n[%d].l[%d].output_v = %f\n", Net->l-1, i, Net->n[Net->l-1].l[i].output_v);

    printf("\n\n");

    return 0;
}

void setNet(network *Net, float INP[])
{
    for(int i = 0; i < sizeof(INP) / sizeof(float); i++)
    {
        Net->n[0].l[i].input_v[0].input = INP[i];
    }

}

#endif // _NEAT_H_
