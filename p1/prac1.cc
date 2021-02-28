//DNI 46086358Q SABATER VILLORA, EVA 
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

struct Date{
  int day;
  int month;
  int year;
};

struct Task{
  string name;
  Date deadline;
  bool isDone;
  int time;
};

struct List{
  string name;
  vector<Task> tasks;
};

struct Project{
  int id;
  string name;
  string description;
  vector<List> lists;
};

enum Error{
  ERR_OPTION,
  ERR_EMPTY,
  ERR_LIST_NAME,
  ERR_TASK_NAME,
  ERR_DATE,
  ERR_TIME
};

void error(Error e){
  switch(e){
    case ERR_OPTION:
      cout << "ERROR: wrong menu option" << endl;
      break;
    case ERR_EMPTY:
      cout << "ERROR: empty string" << endl;
      break;
    case ERR_LIST_NAME:
      cout << "ERROR: wrong list name" << endl;
      break;
    case ERR_TASK_NAME:
      cout << "ERROR: wrong task name" << endl;
      break;
    case ERR_DATE:
      cout << "ERROR: wrong date" << endl;
      break;
    case ERR_TIME:
      cout << "ERROR: wrong expected time" << endl;
  }
}

bool comprueboDate( Date fecha ){
	bool verifico=true;
	if (2000<=fecha.year && fecha.year<=2100){ //si el año esta entre 2000 y 2100
				
	if(fecha.month==2 ){ //si el mes es febrero compruebo si el año es bisiesto
		if (fecha.year%4==0 && fecha.year%100==0 && 
		fecha.year%400==0 && fecha.day>29) //es un año bisiesto
			verifico=false;
		else if (fecha.year%4!=0 && fecha.year%100!=0 &&
		 fecha.year%400!=0 && fecha.day>28)//no es un año bisiesto
			verifico=false;
			}
	else {
		
		if(fecha.month%2==0 && fecha.month<8 && fecha.day>30) 
			verifico=false;					
		else if(fecha.month%2!=0 && fecha.month<8 && fecha.day>31) 
			verifico=false;
		else if (fecha.month%2==0 && fecha.month>=8 && fecha.day>31)
			verifico=false;
		else if (fecha.month%2!=0 && fecha.month>8 && fecha.day>30)
			verifico=false;
			}
		}
	else
		verifico=false;
			
	return(verifico);
}

void showMainMenu(){
  cout << "1- Edit project" << endl
       << "2- Add list" << endl
       << "3- Delete list" << endl 
       << "4- Add task" << endl
       << "5- Delete task" << endl
       << "6- Toggle task" << endl
       << "7- Report" << endl
       << "q- Quit" << endl
       << "Option: ";
}

void editProject(Project &toDoList){
	string nombre; //compruebo el string antes de almacenarlo con s
	do{
	cout<<"Enter project name: ";
	getline(cin,nombre);
	if (nombre.empty())
		error(ERR_EMPTY);
	else{
		cout<<"enter description: " ;
		getline(cin,toDoList.description);
		toDoList.name=nombre;
		}
	}while(nombre.empty());
}

void addList(Project &toDoList){
	List lista;
	do{
	cout<<"Enter list name: ";
	getline(cin,lista.name);
	if(lista.name.empty())
		error(ERR_EMPTY);
	else {
		for(unsigned i=0;i<toDoList.lists.size();i++){
		if(toDoList.lists[i].name==lista.name)
			error(ERR_LIST_NAME);
		else
			toDoList.lists.push_back(lista);
			}
		}	
	}while(lista.name.empty());
}

void deleteList(Project &toDoList){
	string nombre;
	do{
	cout<<"Enter list name: ";
	getline(cin,nombre);
	if(nombre.empty())
		error(ERR_EMPTY);
	else{
		for(unsigned i=0;i<toDoList.lists.size();i++){
		if(toDoList.lists[i].name==nombre)
			toDoList.lists.erase(toDoList.lists.begin()+i);
		else
			error(ERR_LIST_NAME);
	}
}
	}while(nombre.empty());
}

void addTask(Project &toDoList){
	
	cout<<"Enter list name: ";
	//getline(cin,
	
}

void deleteTask(Project &toDoList){
}

void toggleTask(Project &toDoList){
}

void report(const Project &toDoList){
	cout<<"Name: "<<toDoList.name<<endl;
	if (!toDoList.description.empty())
		cout<<"Description: "<<toDoList.description<<endl;
	//cout<<toDoList.lists.name<<endl;
}

int main(){
  Project toDoList;
  toDoList.id=1;
  char option;
  
  do{
    showMainMenu();
    cin >> option;
    cin.get();
    
    switch(option){
      case '1': editProject(toDoList);
                break;
      case '2': addList(toDoList);
                break;
      case '3': deleteList(toDoList);
                break;
      case '4': addTask(toDoList);
                break;
      case '5': deleteTask(toDoList);
                break;
      case '6': toggleTask(toDoList);
                break;
      case '7': report(toDoList);
                break;
      case 'q': break;
      default: error(ERR_OPTION);
    }
  }while(option!='q');
  
  return 0;    
}
