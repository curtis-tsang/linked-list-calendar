#include <iostream>
#include <ctime>
#include <chrono>
#include <cstring>
#include <fstream>
using namespace std;

string file = "empty"; // path to a txt file for storing data, replace "empty" before running
const unsigned int items_in_event = 7;

struct Event{
    string event_name;
    string venue;
    unsigned int yyyy;
    unsigned int mm;
    unsigned int dd;
    unsigned int start_time;
    unsigned int end_time;
    Event* next;
};

Event* reload_event(string arr[]){
    Event* new_event = new Event;
    new_event -> event_name = arr[0];
    new_event -> venue = arr[1];
    new_event -> yyyy = stoi(arr[2]);
    new_event -> mm = stoi(arr[3]);
    new_event -> dd = stoi(arr[4]);
    new_event -> start_time = stoi(arr[5]);
    new_event -> end_time = stoi(arr[6]);
    new_event -> next = nullptr;

    return new_event;
}

Event* data_intake(string file){
    Event* head = nullptr;
    Event* previous = nullptr;
    string text = "";
    ifstream ifs(file);
    
    string temp_arr[items_in_event] = {};

    while(text != "9999"){
        for (int i=0; i<items_in_event; i++){
            ifs >> text;
            if (text == "9999") return head;
            temp_arr[i] = text;
        }
        
        Event* current = reload_event(temp_arr);

        if (!head){
            head = current;
            previous = current;
        }
        else{
            previous -> next = current;
            previous = current;
        }
    }
    ifs.close();
    return head;
}

void output_to_store(string file, Event* head){
    Event* temp = head;
    ofstream ofs(file);

    while(temp != nullptr){
        ofs << temp -> event_name << " " << temp -> venue << " " << temp -> yyyy << " "
        << temp -> mm << " " << temp -> dd << " " << temp ->start_time << " " << temp -> end_time << " ";
        temp = temp -> next;
    }
    ofs << "9999";
    ofs.close();
}

string extract_string(const string s, int x, int y){
    string extract = "";
    for (int i=x; i<(y+1); i++){
        extract += s[i];
    }
    return extract;
}

void extract_time(string arr[], const string time){
    arr[0] = extract_string(time, 0, 1);
    arr[1] = extract_string(time, 3, 4);
    arr[2] = extract_string(time, 6, 9);
    arr[3] = extract_string(time, 12, 15);
    arr[4] = extract_string(time, 17, 20);
}

Event* make_new_event(string name, string venue, string arr[]){
    Event* new_event = new Event;
    new_event -> event_name = name;
    new_event -> venue = venue;
    new_event -> dd = stoi(arr[0]);
    new_event -> mm = stoi(arr[1]);
    new_event -> yyyy = stoi(arr[2]);
    new_event -> start_time = stoi(arr[3]);
    new_event -> end_time = stoi(arr[4]);
    new_event -> next = nullptr;

    return new_event;
}

Event* add_new_event(){
    string name, venue, time, time_arr[5] = {};

    cout << "Event Name : \t\t";
    cin >> name;                            if (name == "-1") return nullptr; //quit option
    cout << "Venue : \t\t";
    cin >> venue;                           if (venue == "-1") return nullptr;//quit option
                             //0123456789012345678901
    cout << "Time in format of dd/mm/yyyy.[0000/2359] :\t\t";
    cin >> time;                            if (time == "-1") return nullptr; //quit option

    extract_time(time_arr, time);

    return make_new_event(name, venue, time_arr);
}

bool compare_date(Event* event1, Event* event2){
    bool compare_y = ((event1 -> yyyy)>(event2 -> yyyy));  // 1 means that date 1 is later than date 2
    bool compare_m = ((event1 -> mm)>(event2 -> mm));
    bool compare_d = ((event1 -> dd)>(event2 -> dd));
    bool compare_time = ((event1 -> start_time)>(event2 -> start_time));
    if (compare_y) return true;
    if ((event1 -> yyyy == event2 -> yyyy) && (compare_m)) return true;
    if ((event1 -> yyyy == event2 -> yyyy) && (event1 -> mm == event2 -> mm) && (compare_d)) return true;
    if ((event1 -> yyyy == event2 -> yyyy) && (event1 -> mm == event2 -> mm) && (event1 -> dd == event2 -> dd) && (compare_time)) return true;
    return false;
}

void insert_event(Event*& head, Event* insert_sth){
    Event* tempLast = nullptr;
    Event* tempNode = head;

    if (head == nullptr || compare_date(head, insert_sth)){
        insert_sth -> next = head;
        head = insert_sth;
        return;
    }

    while (compare_date(insert_sth, tempNode) && (tempNode -> next  != nullptr)){
        tempLast = tempNode;
        tempNode = tempNode -> next;
    }

    if (compare_date(insert_sth, tempNode)){
        tempNode -> next = insert_sth;
    }
    else {
        tempLast -> next = insert_sth;
        insert_sth -> next = tempNode;
    }
    return;
}

void print_add_event(Event* head); // just to declare for convenience

Event* delete_event(Event* head){
    string del_event;
    Event* tempNode = head;
    Event* tempLast = nullptr;
    
    if (tempNode == nullptr){
        cout << "WARNING : THERE IS NOTHING TO DELETE !!!" <<endl;
        return head;
    }

    cout << "What event to delete ?" <<endl;
    cin >> del_event;

    if ((tempLast == nullptr) && (tempNode -> event_name == del_event)){
        head = head -> next;
        delete tempLast;
        tempLast = nullptr;
        return head;
    }
    while (tempNode != nullptr){
        if (tempNode -> event_name == del_event){
            tempLast -> next = tempNode -> next;
            delete tempNode;
            tempNode = nullptr;
            return head;
        }
        tempLast = tempNode;
        tempNode = tempNode -> next;
    }
    cout << "WARNING : THERE IS NO SUCH A THING TO DELETE !!!" <<endl;
    return head;
}

void delete_head_event_auto(Event*& head){
    Event* temp = head;
    head = head -> next;
    delete temp;
}

void print_all_event(Event* head){
    Event* temp = head;
    if (temp == nullptr){
        cout << "CONGRATS !!! THERE IS NO EVENT !!! ENJOY YOUR FREE TIME !!!" <<endl;
    }
    while (temp != nullptr){
        cout << "日期 : "<< temp -> yyyy << " 年 " << temp -> mm << " 月 " << temp -> dd << " 號 "<<endl;
        cout << "地點 : "<< temp -> venue <<endl;
        cout << "時間 : "<< temp -> start_time << " - " << temp -> end_time <<endl;
        cout << "\n活動 : "<< temp -> event_name <<endl<<endl;
        temp = temp -> next;
    }
    return;
}

void print_today_event(Event* head, int tdy[]){
    Event* temp = head;
    int count = 0;
    
    if (!temp){
        cout << "WARNING : THERE IS NO EVENT !!!" << endl;
        return;
    }

    while (temp){
        if ((temp -> yyyy == tdy[2]) && (temp -> mm == tdy[1]) && (temp -> dd == tdy[0])){
            cout << "日期 : "<< temp -> yyyy << " 年 " << temp -> mm << " 月 " << temp -> dd << " 號 "<<endl;
            cout << "地點 : "<< temp -> venue <<endl;
            cout << "時間 : "<< temp -> start_time << " - " << temp -> end_time <<endl;
            cout << "\n活動 : "<< temp -> event_name <<endl<<endl;
            count++;
        }
        temp = temp -> next;
    }
    
    if (count > 1) cout << "There are " << count << " events today !!!" <<endl;
    if (count == 1) cout << "There is " << count << " event today !!!" <<endl;
    if (count == 0) cout << "There are NO events today !!!" <<endl;
    return;
}

void tdy_date(string str, int tdy[]); // just for convenience

bool compare_tdy_date(Event* date, int tdy[]){
    if (date -> yyyy < tdy[2]) return false; // false => delete
    if ((date -> yyyy == tdy[2]) && (date -> mm < tdy[1])) return false;
    if ((date -> yyyy == tdy[2]) && (date -> mm == tdy[1]) && (date -> dd < tdy[0])) return false;
    if ((date -> yyyy == tdy[2]) && (date -> mm == tdy[1]) && (date -> dd == tdy[0]) && (date -> end_time < tdy[3])) return false;
    return true;
}

void refresh(Event* &head, int tdy[]){
    time_t now = time(0);
    string refresh_time = ctime(&now);
    tdy_date(refresh_time, tdy);
    
    Event* temp = head;
    if (!temp) return;

    while (temp != nullptr){
        if (!compare_tdy_date(temp, tdy)) delete_head_event_auto(temp);
        if (compare_tdy_date(temp, tdy)) break;
    }

    head = temp;
}

void choose_task(Event*& head, unsigned int& task_NO, int tdy[]){
    enum OPTION {ADD_NEW_EVENT = 1, DELETE_EVENT, PRINT_ALL_EVENT, PRINT_TODAY_EVENT, REFRESH, END_OF_OPTIOIN};
    string option_list[END_OF_OPTIOIN-1] = {"1.Add new event", "2.Delete event", "3.Print all events", "4.Print today's events", "5.Refresh"};

    cout << "Options : " <<endl;
    for (int i=0; i<END_OF_OPTIOIN-1; i++){
        cout << option_list[i] <<endl;
    }
    cout << "\nPress 0 to quit."<<endl;
    cin >> task_NO;
    switch(task_NO){
        case ADD_NEW_EVENT:{
            Event* new_Node = add_new_event(); if (!new_Node) break;
            insert_event(head, new_Node);
            break;
        }
        case DELETE_EVENT:{
            head = delete_event(head);
            break;
        }
        case PRINT_ALL_EVENT:{
            refresh(head, tdy);
            print_all_event(head);
            break;
        }
        case PRINT_TODAY_EVENT:{
            refresh(head, tdy);
            print_today_event(head, tdy);
            break;
        }
        case REFRESH:{
            refresh(head, tdy);
            cout << "\nRefresh successfully !!!\n" <<endl;
            break;
        }
    }
}

void tdy_date(string str, int tdy[]){
    int tdy_dd, tdy_mm, tdy_yyyy, tdy_time;
    string m = extract_string(str,4,6);

    tdy_dd = stoi(extract_string(str,8,9));
    tdy_yyyy = stoi(extract_string(str,20,23));
    tdy_time = stoi(extract_string(str,11,12))*100 + stoi(extract_string(str,14,15));
    if (m == "Jan") tdy_mm = 1; if (m == "Feb") tdy_mm = 2; if (m == "Mar") tdy_mm = 3; if (m == "Apr") tdy_mm = 4;
    if (m == "May") tdy_mm = 5; if (m == "Jun") tdy_mm = 6; if (m == "Jul") tdy_mm = 7; if (m == "Aug") tdy_mm = 8;
    if (m == "Sep") tdy_mm = 9; if (m == "Oct") tdy_mm = 10; if (m == "Nov") tdy_mm = 11; if (m == "Dec") tdy_mm = 12;

    tdy[0] = tdy_dd;
    tdy[1] = tdy_mm;
    tdy[2] = tdy_yyyy;
    tdy[3] = tdy_time;
}

int main(){
    unsigned int repeat_bool_int = 1;

    int tdy[4] = {};
    time_t now = time(0);
    string login_time = ctime(&now);
    cout << endl << "The time now is : " << login_time <<endl;
    tdy_date(login_time, tdy);
    
    if (file == "empty"){cout << "You haven't set the .txt file for storing data" << endl; return 0;}

    Event* head = data_intake(file);
    while(repeat_bool_int){
        choose_task(head, repeat_bool_int, tdy);
    }

    output_to_store(file, head);

    return 0;
}
