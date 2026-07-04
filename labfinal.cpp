#include<bits/stdc++.h>
using namespace std;
class submission {
    virtual void showdetails()=0;

};
class individualproject : virtual public submission {
   private:
   int submissionID;
   int totalMarks;
   string projecttype;
   public:
   double studentID;
   string coursecode;

   individualproject(int sub, int tot,string pro){
    submissionID=sub;
    totalMarks=tot;
    projecttype=pro;
   }

   void setmarks(int marks){
    totalMarks=marks;
   }
   int getmarks(){
    return totalMarks;
   }

   void setprojectype(int type){
    projecttype=type;
     
    if(type==1){
        projecttype=+5;
    }
    if(type==2){
        projecttype=+0;
    }
    if(type==3){
        projecttype=-5;
    }
   }
   string getprojecttype(){
    return projecttype;
   }
   
   void showdetails(){
    cout << "Submission ID :" << submissionID << endl;
    cout << "Total Marks : " << totalMarks << endl;
    cout << "Project Type :" << projecttype << endl;
    cout << "Student ID :" << studentID << endl;
    cout << "Course code :" << coursecode << endl;
   }
};

class groupproject : virtual public submission {
    public:
       int submissionID;
   int totalMarks;
   string projecttype;
   public:
   double studentID;
   string coursecode;

   groupproject(int sub, int tot,string pro){
    submissionID=sub;
    totalMarks=tot;
    projecttype=pro;
   }

   void showdetails(){
    cout << "Submission ID :" << submissionID << endl;
    cout << "Total Marks : " << totalMarks << endl;
    cout << "Project Type :" << projecttype << endl;
    cout << "Student ID :" << studentID << endl;
    cout << "Course code :" << coursecode << endl;
    
   }
};


class projectsubmission : public individualproject, public groupproject {
    private:
    int groupsubmissionID;
    int teamsize;
    float groupmarks;
    public:
    string groupID;
    string coursecode;

    projectsubmission(int gsid,int team,float gmarks)
    :individualproject(gsid, gmarks, "Group"), groupproject(gsid, gmarks, "Group") {
        groupsubmissionID = gsid;
        teamsize = team;
        groupmarks = gmarks;
    }
    void showdetails(){
        cout << "Group Submission ID:" << groupsubmissionID << endl;
        cout << "Team size :" << teamsize << endl;
        cout << "Group Marks :" << groupmarks << endl;
        cout << " Group ID :" << groupID << endl;
        cout << "Course Code:" << coursecode <<  endl;
    }
    void updatesubmission(int bounsmarks){
        groupmarks+=bounsmarks;
    }
    void updatesubmission(int bounsmarks,int newtype){
        groupmarks+=bounsmarks;
    groupmarks=newtype;
    }

    void displaysubmissiondetails(){
        cout << "Group Submission ID:" << groupsubmissionID << endl;
        cout << "Team size :" << teamsize << endl;
        cout << "Group Marks :" << groupmarks << endl;
        cout << " Group ID :" << groupID << endl;
        cout << "Course Code:" << coursecode <<  endl;

    }

};

int main(){
    submission *p;
    individualproject i;
    groupproject g;
    p=&i;
    p->showdetails();
    p=&g;
    p->showdetails();
    
    projectsubmission ps(111,5,85.5);

    ps.setmarks(5);
    cout << "New marks:" << ps.getmarks() << endl;  
    ps.setprojectype(7);
    cout <<"NEW Project type:" << ps.getprojecttype() << endl;
    ps.groupID="G1";
    ps.coursecode="CS111";
    ps.showdetails();
    ps.updatesubmission(5);
    ps.updatesubmission(5, 90);
    cout << "After updating marks:" << endl;
    ps.displaysubmissiondetails();

     return 0;

}