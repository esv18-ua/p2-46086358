//DNI 46086358Q SABATER VILLORA, EVA 


//funciones añadidas: extraerFecha,compararFechas,compruebodate,
//cadenaVacia,cadenaVaciaError,recorreLista,recorreProyecto,comprueboTime,oldMenu


#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <fstream> 
#include <cstring>

using namespace std;

//Constantes
const char PROJECT_NAME[]="Enter project name: ";
const char PROJECT_DESCRIPTION[]="Enter project description: ";
const char LIST_NAME[]="Enter list name: ";
const char TASK_NAME[]="Enter task name: ";
const char TASK_DEADLINE[]="Enter deadline: ";
const char TASK_TIME[]="Enter expected time: ";
const char PROJECT_ID[]="Enter project id: ";
const char FILE_NAME[]="Enter filename: ";
const char SAVE_PROJECTS[]="Save all projects [Y/N]?: ";
const char CONFIRM_BIN[]="Confirm [Y/N]?: ";
const int MAX_TIME=180;
const int MIN_TIME=1;
const int KMAXNAME=20;
const int KMAXDESC=40;
const int KFECHAMAX=2100;
const int KFECHAMIN=2000;
const int KNOMFICHERO=80;

//Esctructuras de datos
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

//Estructursa de datos para los ficheros binarios
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
       << "b- Back to main menu" << endl
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

//muestra mensaje error(ERR_EMPTY)
void cadenaVaciaError(bool compruebo){
	if (compruebo)
	error(ERR_EMPTY);
	
}

//recorre una lista, se le pasa el nombre de la lista 
//que queremos encontrar y el id del proyecto
unsigned recorreLista(string nombre,ToDo toDoProject, unsigned id){
	unsigned i=0;
	bool encontrado=false;
	for(i=0;i<toDoProject.projects[id].lists.size() && !encontrado;i++){
		if(nombre==toDoProject.projects[id].lists[i].name)
			encontrado=true;
	}
	i--;
	if(!encontrado){
		i=toDoProject.projects[id].lists.size();
		error(ERR_LIST_NAME);	
	} 
	return (i);
}
//devuelve un unsigned con la posicion de la lista con el mismo nombre pedido

//recorre proyecto en busca del nombre solicitado
unsigned recorreProyecto(string nombre,ToDo toDoProject){
	unsigned i=0;
	bool encontrado=false;
	for(i=0;i<toDoProject.projects.size() && !encontrado;i++){
		if(nombre==toDoProject.projects[i].name)
			encontrado=true;
	}
	i--;
	if(!encontrado)
		i=toDoProject.projects.size();
	return (i);
}
//le pasamos el nombre del proyecto y devuelve su posicion en un unsigned

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

void editProject(ToDo &toDoProject,unsigned id){
	string nombre; 
	do{
	cout<<PROJECT_NAME;
	getline(cin,nombre);
	if (!cadenaVacia(nombre)){
		cout<<PROJECT_DESCRIPTION;
		getline(cin,toDoProject.projects[id].description);
		toDoProject.projects[id].name=nombre;
		}
	else 
		cadenaVaciaError(cadenaVacia(nombre));
	}while(cadenaVacia(nombre));
}

void addList(ToDo &toDoProject,unsigned id){
	List lista;
	bool encontrado=false;
	do{
	cout<<LIST_NAME;
	getline(cin,lista.name);
	if(!cadenaVacia(lista.name)){
		for(unsigned i=0;i<toDoProject.projects[id].lists.size();i++){
		if(lista.name==toDoProject.projects[id].lists[i].name)
		encontrado=true;
		}	
		if(encontrado) 
			error(ERR_LIST_NAME);	
		else 
			toDoProject.projects[id].lists.push_back(lista);
		}	
		else 
			error(ERR_EMPTY);
	}while(cadenaVacia(lista.name));
}

void deleteList(ToDo &toDoProject,unsigned id){
	string nombre;
	unsigned i;
	do{
	cout<<LIST_NAME;
	getline(cin,nombre);
	if(!cadenaVacia(nombre)){
		i =recorreLista(nombre,toDoProject,id);	
		if(i<toDoProject.projects[id].lists.size())
			toDoProject.projects[id].lists.erase(toDoProject.projects[id].lists.begin()+i);
		}
	else
		cadenaVaciaError(cadenaVacia(nombre));
	}while(cadenaVacia(nombre));
}

void addTask(ToDo &toDoProject,unsigned id){
	string nombre_list,str_fecha;
	Task tarea;
	unsigned i=0;
	bool encontrado=false;
	do{
	cout<<LIST_NAME;
	getline(cin,nombre_list);
	if(!cadenaVacia(nombre_list)){
		for(i=0;i<toDoProject.projects[id].lists.size() && !encontrado ;i++){
		if(nombre_list==toDoProject.projects[id].lists[i].name){
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
				toDoProject.projects[id].lists[i].tasks.push_back(tarea);	
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

void deleteTask(ToDo &toDoProject,unsigned id){
	string nombre_list,nombre_task;
	bool encontrado=false;
	unsigned i, j=0;
	do{
		cout<<LIST_NAME;
		getline(cin,nombre_list);
		if(!cadenaVacia(nombre_list)){
			i = recorreLista(nombre_list,toDoProject,id);		
			if (i<toDoProject.projects[id].lists.size()){
				cout<<TASK_NAME;
				getline(cin,nombre_task);
			for(j=0;j<toDoProject.projects[id].lists[i].tasks.size() ;j++){
					if(nombre_task==toDoProject.projects[id].lists[i].tasks[j].name){
					toDoProject.projects[id].lists[i].tasks.erase(toDoProject.projects[id].lists[i].tasks.begin()+j);	
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

void toggleTask(ToDo &toDoProject,unsigned id){
	string nombre_list,nombre_task;
	bool encontrado=false,encontrado2=false;
	unsigned i,j;
	do{
	cout<<LIST_NAME;
	getline(cin,nombre_list);
	if(!cadenaVacia(nombre_list)){
		for(i=0;i<toDoProject.projects[id].lists.size() && !encontrado;i++){
		if(nombre_list==toDoProject.projects[id].lists[i].name){
		encontrado=true;
		i--;}
		}
		if(encontrado){
			cout<<TASK_NAME;
			getline(cin,nombre_task);
			for(j=0;j<toDoProject.projects[id].lists[i].tasks.size();j++){
			if(nombre_task==toDoProject.projects[id].lists[i].tasks[j].name){
				encontrado2=true;
				if(toDoProject.projects[id].lists[i].tasks[j].isDone==true) 
					toDoProject.projects[id].lists[i].tasks[j].isDone=false;
				else 
					toDoProject.projects[id].lists[i].tasks[j].isDone=true;	
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

void report(const ToDo &toDoProject,unsigned id){
	string nombre,nombre_anterior;
	Date fecha_min={31,12,2100};
	int total_pendientes=0,total_realizadas=0,total_time_done=0,total_time_left=0;
	cout<<"Name: "<<toDoProject.projects[id].name<<endl;
	if (!cadenaVacia(toDoProject.projects[id].description))
		cout<<"Description: "<<toDoProject.projects[id].description<<endl;
		
		
	for(unsigned i=0;i<toDoProject.projects[id].lists.size();i++){
		cout<< toDoProject.projects[id].lists[i].name <<endl;
		
		
		for(unsigned j=0;j<toDoProject.projects[id].lists[i].tasks.size();j++){ //muestra tareas pendientes en orden 
			if(!toDoProject.projects[id].lists[i].tasks[j].isDone){
			cout<<"[ ] ";
			total_pendientes++;
			total_time_left=total_time_left+toDoProject.projects[id].lists[i].tasks[j].time;
			cout<<"("<<toDoProject.projects[id].lists[i].tasks[j].time<<") ";
			cout<<toDoProject.projects[id].lists[i].tasks[j].deadline.year<<"-"<< toDoProject.projects[id].lists[i].tasks[j].deadline.month<<"-"<<toDoProject.projects[id].lists[i].tasks[j].deadline.day<<" : ";
			if(!toDoProject.projects[id].lists[i].tasks[j].name.empty()) 
			cout<<toDoProject.projects[id].lists[i].tasks[j].name<<endl;
					
			if (comprueboFechas(toDoProject.projects[id].lists[i].tasks[j].deadline,fecha_min)) 
				nombre=toDoProject.projects[id].lists[i].tasks[j].name;
				
			fecha_min=compararFechas(toDoProject.projects[id].lists[i].tasks[j].deadline,fecha_min);
			
		}
			
	}
		for(unsigned j=0;j<toDoProject.projects[id].lists[i].tasks.size();j++){ //muestra tareas realizadas en orden
			if(toDoProject.projects[id].lists[i].tasks[j].isDone){
			cout<<"[X] ";
			total_realizadas++;
			total_time_done=total_time_done+toDoProject.projects[id].lists[i].tasks[j].time;
			cout<<"("<<toDoProject.projects[id].lists[i].tasks[j].time<<") ";
			cout<<toDoProject.projects[id].lists[i].tasks[j].deadline.year<<"-"<< toDoProject.projects[id].lists[i].tasks[j].deadline.month<<"-"<<toDoProject.projects[id].lists[i].tasks[j].deadline.day<<" : ";
			if(!toDoProject.projects[id].lists[i].tasks[j].name.empty()) 
			cout<<toDoProject.projects[id].lists[i].tasks[j].name<<endl;
			else if(toDoProject.projects[id].lists[i].tasks[j].name.empty())
				cout<<endl;
		}
		}
	}

	cout<<"Total left: "<<total_pendientes<<" ("<<total_time_left<<" minutes)"<<endl;
	cout<<"Total done: "<<total_realizadas<<" ("<<total_time_done<<" minutes)"<<endl;
	if(total_pendientes !=0) cout<<"Highest priority: "<<nombre<<" ("<<fecha_min.year<<"-"<<fecha_min.month<<"-"<<fecha_min.day<<")"<<endl;
}

//main menu de la practica 1
void oldMenu(ToDo &toDoProject, unsigned id){
	char option;

  do{
    showProjectMenu();
    cin >> option;
    cin.get();
    
    switch(option){
      case '1': editProject(toDoProject,id);
                break;
      case '2': addList(toDoProject,id);
                break;
      case '3': deleteList(toDoProject,id);
                break;
      case '4': addTask(toDoProject,id);
                break;
      case '5': deleteTask(toDoProject,id);
                break;
      case '6': toggleTask(toDoProject,id);
                break;
      case '7': report(toDoProject,id);
                break;
      case 'b': break;
      default: error(ERR_OPTION);
    }
  }while(option!='b');
  
  }


void projectMenu(ToDo &toDoProject){
	int id2;
	unsigned i;
	bool encontrado=false;
	cout<<PROJECT_ID;
	cin>>id2;
	for(i=0;i<toDoProject.projects.size() && !encontrado ;i++){
		if(toDoProject.projects[i].id==id2)
		encontrado=true;
	}
	if(encontrado){
		i--;
		oldMenu(toDoProject,i);
	}
	else 
		error(ERR_ID);
}

void addProject(ToDo &toDoProject){
		Project proyecto;
		unsigned i;
	do{
		cout<<PROJECT_NAME;
		getline(cin,proyecto.name);
		if(!cadenaVacia(proyecto.name)){
			i = recorreProyecto(proyecto.name,toDoProject);
			if(i==toDoProject.projects.size()){
			cout<<PROJECT_DESCRIPTION;
			getline(cin,proyecto.description);
			proyecto.id=toDoProject.nextId;
			toDoProject.projects.push_back(proyecto);
			toDoProject.nextId++;
		}	
	}
		else 
			cadenaVaciaError(cadenaVacia(proyecto.name));
	
}while(cadenaVacia(proyecto.name));
}

void deleteProject(ToDo &toDoProject){
	unsigned i;
	int id;
	bool encontrado=false;
	cout<<PROJECT_ID;
	cin>>id;
	for (i=0;i<toDoProject.projects.size() &&!encontrado;i++){
		if(toDoProject.projects[i].id==id){
		toDoProject.projects.erase(toDoProject.projects.begin()+i);
		encontrado=true;
		
	}
}
	if (!encontrado)
		error(ERR_ID);
}

void importProjects(ToDo &toDoProject, string iFichero,bool argumento_i){
	string linea,tiempo,fecha,done;
	Project proyecto;
	List lista;
	Task tarea;
	ifstream ficheroLec;
	if(!argumento_i){//si parametro es false, se pide introducir manualmente el nombre del fichero
  	cout<<FILE_NAME;
	getline(cin,iFichero);
}
	ficheroLec.open(iFichero); 
	if(ficheroLec.is_open()){
		
		while(getline(ficheroLec,linea)){
			
			
			if(linea.front()=='<'){
				proyecto.id=toDoProject.nextId;
				proyecto.description.clear();
				proyecto.name.clear();
				proyecto.lists.clear();
				lista.tasks.clear();
				lista.name.clear();

			}
			else if(linea.front()=='#')
				proyecto.name = linea.substr(1,linea.size()-1);
			
			else if(linea.front()=='*')
				proyecto.description = linea.substr(1,linea.size()-1);
		
			else if(linea.front()=='@'){
			if(!lista.name.empty()){
					proyecto.lists.push_back(lista);
				}
				lista.name = linea.substr(1,linea.size()-1);
				lista.tasks.clear();
			}
			else if(linea.front()=='>')	{
				if(!lista.name.empty())
					proyecto.lists.push_back(lista);	
				
				toDoProject.projects.push_back(proyecto);
				toDoProject.nextId++;
				lista.tasks.clear();
				proyecto.lists.clear();
				lista.name.clear();
				}
			
			else { 
				if(linea.front()=='|'){
					size_t pos = linea.find_last_of("|");
					tarea.name=" ";
					tiempo=linea.substr(pos+1, linea.back());
					done=linea.substr(pos-1,1);
					fecha=linea.substr(1,pos-3);
				}
				else{
					size_t pos = linea.find_first_of("|");
					size_t pos2 = linea.find_last_of("|");
					size_t pos3 = pos2-pos;
					tarea.name=linea.substr(0,pos);
					tiempo=linea.substr(pos2+1,linea.back());
					done=linea.substr(pos2-1,1);
					fecha=linea.substr(pos+1,pos3-3);
				}
				if(done=="F")
					tarea.isDone=false;
				else if(done=="T")
					tarea.isDone=true;
			
				tarea.deadline=extraerFecha(fecha);
				tarea.time=stoi(tiempo);
			
				if(comprueboDate(tarea.deadline) && comprueboTime(tarea.time))
					lista.tasks.push_back(tarea);
				
				else if (!comprueboTime(tarea.time))
					error(ERR_TIME);	
				else if(!comprueboDate(tarea.deadline))
					error(ERR_DATE);
				
					
			}
		}
		ficheroLec.close();
	}
	else
		error(ERR_FILE);
}

void exportProjects(ToDo &toDoProject){
	char fichero[KNOMFICHERO];
	ofstream ficheroEsc;
	bool opcion=false;
	
	char option;
	int id;
	unsigned n;
	bool encontrado=false;
	do{
		cout<<SAVE_PROJECTS;
		cin>>option;
		if(option=='Y' || option=='y')
			opcion=true;
		if(option=='n' || option=='N'){
		cout<<PROJECT_ID;
		cin>>id;
		for (n=0;n<toDoProject.projects.size() &&!encontrado;n++){
			if(toDoProject.projects[n].id==id)
			encontrado=true;
			}
		if(!encontrado)
			error(ERR_ID);
		}
	}while (option != 'Y' && option != 'y' && option!='N' && option !='n');
	
	cout<<FILE_NAME;
	cin>>fichero;
	
	ficheroEsc.open(fichero);
	if(ficheroEsc.is_open()){
	
	if(opcion){
		//almacena tooda la informacion que haya 
		for(unsigned i=0;i<toDoProject.projects.size();i++){
			ficheroEsc << "<"<<endl;
			ficheroEsc <<"#"<<toDoProject.projects[i].name<<endl;
			if(!toDoProject.projects[i].description.empty())
			ficheroEsc <<"*"<<toDoProject.projects[i].description<<endl;
			for(unsigned j=0;j<toDoProject.projects[i].lists.size();j++){
				ficheroEsc<<"@"<<toDoProject.projects[i].lists[j].name<<endl;
				for(unsigned k=0;k<toDoProject.projects[i].lists[j].tasks.size();k++){
				if(toDoProject.projects[i].lists[j].tasks[k].name != " ")
					ficheroEsc<< toDoProject.projects[i].lists[j].tasks[k].name;
				ficheroEsc<<"|"<< toDoProject.projects[i].lists[j].tasks[k].deadline.day<<"/"<<toDoProject.projects[i].lists[j].tasks[k].deadline.month<<"/"
				<<toDoProject.projects[i].lists[j].tasks[k].deadline.year;
				if(toDoProject.projects[i].lists[j].tasks[k].isDone)
					ficheroEsc<< "|T|";
				else
					ficheroEsc<<"|F|";
				ficheroEsc<< toDoProject.projects[i].lists[j].tasks[k].time<<endl;
						}
					}
					ficheroEsc <<">"<<endl;
				}
			
			}	
	if(!opcion){
		n--;
		//solo almacena la informacion del proyecto toDoProject.projects[i]
		ficheroEsc << "<"<<endl;
		ficheroEsc << "#"<< toDoProject.projects[n].name <<endl;
		if (!toDoProject.projects[n].description.empty())
		ficheroEsc << "*" << toDoProject.projects[n].description<<endl;
		for(unsigned i=0;i<toDoProject.projects[n].lists.size();i++){
			ficheroEsc << "@" << toDoProject.projects[n].lists[i].name<<endl;
			for(unsigned j=0;j<toDoProject.projects[n].lists[i].tasks.size();j++){
				if(toDoProject.projects[n].lists[i].tasks[j].name != " ")
				ficheroEsc<< toDoProject.projects[n].lists[i].tasks[j].name;
				ficheroEsc<<"|"<< toDoProject.projects[n].lists[i].tasks[j].deadline.day<<"/"<<toDoProject.projects[n].lists[i].tasks[j].deadline.month<<"/"
				<<toDoProject.projects[n].lists[i].tasks[j].deadline.year;
			if(toDoProject.projects[n].lists[i].tasks[j].isDone)
				ficheroEsc<< "|T|";
			else
				ficheroEsc<<"|F|";
			ficheroEsc<< toDoProject.projects[n].lists[i].tasks[j].time<<endl;
			}
		
		}
		ficheroEsc <<">"<<endl;
	}	
		
		ficheroEsc.close();
	}
else 
	error(ERR_FILE);
}


void loadData(ToDo &toDoProject,string lFichero,bool argumento_l){
	ifstream fichBinario;
	string fichero;	
	char op;
	bool elimino=false;
	
	if(!argumento_l){	//el nombre del fichero se pasa manualmente
	cout<<FILE_NAME;
	getline(cin,fichero);
	fichBinario.open(fichero.c_str(),ios::in | ios::binary);
		if(fichBinario.is_open()){ //si el fichero esta abierto 	
		do{
			cout<<CONFIRM_BIN;
			cin>>op;	
		}while(op!='n' && op!='N' && op!='y' && op!='Y');
		
		if(op=='Y' || op=='y')
			elimino=true; //confirmo que deseo eliminar los datos
			}
				else 
		error(ERR_FILE);
		}
	else{ //el fichero viene dado por los argumentos
		fichBinario.open(lFichero.c_str(),ios::in | ios::binary);
		elimino=true;
		if(!fichBinario.is_open())
		error(ERR_FILE);
		}
		
		
		if(elimino){
		//borrado de todos los datos 
		toDoProject.projects.clear();
		toDoProject.name.clear();
		toDoProject.nextId=1;
		
		BinToDo toDoBin;
		fichBinario.read((char*)&toDoBin, sizeof(toDoBin));
		toDoProject.name=toDoBin.name; //aqui almaceno el nombre del proyecto 
		for(unsigned i=0;i<toDoBin.numProjects;i++){
		BinProject proyectoBin;
		Project proyecto;
		fichBinario.read((char*)&proyectoBin,sizeof(proyectoBin));
		proyecto.name=proyectoBin.name;
		proyecto.description=proyectoBin.description;
		proyecto.id=toDoProject.nextId;
		toDoProject.nextId++;
		for(unsigned j=0;j<proyectoBin.numLists;j++){
			BinList listaBin;
			List lista;
			fichBinario.read((char*)&listaBin,sizeof(listaBin));
			lista.name=listaBin.name;
			for(unsigned k=0;k<listaBin.numTasks;k++){
				BinTask tareaBin;
				Task tarea;
				fichBinario.read((char*)&tareaBin,sizeof(tareaBin));
				tarea.name=tareaBin.name;
				tarea.deadline=tareaBin.deadline;
				tarea.time=tareaBin.time;
				tarea.isDone=tareaBin.isDone;
				
				lista.tasks.push_back(tarea);
			}
			proyecto.lists.push_back(lista);
		}	
		toDoProject.projects.push_back(proyecto);
		}
	}       
		
		fichBinario.close();
	}

void saveData(ToDo &toDoProject){
	//ofstream fichBinario;
	string fichero;	
	cout<<FILE_NAME;
	getline(cin,fichero);
	
	ofstream fichBinario(fichero.c_str(),ios::binary);
	//fichBinario.open(fichero.c_str(),ios::binary);
	if(fichBinario.is_open()){
		BinToDo toDoBin;
		strncpy(toDoBin.name,toDoProject.name.c_str(),19);
		toDoBin.numProjects=toDoProject.projects.size();
		fichBinario.write((const char *)&toDoBin,sizeof(toDoBin));
		
		for(unsigned i=0;i<toDoProject.projects.size();i++){
			BinProject proyectoBin;
			strncpy(proyectoBin.name,toDoProject.projects[i].name.c_str(),19);
			strncpy(proyectoBin.description,toDoProject.projects[i].description.c_str(),39);
			proyectoBin.numLists=toDoProject.projects[i].lists.size();
			fichBinario.write((const char *)&proyectoBin,sizeof(proyectoBin));
			
			for(unsigned j=0;j<toDoProject.projects[i].lists.size();j++){
				BinList lista;
				strncpy(lista.name,toDoProject.projects[i].lists[j].name.c_str(),19);
				lista.numTasks=toDoProject.projects[i].lists.size();
				fichBinario.write((const char*)&lista,sizeof(lista));
				
				for(unsigned k=0;k<toDoProject.projects[i].lists[j].tasks.size();k++){
				BinTask tarea;
				strncpy(tarea.name,toDoProject.projects[i].lists[j].tasks[k].name.c_str(),19);
				tarea.deadline=toDoProject.projects[i].lists[j].tasks[k].deadline;
				tarea.isDone=toDoProject.projects[i].lists[j].tasks[k].isDone;
				tarea.time=toDoProject.projects[i].lists[j].tasks[k].time;
				fichBinario.write((const char*)&tarea,sizeof(tarea));	
				
				}
				
			}
			 
		}
		fichBinario.close();
	}
	else 
		error(ERR_FILE);
		
	}


void summary(ToDo &toDoProject){
	int total_realizadas=0,total_tareas=0;
	for(unsigned i=0;i<toDoProject.projects.size();i++){
		total_realizadas=0;
		total_tareas=0;
		cout<<"("<<toDoProject.projects[i].id<<") ";
		cout<<toDoProject.projects[i].name;
		for(unsigned j=0;j<toDoProject.projects[i].lists.size();j++){
			for(unsigned k=0;k<toDoProject.projects[i].lists[j].tasks.size();k++){
				total_tareas++;
				if(toDoProject.projects[i].lists[j].tasks[k].isDone)
					total_realizadas++;
			}
		}
		
		cout<<"["<<total_realizadas<<"/"<<total_tareas<<"]"<<endl;
	}
}
int main(int argc, char *argv[]){

  ToDo toDoProject;
  toDoProject.name="My ToDo List";
  toDoProject.nextId=1;
 
	string iFichero,lFichero;
   bool argumento_i=false;
   bool argumento_l=false;
   bool error_arg=false;
  
  char option='q';

     for(int i=1; i<argc && !error_arg ;i++){                                                  
     if(strcmp(argv[i],"-i")==0 && !argumento_i){ //solo puede haber un argumento del mismo tipo
        if(argc==i+1){ //El programa finaliza si la opcion no va seguida de un posible fichero
          error(ERR_ARGS);
        error_arg=true;
	}
        if(!error_arg){
        argumento_i=true;
        iFichero=argv[i+1];
		i++;
     }
 }
     else if(strcmp(argv[i],"-l")==0 && !argumento_l ){ //solo puede haber un argumento del mismo tipo
        if(argc==i+1 ){//El programa finaliza si la opcion no va seguida de un posible fichero
          error(ERR_ARGS);            
		error_arg=true;
	}
	if(!error_arg){
        argumento_l=true;
        lFichero=argv[i+1];
        i++;
     }
 }
     else
       error(ERR_ARGS);  
   }

   //ejecucion de los argumentos por orden
  if(argumento_i)
	importProjects(toDoProject,iFichero,argumento_i);
  if(argumento_l)
    loadData(toDoProject,lFichero,argumento_l);
 
	if(!error_arg){
  do{
    showMainMenu();
    cin >> option;
    cin.get();
    
    switch(option){
      case '1': projectMenu(toDoProject);
                break;
      case '2': addProject(toDoProject);
                break;
      case '3': deleteProject(toDoProject);
                break;
      case '4': importProjects(toDoProject,iFichero,argumento_i);
                break;
      case '5': exportProjects(toDoProject);
                break;
      case '6': loadData(toDoProject,lFichero,argumento_l);
                break;
      case '7': saveData(toDoProject);
                break;
      case '8': summary(toDoProject);
				break;
      case 'q': break;
      default: error(ERR_OPTION);
    }
  }while(option!='q' );
}
  return 0;    
}
