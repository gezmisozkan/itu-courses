/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */
#ifndef university_h
#define university_h
//this header file contains university class
#include "student.h"


class University {
    public:
        University(string,double,double,double,double,string=""); // The country variable does not require the user to enter a name, it will automatically be setted to an empty string
        //~University(); I don't need destructor
        void evaluate_student(const Student&); // It evaluates the total point of student 
    private:
        // private variables, users do not need to access them directly
        string name{""};
        string country{""};
        double bias{};
        double w_GPA{}; //these are weight values
        double w_GRE{};
        double w_TOEFL{};
};
#endif