/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */
#include "university.h"
//this file contains the bodies of the functions of the student class

//constructor
Student::Student(string Name,double gpa,double gre,double toefl){
    this->name=Name;
    this->GPA=gpa;
    this->GRE=gre;
    this->TOEFL=toefl;
    cout<<name<<" logged in to the system."<<endl;
}

//destructor
Student::~Student(){
    cout<<this->name<<" logged out of the system with "<<this->num_of_app<<" application(s)."<<endl;
}

//copy constructor
Student::Student(const Student& st)
        :Student(st.name,st.GPA,st.GRE,st.TOEFL)    
{
    // I used member initializer list since I wanted to print "... logged in to the system" message
    // And I if don't use the initializer list default copy constructor will copy the num_of_app variable
}

//set_name function
void Student::set_name(const string Name){
    this->name=Name;
}