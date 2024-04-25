/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */
#ifndef student_h
#define student_h
//this header file contains student class
#include <iostream>

using std::cout;
using std::endl;
using std::string;

class Student {
    public:
        friend class University; // evaluate_student function needs to acces private variables
        Student(string,double,double,double); // constructor with parameters
        ~Student(); //destructor
        Student(const Student&); // copy constructor
        void set_name(const string); // for setting the name
    private:
        // private variables, users do not need to access them directly
        string name{""};
        double GPA{0};
        double GRE{0};
        double TOEFL{0};
        mutable int num_of_app{0}; // even for const objects this variables should change
};

#endif