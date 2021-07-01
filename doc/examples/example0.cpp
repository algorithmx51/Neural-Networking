/**
  * Welcome to example0.cpp of the neat.h library.
  * In this example file is the code of how to create a simple neural network, set the input, feedforward it and get a final output.
  * Good luck.
  *
  * (This example is based on the v1.1 of the neat.h library) - (c) Algorithm X, 1/08/2020
  **/


#include"../../src/headless-neat.h" // Include the main header file

int main(void){ // Main function

    int nmap[] = {3, 2, 1}; // Create a simple 3-2-1 neural network, nmap is the map of that network
    int number_of_layers = sizeof(nmap) / sizeof(int); // calculate how many ints we use for the network map

    network * Net;
    Net = confNet(number_of_layers, nmap); // Create a neural network based on the nmap provided


    float input_data[number_of_layers] = {0.5, 0.7, -1}; // declare an array to store the input of the network
    setNet(Net, input_data); // call setNet to set the input for the provided Network

    feedforward(Net); // call feddforward to feedforward the network

    printNet(Net, 1); // call printNet to print the Network status in a detailed form

    return 0; // exit
}
