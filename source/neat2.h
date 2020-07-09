/**
  *
  * Official Copy of NEAT2.h library
  * Copywrite (c) Algorithm X, Greece
  * Last edit : 08/07/2020
  * Description : improved drawNet for Linux
  *
  *
  **/

#ifndef NEAT2_H
#define NEAT2_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<graphics.h>

class neuron;
class layer;
class network;

#ifndef linux // if we aren't at a linux system

#include<conio.h>
#include<time.h>

int NEAT2_DRAW_NET_CW = 0; // current_window
void drawNet(network *, int, int); // draw the net on the provided window

#else

#include<sys/time.h>
void drawNet(network *, int, int, int, int); // draw the net on the provided co-ords

#endif // linux

void calcOutput(network*, int, int); // calculate the output of the neuron defined by the two ints
network *confNet(int, int[]); // allocate memory for the network
network *copyNet(network*); // copy the net to a new place
void feedforward(network*); // feed the net so we have a final output
neuron *getneuronP(network*, int, int); // get the address in memory a the selected neuron
void mutateNet(network*, int, int); // mutate the net
void printNet(network*, int); // print the net
float randomfloat(void); // generate a random float
void setNet(network*, float[]); // set the input for the net
int sum(int*, int); // calculate the sum of an array
void exportNet(network *, FILE *); // export the net
network * importNet(FILE *); // import the net
float calcAvgWeight(network *); // calculate the average weight of the network


typedef struct connection
{

    neuron *n; // a pointer to the neuron connected
    float weight; // the weight of the connection

}connection;


class neuron
{

public:

    connection *connect; // a list of connections
    int num_connections; // the number of connections
    float sum; // the sum of the input
    float output ; // the calculated output

};

class layer
{

public:
    neuron *n; // a list of neurons

};

class network
{

public:
    layer *l; // a list of layers
    int *nmap; // network map , how many neurons on each layer
    int layers; // How many layers

};

/// *****************************************************************************************************
/// Utility functions -----------------------------------------------------------------------------------

int sum ( int *array, int size )
{

    int s = 0;

    for(int i = 0; i < size; i++)
        s += array[i];

    return s;

}

/// ========================================================================================

neuron * getneuronP(network *Net, int L, int N)
{

    return &(Net->l[L].n[N]);

}

/// ========================================================================================

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

/// ========================================================================================

float calcAvgWeight(network *Net)
{
    float s = 0;
    int num_connections = 0;

    for(int i = 0; i < Net->layers; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            for(int k = 0; k < Net->l[i].n[j].num_connections; k++)
            {
                s += Net->l[i].n[j].connect[k].weight;
                num_connections++;
            }
        }
    }

    return (s / num_connections);

}

/// *****************************************************************************************************
/// Network Functions -----------------------------------------------------------------------------------

network *confNet(int _layers, int _nmap[])
{
    network *Net;

    Net = (network *) malloc ( sizeof (network) ); // we want 1 network, right?

    Net->layers = _layers;
    Net->nmap = (int *) malloc ( _layers * sizeof (int) );

    for(int i = 0; i < Net->layers; i++)
    {
        Net->nmap[i] = _nmap[i];
    }

    Net->l = (layer *) malloc ( Net->layers * sizeof (layer) );

    for(int i = 0; i < Net->layers; i++)
    {
        Net->l[i].n = (neuron *) malloc ( sizeof( neuron ) * Net->nmap[i] );

        for(int j = 0; j < Net->nmap[i]; j++)
        {
            Net->l[i].n[j].connect = (connection *) malloc ( sizeof( connection ) * sum(Net->nmap, i) ); // up to : sum(Net->nmap, i)

            for(int ii = 0, k = 0 ; ii < i && k < sum(Net->nmap, i); ii++)
            {
                for(int jj = 0; jj < Net->nmap[ii] && k < sum(Net->nmap, i); jj++, k++)
                {
                    Net->l[i].n[j].connect[k].n = getneuronP(Net, ii, jj);
                    Net->l[i].n[j].connect[k].weight = randomfloat();

                    if(Net->l[i].n[j].connect[k].weight > 1) // check if the weight is off-limits
                        Net->l[i].n[j].connect[k].weight = 1;
                    else if (Net->l[i].n[j].connect[k].weight < -1)
                        Net->l[i].n[j].connect[k].weight = -1;
                }
            }

            Net->l[i].n[j].sum = 0; // initialize the neuron variables
            Net->l[i].n[j].output = 0;
            Net->l[i].n[j].num_connections = ( sum(Net->nmap, i) );
        }
    }

    return Net;
}

/// =======================================================================================================

void calcOutput(network *Net, int L, int N)
{

    float s = 0;

    if(L != 0)
    {

        for(int i = 0; i < Net->l[L].n[N].num_connections; i++)
        {
            s += Net->l[L].n[N].connect[i].n->output * Net->l[L].n[N].connect[i].weight;
        }

        Net->l[L].n[N].sum = s;
        Net->l[L].n[N].output = tanh(s); // activation function
    }

}

/// =======================================================================================================

void feedforward(network *Net)
{

    for(int i = 1; i < Net->layers; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            calcOutput(Net, i, j);
        }
    }

}

/// =======================================================================================================

void printNet(network *Net, int detailed)
{

    if(detailed) printf("----------------------------------------\n");

    if(detailed)
    {

        printf("In Net 0x%x\n", Net);

        for(int j = 0; j < Net->nmap[0]; j++)
        {
            printf("Net->l[0].n[%d].output = %f\n", j, Net->l[0].n[j].output);
            printf("Net->l[0].n[%d].number of connections = 1\n", j);
            printf("Net->l[0].n[%d].connect[0].weight = 1\n", j);
        }

        for(int i = 1 ; i < Net->layers-1; i++)
        {
            printf("----------------------------------------\n");

            for(int j = 0; j < Net->nmap[i]; j++)
            {
                printf("Net->l[%d].n[%d].sum = %f\n", i, j, Net->l[i].n[j].sum);
                printf("Net->l[%d].n[%d].output = %f\n", i, j, Net->l[i].n[j].output);
                printf("Net->l[%d].n[%d].number of connections = %d\n", i, j, Net->l[i].n[j].num_connections);

                for(int k = 0; k < Net->l[i].n[j].num_connections; k++)
                {
                    printf("Net->l[%d].n[%d].connect[%d].weight = %f\n", i, j, k, Net->l[i].n[j].connect[k].weight);
                /*    printf("Net->l[%d].n[%d].connect[%d].n->sum = %f\n", i, j, k, Net->l[i].n[j].connect[k].n->sum); */
                    printf("Net->l[%d].n[%d].connect[%d].n->output = %f\n", i, j, k, Net->l[i].n[j].connect[k].n->output);
                }
            }
        }
    }

    printf("----------------------------------------\n");

    if(!detailed) printf("In Net 0x%x\n", Net);

    for(int i = 0; i < Net->nmap[Net->layers-1]; i++)
    {
        printf("Net->l[%d].n[%d].sum = %f\n", Net->layers-1, i, Net->l[Net->layers-1].n[i].sum);
        printf("Net->l[%d].n[%d].output = %f\n", Net->layers-1, i, Net->l[Net->layers-1].n[i].output);

        printf("Net->l[%d].n[%d].number of connections = %d\n", Net->layers-1, i, Net->l[Net->layers-1].n[i].num_connections);

        for(int k = 0; k < Net->l[Net->layers-1].n[i].num_connections; k++)
        {
            printf("Net->l[%d].n[%d].connect[%d].weight = %f\n", Net->layers-1, i, k, Net->l[Net->layers-1].n[i].connect[k].weight);
        /*    printf("Net->l[%d].n[%d].connect[%d].n->sum = %f\n", Net->layers-1, i, k, Net->l[Net->layers-1].n[i].connect[k].n->sum); */
            printf("Net->l[%d].n[%d].connect[%d].n->output = %f\n", Net->layers-1, i, k, Net->l[Net->layers-1].n[i].connect[k].n->output);
        }
    }


    printf("----------------------------------------\n");
}

/// =======================================================================================================

void setNet(network *Net, float INP[])
{

    for(int i = 0; i < Net->nmap[0]; i++)
    {
        Net->l[0].n[i].output = INP[i]; // since there are no previous layers, we store the input as an output
    }

}

/// =======================================================================================================

void mutateNet(network *Net, int degree, int percent)
{

    if(degree == 0)  // Simple Mutation
    {

        for(int i = 1; i < Net->layers; i++) // Begin from the 2nd layer
        {
            for(int j = 0; j < Net->nmap[i]; j++)
            {
                for(int k = 0; k < Net->l[i].n[j].num_connections; k++)
                {
                    if(rand() % 100 - percent < 0)
                        Net->l[i].n[j].connect[k].weight += randomfloat();

                    if(Net->l[i].n[j].connect[k].weight > 1)
                        Net->l[i].n[j].connect[k].weight = 1;
                    else if (Net->l[i].n[j].connect[k].weight < -1)
                        Net->l[i].n[j].connect[k].weight = -1;
                }
            }
        }

    }
    else if(degree == 1)  // Total Mutation
    {

        for(int i = 1; i < Net->layers; i++) // Begin from the 2nd layer
        {
            for(int j = 0; j < Net->nmap[i]; j++)
            {
                for(int k = 0; k < Net->l[i].n[j].num_connections; k++)
                {
                    if(rand() % 100 - percent < 0)
                        Net->l[i].n[j].connect[k].weight = randomfloat();

                    if(Net->l[i].n[j].connect[k].weight > 1)
                        Net->l[i].n[j].connect[k].weight = 1;
                    else if (Net->l[i].n[j].connect[k].weight < -1)
                        Net->l[i].n[j].connect[k].weight = -1;
                }
            }
        }

    }
    /* else if(degree == 2){ // Total-Net Mutation

         for(int i = 1; i < Net->layers; i++) // Begin from the 2nd layer
         {
             for(int j = 0; j < Net->nmap[i]; j++)
             {
                 for(int k = 0; k < Net->l[i].n[j].num_connections; k++)
                 {
                    if(rand() % 100 - percent < 0)
                         Net->l[i].n[j].connect[k].weight = randomfloat();

                     if(Net->l[i].n[j].connect[k].weight > 1)
                         Net->l[i].n[j].connect[k].weight = 1;
                     else if (Net->l[i].n[j].connect[k].weight < -1)
                         Net->l[i].n[j].connect[k].weight = -1;

                     insertLayer(Net, rand() % (Net->layers-1) + 1, rand() % 5); // add up to 5 neurons to a random position
                 }
             }
         }

     } */ /// Up to construction

}

/// =======================================================================================================

network *copyNet(network *Net)
{
    network *Net2;

    Net2 = confNet(Net->layers, Net->nmap); // create a new empty net

    for(int i = 1; i < Net2->layers; i++)
    {
        for(int j = 0; j < Net2->nmap[i]; j++)
        {
            for(int k = 0; k < Net2->l[i].n[j].num_connections; k++)
            {
                Net2->l[i].n[j].connect[k].weight = Net->l[i].n[j].connect[k].weight;
            }
        }
    }

    return Net2;
}

/// =======================================================================================================

#ifndef linux

void drawNet(network *Net, int window, int d)
{

    int w = getcurrentwindow();

    setcurrentwindow(window);

    int oldv = NEAT2_DRAW_NET_CW; // save the cw var to a tmp int

    if(!d){ // if we have not a direct output
    setactivepage(NEAT2_DRAW_NET_CW); // change the pages and display the old page
    setvisualpage(1 - NEAT2_DRAW_NET_CW);
    }
    else
        setvisualpage(NEAT2_DRAW_NET_CW); // write direct to the screen


    cleardevice();

    char buffer[400];

    int maxx = getmaxx(), maxy = getmaxy();

    for(int i = 0; i < Net->layers; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            for(int ii = 0, k = 0; ii < i && k < Net->l[i].n[j].num_connections; ii++)
            {
                for(int jj = 0; jj < Net->nmap[ii] && k < Net->l[i].n[j].num_connections; jj++, k++)
                {
                    if(Net->l[i].n[j].connect[k].weight > 0)
                        setcolor(RED);
                    else
                        setcolor(BLUE);

                    setlinestyle( SOLID_LINE, 0, abs(Net->l[i].n[j].connect[k].weight * 9)); // set the thickness of the line

                    line( (ii+1) * (maxx / (Net->layers+1)) + 25 + 1, (jj+1) * (maxy / (Net->nmap[ii] + 1)), (i+1) * (maxx / (Net->layers+1)) - 25 - 1, (j+1) * (maxy / (Net->nmap[i] + 1)));
                    //  sprintf(buffer, "%f", Net->l[i].n[j].connect[k].weight);
                    //  outtextxy( ( ( ( ii+1 ) * (maxx / (Net->layers+1)) + 25 ) + ( (i+1) * (maxx / (Net->layers+1)) - 25) ) / 2, ( ( (jj+1) * (maxy / (Net->nmap[ii] + 1))) + ( (j+1) * (maxy / (Net->nmap[i] + 1) ) ) ) / 2 - 20, buffer);

                    setlinestyle(SOLID_LINE, 0, 1);

                }
            }
        }

    }

    for(int i = 0; i < Net->layers; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            setcolor(WHITE);
            circle((i+1) * (maxx / (Net->layers+1)), (j+1) * (maxy / (Net->nmap[i] + 1)), 25);
            sprintf(buffer, "%0.3f", Net->l[i].n[j].output);
            outtextxy((i+1) * (maxx / (Net->layers+1)) - 18, (j+1) * (maxy / (Net->nmap[i] + 1)) - 8, buffer);

        }

    }

    if(!d) // if not a direct output , reset the cw var
        NEAT2_DRAW_NET_CW = 1 - NEAT2_DRAW_NET_CW;
    else // reset to the old cw
        NEAT2_DRAW_NET_CW = oldv;

    setcurrentwindow(w);

}

#else

void drawNet(network *Net, int x1, int y1, int x2, int y2)
{

    int xmax = getmaxx(), ymax = getmaxy();

    char buffer[400];

    if(x1 > xmax || x2 > xmax || y1 > ymax || y2 > ymax)
        return;

    int maxx = x2 - x1, maxy = y2 - y1;

    for(int i = x1; i < x2; i++)
        for(int j = y1; j < y2; j++)
            if(getpixel(i, j) != BLACK)
                putpixel(i, j, BLACK);

    for(int i = 0; i < Net->layers; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            for(int ii = 0, k = 0; ii < i && k < Net->l[i].n[j].num_connections; ii++)
            {
                for(int jj = 0; jj < Net->nmap[ii] && k < Net->l[i].n[j].num_connections; jj++, k++)
                {
                    if(Net->l[i].n[j].connect[k].weight > 0)
                        setcolor(RED);
                    else
                        setcolor(BLUE);

                    setlinestyle( SOLID_LINE, 0, abs(Net->l[i].n[j].connect[k].weight * 9)); // set the thickness of the line

                    line( (ii+1) * ((maxx) / (Net->layers+1)) + 25 + 1 + x1, (jj+1) * (maxy / (Net->nmap[ii] + 1)) + y1, (i+1) * (maxx / (Net->layers+1)) - 25 - 1 + x1, (j+1) * (maxy / (Net->nmap[i] + 1)) + y1);
                    //  sprintf(buffer, "%f", Net->l[i].n[j].connect[k].weight);
                    //  outtextxy( ( ( ( ii+1 ) * (maxx / (Net->layers+1)) + 25 ) + ( (i+1) * (maxx / (Net->layers+1)) - 25) ) / 2, ( ( (jj+1) * (maxy / (Net->nmap[ii] + 1))) + ( (j+1) * (maxy / (Net->nmap[i] + 1) ) ) ) / 2 - 20, buffer);

                    setlinestyle(SOLID_LINE, 0, 1);

                }
            }
        }

    }

    for(int i = 0; i < Net->layers; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {

            setcolor(WHITE);
            circle((i+1) * (maxx / (Net->layers+1)) + x1, (j+1) * (maxy / (Net->nmap[i] + 1)) + y1, 25);
            sprintf(buffer, "%0.3f", Net->l[i].n[j].output);

            setfontcolor(WHITE);
            outtextxy((i+1) * (maxx / (Net->layers+1)) - 18 + x1, (j+1) * (maxy / (Net->nmap[i] + 1)) - 8 + y1, buffer);

        }

    }

}

#endif

/// =======================================================================================================

void exportNet(network *Net, FILE *file)
{
    if(file == NULL)
    {
        printf("Error, file not open !\n");
        return ;
    }

    int s = 0;

    for(int i = 0; i < Net->layers; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            for(int k = 0; k < Net->l[i].n[j].num_connections; k++, s++)
            {
                ; // count the number of connections
            }
        }
    }

    float *array;

    array =(float *) malloc ( sizeof(float) * (s + Net->layers + 2));

    array[0] = (float) sizeof(float) * (s + Net->layers + 1);
    array[1] =(float) Net->layers;

    s = 2;

    for(int i = 0; i < Net->layers; i++, s++)
        array[s] =(float) Net->nmap[i];

    for(int i = 0; i < Net->layers; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            for(int k = 0; k < Net->l[i].n[j].num_connections; k++, s++)
            {
                array[s] = Net->l[i].n[j].connect[k].weight;
            }
        }
    }

    fwrite(array, sizeof(float) * (s), 1, file);
    fclose(file);

}

/// =======================================================================================================

network *importNet(FILE *file)
{
    if(file == NULL)
    {
        printf("Error, file not open !\n");
        return  (network *) -1;
    }

    fseek(file, 0, 0);

    network *Net;
    float *p =(float *) malloc(sizeof(float));
    float *array;

    fread(p, sizeof(float), 1, file);
    array =(float *) malloc (*p);

    fread(array, *p, 1, file);

    int s = 1;
    int *nmap = (int *) malloc ( sizeof (int) * array[0]);

    for(int i = 0; i < array[0]; i++, s++)
        nmap[i] = array[s];

    Net = confNet(array[0], nmap);

    for(int i = 1; i < Net->layers; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            for(int k = 0; k < Net->l[i].n[j].num_connections; k++, s++)
            {
                Net->l[i].n[j].connect[k].weight = array[s];
            }
        }
    }

    return Net;
}

/// *******************************************************************************************************

#endif // NEAT2_H
