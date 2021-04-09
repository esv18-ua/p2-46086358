//DNI 46086358Q SABATER VILLORA, EVA 
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

const char PROJECT_NAME[]="Enter project name: ";
const char PROJECT_DESCRIPTION[]="Enter project description: ";
const char LIST_NAME[]="Enter list name: ";
const char TASK_NAME[]="Enter task name: ";
const char TASK_DEADLINE[]="Enter deadline: ";
const char TASK_TIME[]="Enter expected time: ";
const int MAX_TIME=180;
const int MIN_TIME=1;
const int KMAXNAME=20;
const int KMAXDESC=40;
const int KFECHAMAX=2100;
const int KFECHAMIN=2000;

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

struct ToDo{
	int nextId;
	string name;
	vector <Project> projects;
};

struct BinTask{
	char name[KMAXNAME];
	Date deadline;
	bool isDone;
	int time;
};

struct BinList{
	char name[KMAXNAME];
	unsigned numTasks;
};

struct BinProject{
	char name[KMAXNAME];
	char description[KMAXDESC];
	unsigned numLists;
};

struct BinToDo{
	char name[KMAXNAME];
	unsigned numProjects;
};
	
enum Error{
  ERR_OPTION,
  ERR_EMPTY,
  ERR_LIST_NAME,
  ERR_TASK_NAME,
  ERR_DATE,
  ERR_TIME,
  ERR_ID,
  ERR_PROJECT_NAME,
  ERR_FILE,
  ERR_ARGS
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
      break;
    case ERR_ID:
      cout << "ERROR: wrong project id" << endl;
      break;
    case ERR_PROJECT_NAME:
      cout << "ERROR: wrong project name" << endl;
      break;
    case ERR_FILE:
      cout << "ERROR: cannot open file" << endl;
      break;
    case ERR_ARGS:
      cout << "ERROR: wrong arguments" << endl;
  }
}

void showProjectMenu(){
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

void showMainMenu(){
	cout << "1- Project menu" << endl
		 << "2- Add project" << endl
		 << "3- Delete project" << endl
		 << "4- Import projects" << endl
		 << "5- Export projects" << endl
		 << "6- Load data" << endl
		 << "7- Save data" << endl
		 << "8- Summary" << endl
		 << "q- Quit" << endl
		 << "Option: ";
		
}



//Función a la cual se le pasa  el string 
//correspondiente a deadline de la tarea
Date extraerFecha(string str_fecha){
	Date fecha;
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
//devuelve la fecha en formato date después de ser 
//convertida de string a int con stoi

//le pasamos la fecha actual de la tarea (fecha) y la fecha anterior almacenada
//para la funcion report, saber highest priority
Date compararFechas(Date fecha, Date fecha_min){
	if(fecha.year==fecha_min.year){
		fecha_min.year=fecha.year;
		if(fecha.month<=fecha_min.month){
			fecha_min.month=fecha.month;
			if(fecha.day<=fecha_min.day)
			fecha_min.day=fecha.day;
		}
	}
	else if (fecha.year<fecha_min.year){
		fecha_min.year=fecha.year;
		fecha_min.month=fecha.month;
		fecha_min.day=fecha.day;
	}
	return(fecha_min);
}
//devuleve la fecha mínima (la que pondremos en highest priority)

//le  pasamos la fecha actual de la tarea y la fecha  ya almacenada 
bool comprueboFechas(Date fecha, Date fecha_min){
	bool comparado=false;
	if(fecha.year==fecha_min.year){
		if(fecha.month==fecha_min.month){
			 if(fecha.day<fecha_min.day)
				comparado=true;
		}
		else if(fecha.month<fecha_min.month)
			comparado=true;
	}
	else if(fecha.year<fecha_min.year)
		comparado=true;
	return(comparado);
}
//devuelve bool que indica si el nombre de l
//a tarea es el ya almacenado o el de la tarea actual	

//le pasamos un tipo date, el que hemos obtenido de la funcion extraerFecha
bool comprueboDate( Date fecha ){
	bool verifico=true;
	if (KFECHAMIN<=fecha.year && fecha.year<=KFECHAMAX){ //si el año esta entre 2000 y 2100
				
	if(fecha.month==2 ){ //si el mes es febrero compruebo si el año es bisiesto
		if (fecha.year%4==0 && fecha.year%100==0 && 
		fecha.year%400==0 && fecha.day>29) //es un año bisiesto
			verifico=false;
		else if (fecha.year%4!=0 && fecha.year%100!=0 &&
		 fecha.year%400!=0 && fecha.day>28)//no es un año bisiesto
			verifico=false;
		else if (fecha.year%4==0 && fecha.year%100==0 &&
		 fecha.year%400!=0 && fecha.day>28)
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
	}	
	return(verifico);
}
//devuelve bool indicando si es una fecha valida, acorde con lo solicitado

//pasamos un string 
bool cadenaVacia(string nombre){
	bool compruebo=false;
	if (nombre.empty())
		compruebo=true;
	return (compruebo);
}
//nos devuelve un bool , true si está vacío y false al contrario

void cadenaVaciaError(bool compruebo){
	if (compruebo)
	error(ERR_EMPTY);
	
}

unsigned recorreLista(string nombre,Project toDoList){
	unsigned i=0;
	bool encontrado=false;
	for(i=0;i<toDoList.lists.size() && !encontrado;i++){
		if(nombre==toDoList.lists[i].name)
			encontrado=true;
	}
	i--;
	if(!encontrado){
		i=toDoList.lists.size();
		error(ERR_LIST_NAME);	
	} 
	return (i);
}


//pasamos el time pedido en addtask
bool comprueboTime( int time){
	bool compruebo =false;
	if(MIN_TIME<=time && time<=MAX_TIME)
		compruebo =true;
	else
		error(ERR_TIME); 
	return (compruebo);
}
//devuelve bool, si está dentro del rango solicitado

void addProject(Project &toDoList, ToDo &toDo){
	cout<<"añade proyecto"<<endl;
}

void deleteProject(Project &toDoList, ToDo &toDo){
	cout<<"elimina proyecto"<<endl;
}

void importProjects(Project &toDoList, ToDo &toDo){
	cout<<"importa proyectos"<<endl;
}

void exportProjects(Project &toDoList, ToDo &toDo){
	cout<< "exporta proyectos"<<endl;
}

void loadData(Project &toDoList, ToDo &toDo){
	cout<< "load data"<<endl;
}

void saveData(Project &toDoList, ToDo &toDo){
	cout<<"save dtaa"<<endl;
}

void summary(Project &toDoList, ToDo &toDo){
	cout<<" summaryy"<<endl;
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
	else 
		cadenaVaciaError(cadenaVacia(nombre));
	}while(cadenaVacia(nombre));
}

void addList(Project &toDoList){
	List lista;
	bool encontrado=false;
	do{
	cout<<LIST_NAME;
	getline(cin,lista.name);
	if(!cadenaVacia(lista.name)){
		for(unsigned i=0;i<toDoList.lists.size();i++){
		if(lista.name==toDoList.lists[i].name)
		encontrado=true;
		}	
		if(encontrado) 
			error(ERR_LIST_NAME);	
		else 
			toDoList.lists.push_back(lista);
		}	
		else 
			error(ERR_EMPTY);
	}while(cadenaVacia(lista.name));
}

void deleteList(Project &toDoList){
	string nombre;
	unsigned i;
	do{
	cout<<LIST_NAME;
	getline(cin,nombre);
	if(!cadenaVacia(nombre)){
		i =recorreLista(nombre,toDoList);	
		if(i<toDoList.lists.size())
			toDoList.lists.erase(toDoList.lists.begin()+i);
		}
	else
		cadenaVaciaError(cadenaVacia(nombre));
	}while(cadenaVacia(nombre));
}

void addTask(Project &toDoList){
	string nombre_list,str_fecha;
	Task tarea;
	unsigned i=0;
	bool encontrado=false;
	do{
	cout<<LIST_NAME;
	getline(cin,nombre_list);
	if(!cadenaVacia(nombre_list)){
		for(i=0;i<toDoList.lists.size() && !encontrado ;i++){
		if(nombre_list==toDoList.lists[i].name){
		encontrado=true;
		i--; }
		}
		if(encontrado){ 
			cout<<TASK_NAME;
			getline(cin,tarea.name);
			cout<<TASK_DEADLINE;
			getline(cin,str_fecha);
			tarea.deadline=extraerFecha(str_fecha);
			if(comprueboDate(tarea.deadline)){
				tarea.isDone=false;	
				cout<<TASK_TIME;
				cin>>tarea.time;
				if(comprueboTime(tarea.time))
				toDoList.lists[i].tasks.push_back(tarea);	
			}
			else 
				error(ERR_DATE);
		}
		else
			error(ERR_LIST_NAME);
	}	
	else
		error(ERR_EMPTY);
	}while(nombre_list.empty());
}

void deleteTask(Project &toDoList){
	string nombre_list,nombre_task;
	bool encontrado=false;
	unsigned i, j=0;
	do{
		cout<<LIST_NAME;
		getline(cin,nombre_list);
		if(!cadenaVacia(nombre_list)){
			i = recorreLista(nombre_list,toDoList);		
			if (i<toDoList.lists.size()){
				cout<<TASK_NAME;
				getline(cin,nombre_task);
			for(j=0;j<toDoList.lists[i].tasks.size() ;j++){
					if(nombre_task==toDoList.lists[i].tasks[j].name){
					toDoList.lists[i].tasks.erase(toDoList.lists[i].tasks.begin()+j);	
					encontrado=true;
					j--;
				}
			}
				if(!encontrado)
					error(ERR_TASK_NAME);
			}
		
		}
		else 
			error(ERR_EMPTY);
	}while(cadenaVacia(nombre_list));
}

void toggleTask(Project &toDoList){
	string nombre_list,nombre_task;
	bool encontrado=false,encontrado2=false;
	unsigned i,j;
	do{
	cout<<LIST_NAME;
	getline(cin,nombre_list);
	if(!cadenaVacia(nombre_list)){
		for(i=0;i<toDoList.lists.size() && !encontrado;i++){
		if(nombre_list==toDoList.lists[i].name){
		encontrado=true;
		i--;}
		}
		if(encontrado){
			cout<<TASK_NAME;
			getline(cin,nombre_task);
			for(j=0;j<toDoList.lists[i].tasks.size();j++){
			if(nombre_task==toDoList.lists[i].tasks[j].name){
				encontrado2=true;
				if(toDoList.lists[i].tasks[j].isDone==true) toDoList.lists[i].tasks[j].isDone=false;
				else toDoList.lists[i].tasks[j].isDone=true;	
				}
			}
			if(!encontrado2) 
				error(ERR_TASK_NAME);
		}
		else 
			error(ERR_LIST_NAME);
	}
		else
			error(ERR_EMPTY);
	
	}while(cadenaVacia(nombre_list));
}

void report(const Project &toDoList){
	string nombre,nombre_anterior;
	Date fecha_min={31,12,2100};
	int total_pendientes=0,total_realizadas=0,total_time_done=0,total_time_left=0;
	cout<<"Name: "<<toDoList.name<<endl;
	if (!cadenaVacia(toDoList.description))
		cout<<"Description: "<<toDoList.description<<endl;
	for(unsigned i=0;i<toDoList.lists.size();i++){
		cout<< toDoList.lists[i].name <<endl;
		//HASTA AQUI TODO OK
		
		for(unsigned j=0;j<toDoList.lists[i].tasks.size();j++){ //muestra tareas pendientes en orden 
			if(!toDoList.lists[i].tasks[j].isDone){
			cout<<"[ ] ";
			total_pendientes++;
			total_time_left=total_time_left+toDoList.lists[i].tasks[j].time;
			cout<<"("<<toDoList.lists[i].tasks[j].time<<") ";
			cout<<toDoList.lists[i].tasks[j].deadline.year<<"-"<<	toDoList.lists[i].tasks[j].deadline.month<<"-"<<toDoList.lists[i].tasks[j].deadline.day<<" : ";
			if(!toDoList.lists[i].tasks[j].name.empty()) 
			cout<<toDoList.lists[i].tasks[j].name<<endl;
					
			if (comprueboFechas(toDoList.lists[i].tasks[j].deadline,fecha_min)) 
				nombre=toDoList.lists[i].tasks[j].name;
				
			fecha_min=compararFechas(toDoList.lists[i].tasks[j].deadline,fecha_min);
			
		}
		
		
		
	}
		for(unsigned j=0;j<toDoList.lists[i].tasks.size();j++){ //muestra tareas realizadas en orden
			if(toDoList.lists[i].tasks[j].isDone){
			cout<<"[X] ";
			total_realizadas++;
			total_time_done=total_time_done+toDoList.lists[i].tasks[j].time;
			cout<<"("<<toDoList.lists[i].tasks[j].time<<") ";
			cout<<toDoList.lists[i].tasks[j].deadline.year<<"-"<<	toDoList.lists[i].tasks[j].deadline.month<<"-"<<toDoList.lists[i].tasks[j].deadline.day<<" : ";
			if(!toDoList.lists[i].tasks[j].name.empty()) 
			cout<<toDoList.lists[i].tasks[j].name<<endl;
		}
		//fecha_min=compararFechas(toDoList.lists[i].tasks[j].deadline,fecha_min);
			//if (comprueboFechas(toDoList.lists[i].tasks[j].deadline,fecha_min))   
				//nombre=toDoList.lists[i].tasks[j].name;
		}
	}
	cout<<"Total left: "<<total_pendientes<<" ("<<total_time_left<<" minutes)"<<endl;
	cout<<"Total done: "<<total_realizadas<<" ("<<total_time_done<<" minutes)"<<endl;
	if(total_pendientes !=0) cout<<"Highest priority: "<<nombre<<" ("<<fecha_min.year<<"-"<<fecha_min.month<<"-"<<fecha_min.day<<")"<<endl;
}

void projectMenu(Project &toDoList){
	char option;

  do{
    showProjectMenu();
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
  
  }

int main(){
  Project toDoList;
  ToDo toDo;
  toDo.name="My ToDo List";
  toDo.nextId=1;
  
  toDoList.id=1;
  char option;
  
  do{
    showMainMenu();
    cin >> option;
    cin.get();
    
    switch(option){
      case '1': projectMenu(toDoList);
                break;
      case '2': addProject(toDoList,toDo);
                break;
      case '3': deleteProject(toDoList,toDo);
                break;
      case '4': importProjects(toDoList,toDo);
                break;
      case '5': exportProjects(toDoList,toDo);
                break;
      case '6': loadData(toDoList,toDo);
                break;
      case '7': saveData(toDoList,toDo);
                break;
      case '8': summary(toDoList,toDo);
				break;
      case 'q': break;
      default: error(ERR_OPTION);
    }
  }while(option!='q');
  
  return 0;    
}
