// Zoe Dauphinee, recitation 5, Classes; Delegation; Output Operator; Copy Control

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Section {
    private:
        class StudentRecord {
            friend ostream& operator<<(ostream& os, const StudentRecord& record);
            public:
                StudentRecord(const string& name) : name(name), grades(14, -1) {}
                const string& get_name() const { return name; }
                void addGrade(int new_grade, int week) { grades[week - 1] = new_grade; }            
            private:
                string name;
                vector<int> grades;
        };

        class TimeSlot {
            friend ostream& operator<<(ostream& os, const TimeSlot& time);
            public:
                TimeSlot(const string& day, unsigned hour) : day(day), hour(hour) {}
            private:
                string day;
                unsigned hour;
        };

        string section_name;
        vector<StudentRecord*> records;
        TimeSlot time;

    public:
        Section(const string& name, const string& day, unsigned time) : section_name(name), time(day, time){}
        // destructor
        ~Section() {
            cout << "Section " << section_name << " is being deleted\n";
            for (size_t index = 0; index < records.size(); ++index) {
                cout << "deleting " << records[index]->get_name() << endl;
                delete records[index];
            }
            records.clear();
        }
        // copy control
        Section(const Section& another_section) 
            : section_name(another_section.section_name), time(another_section.time) {
            for (size_t index = 0; index < another_section.records.size(); ++index) {
                StudentRecord* student_copy = new StudentRecord(*another_section.records[index]);
                records.push_back(student_copy);
            }
        }
        
        void addStudent(const string& name);
        void addGrade(const string& student, int grade, int week);

    friend ostream& operator<<(ostream& os, const Section& section);
    friend ostream& operator<<(ostream& os, const StudentRecord& record);
    friend ostream& operator<<(ostream& os, const TimeSlot& time);
};


class LabWorker {
    friend ostream& operator<<(ostream& os, const LabWorker& ta);
    public:
        LabWorker(const string& name) : name(name) {}
        void addSection(Section& section);
        void addGrade(const string& name, unsigned grade, unsigned week);
    private:
        string name;
        Section* section = nullptr;
};



// Test code
void doNothing(Section sec) { cout << sec << endl; }

int main() {
    cout << "Test 1: Defining a section\n";
    Section secA2("A2", "Tuesday", 16);
    cout << secA2 << endl;

    cout << "\nTest 2: Adding students to a section\n";
    secA2.addStudent("John");
    secA2.addStudent("George");
    secA2.addStudent("Paul");
    secA2.addStudent("Ringo");
    cout << secA2 << endl;

    cout << "\nTest 3: Defining a lab worker.\n";
    LabWorker moe( "Moe" );
    cout << moe << endl;

    cout << "\nTest 4: Adding a section to a lab worker.\n";
    moe.addSection( secA2 );
    cout << moe << endl;

    cout << "\nTest 5: Adding a second section and lab worker.\n";
    LabWorker jane( "Jane" );
    Section secB3( "B3", "Thursday", 11 );
    secB3.addStudent("Thorin");
    secB3.addStudent("Dwalin");
    secB3.addStudent("Balin");
    secB3.addStudent("Kili");
    secB3.addStudent("Fili");
    secB3.addStudent("Dori");
    secB3.addStudent("Nori");
    secB3.addStudent("Ori");
    secB3.addStudent("Oin");
    secB3.addStudent("Gloin");
    secB3.addStudent("Bifur");
    secB3.addStudent("Bofur");
    secB3.addStudent("Bombur");
    jane.addSection( secB3 );
    cout << jane << endl;

    cout << "\nTest 6: Adding some grades for week one\n";
    moe.addGrade("John", 17, 1);  
    moe.addGrade("Paul", 19, 1);  
    moe.addGrade("George", 16, 1);  
    moe.addGrade("Ringo", 7, 1);  
    cout << moe << endl;

    cout << "\nTest 7: Adding some grades for week three (skipping week 2)\n";
    moe.addGrade("John", 15, 3);  
    moe.addGrade("Paul", 20, 3);  
    moe.addGrade("Ringo", 0, 3);  
    moe.addGrade("George", 16, 3);  
    cout << moe << endl;

    cout << "\nTest 8: We're done (almost)! \nWhat should happen to all "
         << "those students (or rather their records?)\n";

    cout << "\nTest 9: Oh, IF you have covered copy constructors in lecture, "
         << "then make sure the following call works:\n";
    doNothing(secA2);
    cout << "Back from doNothing\n\n";
}



/*
Section method implementations
*/
void Section::addStudent(const string& name) {
    StudentRecord* new_student = new StudentRecord(name);
    records.push_back(new_student);
}

void Section::addGrade(const string& student, int grade, int week) {
    for (size_t index = 0; index < records.size(); ++index) {
        if (records[index]->get_name() == student) {
            records[index]->addGrade(grade, week);
            break;
        }
    }
}


/*
LabWorker method implementations
*/
void LabWorker::addSection(Section& new_section) {
    section = &new_section;
}

void LabWorker::addGrade(const string& name, unsigned grade, unsigned week) {
    if (section) section->addGrade(name, grade, week);
}


/*
Output Operator implementations
*/

// LabWorker output operator
ostream& operator<<(ostream& os, const LabWorker& ta) {
    if (ta.section == nullptr) {
        os << ta.name << " does not have a section\n";
    }
    else {
        os << ta.name << " has " << *ta.section;
    }
    return os;
}

// Section output operator
ostream& operator<<(ostream& os, const Section& section) {
    os << "Section: " << section.section_name << ", Time slot: " << section.time << ", Students: ";
    if (section.records.size() == 0) {
        os << "None\n";
    }
    else {
        os << endl;
       for (size_t index = 0; index < section.records.size(); ++index) {
            os << *section.records[index] << endl;
        } 
    }
    return os;
}

// StudentRecord output operator
ostream& operator<<(ostream& os, const Section::StudentRecord& record) {
    os << "Name: " << record.name << ", Grades: ";
    for (size_t index = 0; index < record.grades.size(); ++index) {
        os << record.grades[index] << " ";
    }
    return os;
}

// TimeSlot output operator
ostream& operator<<(ostream& os, const Section::TimeSlot& time) {
        os << "[Day: " << time.day << ", Start time: ";
    if (time.hour < 12) {
        os << time.hour << "am";
    }
    else if (time.hour == 12) {
        os << time.hour << "pm";
    }
    else {
        os << (time.hour - 12) << "pm";
    }
    os << "]";
    return os;
}