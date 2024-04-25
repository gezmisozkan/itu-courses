/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */
#include "Computer.h"
//There is no need for destructors. That's why I didn't write for any of them.

//Computer class's methods

// default constructor
Computer::Computer(){
    cout<<"Computer is ready"<<endl;
}
// + operator overloading for CPU objects
bool Computer::operator+(const CPU& CPU_object){
    // Adding new entities to a computer is not possible if other entities are already attached
    if(this->attachedCPU==NULL){
       this->attachedCPU=&CPU_object;    
       cout<<"CPU is attached"<<endl;
       return true;
    }
    else{
        cout<<"There is already a CPU"<<endl;
        return false;
    }
        
}
// + operator overloading for GPU objects
bool Computer::operator+(const GPU& GPU_object){
    // Adding new entities to a computer is not possible if other entities are already attached
    if(this->attachedGPU==NULL){
       this->attachedGPU=&GPU_object;    
       cout<<"GPU is attached"<<endl;
       return true;
    }
    else{
        cout<<"There is already a GPU"<<endl;
        return false;
    }
        
}
//execute method is called by giving the operation name as an argument. 
void Computer::execute(string operation) const{
    int result{};
    // if operations is add, subtract or multiply CPU will be executed
    if(operation == "add" || operation == "subtract" || operation == "multiply"){
        result=this->attachedCPU->execute(operation);
        //printing the result 
        cout<<result<<endl;
    }
    // if operations is render or trainModel then GPU will be executed
    else if(operation == "render" || operation == "trainModel"){
        string output;
        output=this->attachedGPU->execute(operation);
        //printing the result 
        cout<<output<<endl;
    }
}

//CPU class's methods

// constructor with parameter
CPU::CPU(int init)
    :ALU_object{init}
{
    cout<<"CPU is ready"<<endl;
}
//It reads two integers from the keyboard, delegates the operation to the ALU, and returns the result.
int CPU::execute(string operation) const{
    int num1{},num2{};
    int result{};
    // getting numbers
    cout<<"Enter two integers"<<endl;
    cin>>num1>>num2;

    if(operation == "add"){
        result=this->ALU_object.add(num1,num2);
    }
    else if(operation == "subtract"){
        result=this->ALU_object.subtract(num1,num2);
    }
    else if(operation == "multiply"){
        result=this->ALU_object.multiply(num1,num2);
    }
    return result;
}

//GPU class's methods

// constructor with parameter
GPU::GPU(int init)
    :CUDA_object{init}
{
    cout<<"GPU is ready"<<endl;
}
// execute method uses its CUDA for operations and returns the result
string GPU::execute(string operation) const{
    string result;
    if(operation == "render"){
        result= this->CUDA_object.render();
    }
    else if(operation == "trainModel"){
        result= this->CUDA_object.trainModel();
    }
    return result;
}

//ALU class's methods

// constructor with parameter
ALU::ALU(int init)
    :numPerCores{init}
{
    cout<<"ALU is ready"<<endl;
}
//These methods take two integers as arguments and returns the result.
int ALU::add(int num1,int num2) const{
    return num1+num2;
}
int ALU::subtract(int num1,int num2) const{
    return num1-num2;
}
int ALU::multiply (int num1,int num2) const{
    return num1*num2;
}

//CUDA class's methods

// constructor with parameter
CUDA::CUDA(int init)
    :numCores{init}
{
    cout<<"CUDA is ready"<<endl;
}
// when it is called it returns "Video is rendered"
string CUDA::render() const{
    return "Video is rendered";
}
// when it is called it prints "AI Model is trained"
string CUDA::trainModel() const{
    return "AI Model is trained";
}