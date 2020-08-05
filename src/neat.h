/**
  *
  * Official Copy of NEAT.h library
  * Copywrite (c) Algorithm X, Greece
  * Current version : v1.1
  *
  *
  **/

#ifndef NEAT_H
#define NEAT_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<graphics.h>

class neuron;
class layer;
class network;

#ifndef linux // if we aren't at a linux system

#include<conio.h>
#include<time.h>

int NEAT2_DRAW_NET_CW = 0; // current_window -- defined a long name to avoid errors
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

#ifndef linux

void drawNet(network *Net, int window, int d)
{

    int w = getcurrentwindow();

    setcurrentwindow(window);

    int oldv = NEAT2_DRAW_NET_CW; // save the cw var to a tmp int

    if(!d)  // if we have not a direct output
    {
        setactivepage(NEAT2_DRAW_NET_CW); // change the pages and display the old page
        setvisualpage(1 - NEAT2_DRAW_NET_CW);
    }
    else
        setvisualpage(NEAT2_DRAW_NET_CW); // write direct to the screen


    cleardevice();

    char buffer[400];

    int maxx = getmaxx(), maxy = getmaxy();

    for(int i = 1; i < Net->layers; i++)
    {
        for(int j = 0; j < Net->nmap[i]; j++)
        {
            for(int k = 0; k < Net->l[i].n[j].num_connections; k++)
            {
                    if(Net->l[i].n[j].connect[k].weight > 0)
                        setcolor(RGB(255, 0, 0));
                    else
                        setcolor(RGB(0, 0, 255));

                    setlinestyle( SOLID_LINE, 0, abs(Net->l[i].n[j].connect[k].weight * 9)); // set the thickness of the line

                    line((Net->l[i].n[j].connect[k].n->l+1) * (maxx / (Net->layers + 1)) + 25 + 1, (Net->l[i].n[j].connect[k].n->n+1) * (maxy / (Net->nmap[Net->l[i].n[j].connect[k].n->l] + 1)), (i+1) *  (maxx / (Net->layers+1)) - 25 - 1, (j+1) * (maxy / (Net->nmap[i] + 1)));

                    setlinestyle(SOLID_LINE, 0, 1);
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

    if(x1 > xmax || x2 > xmax || y1 > ymax || y2 > ymax || x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0)
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

#endif // NEAT_H
