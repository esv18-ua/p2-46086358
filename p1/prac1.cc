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

const char PROJECT_NAME[]="Enter project name: ";
const char PROJECT_DESCRIPTION[]="Enter project description: ";
const char LIST_NAME[]="Enter list name: ";
const char TASK_NAME[]="Enter task name: ";
const char TASK_DEADLINE[]="Enter deadline: ";
const char TASK_TIME[]="Enter expected time: ";
const int MAX_TIME=180;
const int MIN_TIME=1;

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

Date extraerFecha(Date fecha,string str_fecha){
	size_t pos1=str_fecha.find_first_of("/");
	size_t pos2=str_fecha.find_last_of("/");
	size_t pos3=pos2-pos1;
	string str_day=str_fecha.substr(0,pos1);
	string str_month=str_fecha.substr(pos1+1,pos3-1);
	string str_year=str_fecha.substr(pos2+1);
	
	fecha.day = stoi(str_day);
	fecha.month = stoi(str_month);
	fecha.year =stoi(str_year);

	return (fecha);
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
	else{
		verifico=false;
		error(ERR_DATE);
	}	
	return(verifico);
}

bool cadenaVacia(string nombre){
	bool compruebo =true;
	if (nombre.empty())
		error(ERR_EMPTY);
	else
		compruebo=false;
	
	return (compruebo);
}

unsigned recorreVectorLists(string nombre, Project  proyecto){
	bool encontrado=false;
	unsigned i;
	for(i=0;i<proyecto.lists.size() && !encontrado;i++){
		if (nombre==proyecto.lists[i].name)
			encontrado=true;
	}
	if (!encontrado)
		i=0;
	
	return (i);
}

unsigned recorreVectorTasks(string nombre, Project proyecto,unsigned j){
	bool encontrado=false;
	unsigned i;
	for(i=0;i<proyecto.lists[j].tasks.size() && !encontrado;i++){
		if (nombre==proyecto.lists[j].tasks[i].name)
			encontrado=true;
	}
	if (!encontrado)
		i=0;
	
	return (i);
}

bool comprueboTime( int time){
	bool compruebo =false;
	if(MIN_TIME<=time && time<=MAX_TIME)
		compruebo =true;
	else
		error(ERR_TIME); 
	return (compruebo);
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
	string nombre; 
	do{
	cout<<PROJECT_NAME;
	getline(cin,nombre);
	if (!cadenaVacia(nombre)){
		cout<<PROJECT_DESCRIPTION;
		getline(cin,toDoList.description);
		toDoList.name=nombre;
		}
	}while(cadenaVacia(nombre));
}

void addList(Project &toDoList){
	List lista;
	do{
	cout<<LIST_NAME;
	getline(cin,lista.name);
	if(!cadenaVacia(lista.name)){
		unsigned j= recorreVectorLists(lista.name, toDoList);
		if (j != 0 )
			toDoList.lists.push_back(lista);
		else 
			error(ERR_LIST_NAME);
		}	
	}while(cadenaVacia(lista.name));
}

void deleteList(Project &toDoList){
	string nombre;
	do{
	cout<<LIST_NAME;
	getline(cin,nombre);
	if(!cadenaVacia(nombre)){
		unsigned j= recorreVectorLists(nombre, toDoList);
		if ( j!=0)
			error(ERR_LIST_NAME);
		else
			toDoList.lists.erase(toDoList.lists.begin()+j);
		}
	}while(cadenaVacia(nombre));
}

void addTask(Project &toDoList){
	string nombre,str_fecha;
	Task tarea;
	Date fecha;
	do{
	cout<<LIST_NAME;
	getline(cin,nombre);
	if(!cadenaVacia(nombre)){
		unsigned j= recorreVectorLists(nombre, toDoList);
		if ( j!=0){
			cout<<TASK_DEADLINE;
			getline(cin,str_fecha);
			fecha=extraerFecha(fecha,str_fecha);
			if(comprueboDate(fecha)){
				cout<<TASK_TIME;
				cin>>tarea.time;
				if(comprueboTime(tarea.time)){
					tarea.isDone=false;
					toDoList.lists[j].tasks.push_back(tarea);
				}
			}
		}
		else
			error(ERR_LIST_NAME);	
		}
	}while(nombre.empty());
}

void deleteTask(Project &toDoList){
	string nombre_list,nombre_task;
	do{
		cout<<LIST_NAME;
		getline(cin,nombre_list);
		if(!cadenaVacia(nombre_list)){
			unsigned j= recorreVectorLists(nombre_list, toDoList);
			if (j!=0){
				cout<<TASK_NAME;
				getline(cin,nombre_task);
				unsigned k = recorreVectorTasks(nombre_task, toDoList,j);
				if (k!=0)
					toDoList.lists[j].tasks.erase(toDoList.lists[j].tasks.begin()+k);				
				else
				error(ERR_TASK_NAME);
			}
			else 
				error(ERR_LIST_NAME);
		}
	}while(cadenaVacia(nombre_list));
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
