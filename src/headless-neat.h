/*
 *
 *
 * Official Copy of NEAT.h library
 * Current version : v1.1
 *
 * Copyright (C) 2020  Algorithm X
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HEADLESS_NEAT_H
#define HEADLESS_NEAT_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

class neuron;
class layer;
class network;

#ifndef linux // if we aren't at a linux system

#include<conio.h>
#include<time.h>

#else

#include<sys/time.h>

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
void exportNet(network *, char *); // export the net
network * importNet(char *); // import the net
float calcAvgWeight(network *); // calculate the average weight of the network
void connectNeuron(neuron *, neuron *); // connect two neurons
void disconnectNeuron(neuron *, neuron *); // disconnect two neurons
void setWeight(neuron *, neuron *, float); // set the weight of a connection


typedef struct connection
{

    neuron *n; // a pointer to the neuron connected
    float weight; // the weight of the connection

} connection;


class neuron
{

public:

    connection *connect; // a list of connections
    int num_connections; // the number of connections
    float sum; // the sum of the input
    float output ; // the calculated output

    int l; // the coords of the neuron on the network
    int n;

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

            Net->l[i].n[j].l = i;
            Net->l[i].n[j].n = j;

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

    if(detailed)
        fprintf(stdout, "----------------------------------------\n");

    if(detailed)
    {

        fprintf(stdout, "In Net 0x%x\n", Net);

        for(int j = 0; j < Net->nmap[0]; j++)
        {
            fprintf(stdout, "Net->l[0].n[%d].output = %f\n", j, Net->l[0].n[j].output);
            fprintf(stdout, "Net->l[0].n[%d].number of connections = 1\n", j);
            fprintf(stdout, "Net->l[0].n[%d].connect[0].weight = 1\n", j);
        }

        for(int i = 1 ; i < Net->layers-1; i++)
        {
            fprintf(stdout, "----------------------------------------\n");

            for(int j = 0; j < Net->nmap[i]; j++)
            {
                fprintf(stdout, "Net->l[%d].n[%d].sum = %f\n", i, j, Net->l[i].n[j].sum);
                fprintf(stdout, "Net->l[%d].n[%d].output = %f\n", i, j, Net->l[i].n[j].output);
                fprintf(stdout, "Net->l[%d].n[%d].number of connections = %d\n", i, j, Net->l[i].n[j].num_connections);

                for(int k = 0; k < Net->l[i].n[j].num_connections; k++)
                {
                    fprintf(stdout, "Net->l[%d].n[%d].connect[%d].weight = %f\n", i, j, k, Net->l[i].n[j].connect[k].weight);
                    /*    fprintf(stdout, "Net->l[%d].n[%d].connect[%d].n->sum = %f\n", i, j, k, Net->l[i].n[j].connect[k].n->sum); */
                    fprintf(stdout,"Net->l[%d].n[%d].connect[%d].n->output = %f\n", i, j, k, Net->l[i].n[j].connect[k].n->output);
                }
            }
        }
    }

    fprintf(stdout, "----------------------------------------\n");

    if(!detailed)
        fprintf(stdout, "In Net 0x%x\n", Net);

    for(int i = 0; i < Net->nmap[Net->layers-1]; i++)
    {
        fprintf(stdout, "Net->l[%d].n[%d].sum = %f\n", Net->layers-1, i, Net->l[Net->layers-1].n[i].sum);
        fprintf(stdout, "Net->l[%d].n[%d].output = %f\n", Net->layers-1, i, Net->l[Net->layers-1].n[i].output);

        fprintf(stdout, "Net->l[%d].n[%d].number of connections = %d\n", Net->layers-1, i, Net->l[Net->layers-1].n[i].num_connections);

        for(int k = 0; k < Net->l[Net->layers-1].n[i].num_connections; k++)
        {
            fprintf(stdout, "Net->l[%d].n[%d].connect[%d].weight = %f\n", Net->layers-1, i, k, Net->l[Net->layers-1].n[i].connect[k].weight);
            /*    printf("Net->l[%d].n[%d].connect[%d].n->sum = %f\n", Net->layers-1, i, k, Net->l[Net->layers-1].n[i].connect[k].n->sum); */
            fprintf(stdout, "Net->l[%d].n[%d].connect[%d].n->output = %f\n", Net->layers-1, i, k, Net->l[Net->layers-1].n[i].connect[k].n->output);
        }
    }


    fprintf(stdout, "----------------------------------------\n");
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
        for(int j = 0; j < Net2->nmap[i]; j++)
            for(int k = Net2->l[i].n[j].num_connections - 1; k >= 0; k--)
               disconnectNeuron(Net2->l[i].n[j].connect[k].n, getneuronP(Net2, i, j));


    for(int i = 0; i < Net->layers; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            for(int k = 0; k < Net->l[i].n[j].num_connections; k++)
            {
                connectNeuron(getneuronP(Net2, Net->l[i].n[j].connect[k].n->l, Net->l[i].n[j].connect[k].n->n), getneuronP(Net2, i, j));
                setWeight(getneuronP(Net2, Net->l[i].n[j].connect[k].n->l, Net->l[i].n[j].connect[k].n->n), getneuronP(Net2, i, j), Net->l[i].n[j].connect[k].weight);
            }
        }
    }


    return Net2;
}

/// =======================================================================================================

void exportNet (network *Net, char *filename)
{
    FILE *file = fopen(filename, "wb");

    if(file == NULL)
    {
        printf("\nfopen failed to open file '%s'\n", filename);
        return ;
    }


    float *data; // the output data
    unsigned int size = 1 + 1 + Net->layers;

    unsigned int index;

    // So we need 1 float for the size, 1 float for the layers, n floats for the nmap ( n = number of layers ). (example : |120| |3| |3 2 1| )
    // Each neuron allocates : 2 floats for the coords, 1 float for the number of connections, n x ( 2 floats for the coords of the connected neuron + 1 float for the weight of the connection - n = number of connections ). example : |0 0| |2| |1 0| |0.5| |1 1| |-0.7|
    // size(1) + layers(1) + nmap(layers) + sum(nmap, layers) x [ neuron(2) + connections(1) + connections x [ neuron connected (2) + weight (1) ]  ] = final size

    // I think it would be easier if we just counted the floats we need ( if anyone finds a formula, I REALLY want to see how it works )


    for(int i = 0; i < Net->layers; i++)
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            size += (3 + 3 * Net->l[i].n[j].num_connections);
        }


    data  = (float *) malloc(size * sizeof (float)); // type-casting float* to avoid compiler warnings


    data[0] = size-1;
    data[1] = Net->layers;


    for(index = 2; index < Net->layers + 2; index++)
        data[index] = Net->nmap[index-2];

    for(int i = 0; i < Net->layers; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            data[index] = i;
            data[index+1] = j;
            data[index+2] = Net->l[i].n[j].num_connections;

            index += 3;

            for(int k = 0; k < Net->l[i].n[j].num_connections; k++)
            {
                data[index] = Net->l[i].n[j].connect[k].n->l;
                data[index+1] = Net->l[i].n[j].connect[k].n->n;

                data[index+2] = Net->l[i].n[j].connect[k].weight;

                index += 3;
            }
        }
    }


    if(index != size)
        fprintf(stdout, "WARNING : index != size\n");

    fseek(file, 0, 0);
    fwrite(data, size * sizeof(float), 1, file);
    fclose(file);

}

/// =======================================================================================================

network *importNet (char *filename)
{

    FILE *file = fopen(filename, "rb");

    if(file == NULL)
    {
        printf("fopen failed to open file '%s'", filename);
        return  (network *) 0; // Avoid warnings
    }

    // So we have two ways
    // 1: reconstruct the whole network from scratch -- not going to do that !!
    // 2 : call confNet, disconnect all neurons and then connect the neurons that are mentioned in the input file -- lets do it

    float *data;

    float size;
    unsigned int index = 1;

    fread(&size, sizeof(float), 1, file);
    data = (float *) malloc(size * ( sizeof(float) ) ); // Avoid warnings


    fread(data, size * sizeof (float), 1, file );

    int number_of_layers = data[0];
    int nmap[number_of_layers];

    for(int i = 0 ; i < number_of_layers; i++, index++)
        nmap[i] = data[index];

    // Now that we have the number of the layers and the network map, we call confNet

    network *Net = confNet(number_of_layers, nmap);

    for(int i = 0; i < Net->layers; i++)
        for(int j = 0; j < Net->nmap[i]; j++)
            for(int k =  Net->l[i].n[j].num_connections - 1; k >= 0; k--) // follow the crop of the 'connect' list
                disconnectNeuron(Net->l[i].n[j].connect[k].n, getneuronP(Net, i, j)); // disconnect all the neurons



    while(index < size)
    {
        int number_of_connections = data[index+2];

        if(number_of_connections > 0)  // if the number of connections are > 0
        {
            for(int i = 0; i < number_of_connections; i++)
            {
                int src_l, src_n, dest_l, dest_n;
                float c_weight;

                src_l = data[index];
                src_n = data[index+1];
                dest_l = data[index+3+i*3];
                dest_n = data[index+3+i*3+1];
                c_weight = data[index+3+i*3+2];

                connectNeuron(getneuronP(Net, dest_l, dest_n), getneuronP(Net, src_l, src_n));
                setWeight(getneuronP(Net, dest_l, dest_n), getneuronP(Net, src_l, src_n), c_weight);
            }
        }

        index += number_of_connections * 3 + 3;

    }


    fclose(file);
    return Net;

}

/// *******************************************************************************************************

void connectNeuron(neuron * src_neuron, neuron * dest_neuron)
{

    for(int i = 0; i < dest_neuron->num_connections; i++)
        if(dest_neuron->connect[i].n == src_neuron) // if the two neurons are already connected
            return; // abort

    dest_neuron->num_connections++; // We add the neuron to the "address book"
    connection * connect = (connection *) malloc ( sizeof(connection) * dest_neuron->num_connections ); // allocate memory to process the 'connect' array

    memcpy(connect, dest_neuron->connect, sizeof(connection) * (dest_neuron->num_connections-1)); // setup the connection
    connect[dest_neuron->num_connections-1].n = src_neuron;
    connect[dest_neuron->num_connections-1].weight = randomfloat();

    free(dest_neuron->connect); // copy the new "address book"
    dest_neuron->connect = (connection *) malloc ( sizeof(connection) * dest_neuron->num_connections );
    memcpy(dest_neuron->connect, connect, sizeof(connection) * dest_neuron->num_connections);

    free(connect);

    return;
}

/// *******************************************************************************************************

void disconnectNeuron(neuron * src_neuron, neuron * dest_neuron)
{

    int index = -1;

    for(int i = 0; i < dest_neuron->num_connections; i++)
        if(dest_neuron->connect[i].n == src_neuron) // search for the neuron that we want to disconnect
        {
            index = i;
            break;
        }

    if(index == -1) // if we do not find that neuron
        return; // abort


    dest_neuron->num_connections--;
    connection *connect = (connection *) malloc ( sizeof ( connection ) * dest_neuron->num_connections );

    memcpy( connect, dest_neuron->connect, index * sizeof(connection)); // setup the new connect array - crop the disconnected neuron
    memcpy( connect + index, dest_neuron->connect + index + 1, (dest_neuron->num_connections-index) * sizeof(connection));

    free(dest_neuron->connect);
    dest_neuron->connect = (connection *) malloc ( sizeof ( connection ) * dest_neuron->num_connections );
    memcpy(dest_neuron->connect, connect, sizeof ( connection ) * dest_neuron->num_connections);


    free(connect);

    return;
}

/// *******************************************************************************************************

void setWeight(neuron * src_neuron, neuron * dest_neuron, float weight)
{

    int index = -1;

    for(int i = 0; i < dest_neuron->num_connections; i++)
        if(dest_neuron->connect[i].n == src_neuron)
        {
            index = i;
            break;
        }

    if(index == -1)
        return;


    dest_neuron->connect[index].weight = weight;

    return;
}

#endif // HEADLESS_NEAT_H
