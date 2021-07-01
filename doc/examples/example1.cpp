/**
  * Welcome to example1.cpp of the neat.h library.
  * This example code is based on the example0.cpp, better read that file first
  * In this example code we will learn some more advanced functions of the library.
  * How to use : connectNeuron, disconnectNeuron, setWeight, mutateNet, importNet, exportNet, copyNet
  * Good luck.
  *
  * (This example is based on the v1.1 of the neat.h library) - (c) Algorithm X, 1/08/2020
  **/


#include"../../src/headless-neat.h" // Include the main header file

int main(void){ // Main function

    int nmap[] = {3, 1}; // Create a 3-1 network
    network *Net = confNet(2, nmap);

    float input_data[] = {1, -1, 0.5}; // Test data

    setWeight(getneuronP(Net, 0, 0), getneuronP(Net, 1, 0), 1); // set the weight to 1 for all the connections
    setWeight(getneuronP(Net, 0, 1), getneuronP(Net, 1, 0), 1); // neuron *getneuronP ( network * /* a pointer to the provided network */, int L /* the layer of the neuron */, int N /* what neuron to select */ )
    setWeight(getneuronP(Net, 0, 2), getneuronP(Net, 1, 0), 1);


    setNet(Net, input_data); // Get the Output of that Network
    feedforward(Net);
    printNet(Net, 1);

    // Now lets copy the Network to a new place to play with it safely

    network * Net_Copy = copyNet(Net); // create a copy for the Network 'Net'

    mutateNet(Net_Copy, 1, 25); // mutate the network 'Net_Copy', at a total mutation , with a 25% chance for each connection to get mutated

    setNet(Net_Copy, input_data); // Get a final output
    feedforward(Net_Copy);
    printNet(Net_Copy, 1);

    // Now lets disconnect the 0,2 neuron from the 1,0 neuron

    disconnectNeuron(getneuronP(Net_Copy, 0, 2), getneuronP(Net_Copy, 1, 0));

    setNet(Net_Copy, input_data); // Get a final output
    feedforward(Net_Copy);
    printNet(Net_Copy, 1);

    // Now lets disconnect the 0,1 from 1,0 and connect again the 0,2 to the 1,0 neuron

    disconnectNeuron(getneuronP(Net_Copy, 0, 1), getneuronP(Net_Copy, 1, 0));
    connectNeuron(getneuronP(Net_Copy, 0, 2), getneuronP(Net_Copy, 1, 0));

    setNet(Net_Copy, input_data); // Get a final output
    feedforward(Net_Copy);
    printNet(Net_Copy, 1);

    // Now lets connect the 0,1 to the 1,0 neuron

    connectNeuron(getneuronP(Net_Copy, 0, 1), getneuronP(Net_Copy, 1, 0));

    setNet(Net_Copy, input_data); // Get a final output
    feedforward(Net_Copy);
    printNet(Net_Copy, 1);

    // Now lets export the Net_copy to a file called 'Net_Copy.bin'
    exportNet(Net_Copy, "Net_Copy.bin");

    // And import it again

    network *Copy_of_Net_Copy = importNet("Net_Copy.bin");

    printNet(Copy_of_Net_Copy, 1);

    return 0; // exit
}
