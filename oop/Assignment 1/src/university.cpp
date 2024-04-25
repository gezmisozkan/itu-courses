/* @Author
 * Student Name: Özkan Gezmiş
 * Student ID :  150200033
 */
#include "university.h"
//this file contains the bodies of the functions of the university class

//constructor
University::University(string Name,double gpa,double gre,double toefl,double Bias,string Country){
    this->name=Name;
    this->w_GPA=gpa;
    this->w_GRE=gre;
    this->w_TOEFL=toefl;
    this->bias=Bias;
    this->country=Country;
}

//This function evaluates students' overall scores based on their GPA, GRE and TOEFL scores and university weights
void University::evaluate_student(const Student& st){
    st.num_of_app++;
    double total_point{0};
    total_point=(this->bias + (this->w_GPA * st.GPA) + (this->w_GRE * st.GRE) + (this->w_TOEFL * st.TOEFL));
    if(total_point > 0){
        cout<<st.name<<" is admitted to "<<this->name<<"."<<endl;
    }
    else{
        cout<<st.name<<" is rejected from "<<this->name<<"."<<endl;
    }
}