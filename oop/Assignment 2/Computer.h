/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */
#ifndef COMPUTER_H
#define COMPUTER_H
//this header file contains Computer, GPU and CPU classes
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
using std::string;
//There is no need for destructors. That's why I didn't write them.
class ALU{
    public: 
        ~ALU()=default;                 // default destructor, actually there is no need
        ALU(int);                       // constructor with parameter
        int add(int,int) const;         // add method
        int subtract(int,int) const;    // subtract method
        int multiply(int,int) const;    // multiply method
    private:
        const int numPerCores{};        // numbers of ALU per each core

};

class CUDA{
    public: 
        CUDA(int);                      // constructor with parameter
        string render() const;          // render method
        string trainModel() const;      // trainModel method
    private:
        const int numCores{};           // numbers of CUDA cores
};

class CPU{
    public: 
        CPU(int);                       // constructor with parameter
        int execute(string) const;      // execute method
    private:
        const ALU ALU_object;           // composition
};

class GPU {
    public: 
        GPU(int);                       // constructor with parameter
        string execute(string) const;   // execute method
    private:
        const CUDA CUDA_object;         // composition
};

class Computer {
    public: 
        Computer();                     // default constructor
        // pass by reference is more efficient and since CPU and GPU objects are const it cannot be changed
        bool operator+(const CPU&);     // + operator for CPU objects
        bool operator+(const GPU&);     // + operator for GPU objects
        void execute(string) const;     // execute method
    private:
        const CPU* attachedCPU{NULL};   // aggregation Computer object cannot change attributes of CPU and GPU
        const GPU* attachedGPU{NULL};   // aggregation
};

#endif