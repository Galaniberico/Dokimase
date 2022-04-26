//
// Created by Alejandro Galan Perez on 13/3/22.
//

#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

const string VERSION = "2.0.2";

struct TFlags {
    bool Hide = false;
    bool Valgrind = false;
    bool Help = false;
    bool Version = false;
    bool Args = false;
    bool Folder = false;
    string Fdir;
    bool Test = false;
    string Tdir;
    bool Error = false;
};
struct TTest {
    string dir;
    string name;
    string nameCompiled;
};
bool compareByName(const TTest &a, const TTest &b){
    string aa = a.nameCompiled;
    string bb = b.nameCompiled;
    unsigned i = 0;
    while(aa[i] == bb[i])
        i++;
    return aa[i] < bb[i];
}
struct TNecessaryFile{
    string name;
    string cppFile;
    string cppFileDir;
    string hFile;
};
struct TConfig{
    vector<string> outputExtensions;
    bool outputFilenameWithExtension = false;
};
enum TERROR{
    ERR_FOLDER,
    ERR_ARG_REP,
    ERR_FILE_AND_TEST,
    ERR_TOO_ARG,
    ERR_TEST,
    ERR_ARG,
    ERR_ARGS_NOT_TEST,
    ERR_DIR_UNREACHEABLE,
    ERR_FOLDER_EMPTY,
    ERR_CONFIG_FILE,
    ERR_ARGS_VALGRIND,
};
void error (TERROR err, const string &errArg){
    switch (err) {
        case ERR_FOLDER: cerr << "ERROR: La carpeta no ha podido ser leida. Por favor, revisa que este bien escrita la direccion." << endl;
            break;
        case ERR_FOLDER_EMPTY: cerr << "ERROR: La carpeta no contiene tests. Por favor, revisa que este bien escrita la direccion." << endl;
            break;
        case ERR_ARG_REP: cerr << "ERROR: El argumento " << errArg << " se encuentra repetido. Por favor, elimina el elemento repetido." << endl;
            break;
        case ERR_FILE_AND_TEST: cerr << "ERROR: Los argumentos [-f | --folder] y [-t | --test] no pueden usarse juntos." << endl;
            break;
        case ERR_TOO_ARG: cerr << "ERROR: El argumento " << errArg << " no puede usarse con mas argumentos." << endl;
            break;
        case ERR_TEST: cerr << "ERROR: El test no ha podido ser leido. Por favor, revisa que este bien escrita la direccion o que el archivo sea correcto." << endl;
            break;
        case ERR_ARGS_NOT_TEST: cerr << "ERROR: El argumento [-a | --args] no puede usarse sin especificar el test a usar con los argumentos del archivo args.in." << endl;
            break;
        case ERR_ARG: cerr << "ERROR: Comando no reconocido. Usa --help para mas informacion" << endl;
            break;
        case ERR_DIR_UNREACHEABLE: cerr << "ERROR: No se ha podido leer la direccion proporcionada" << endl;
            break;
        case ERR_CONFIG_FILE: cerr << "ERROR: La variable de configuracion \"" + errArg + "\" no se reconoce. Por favor, revise que esta escrita correctamente" << endl;
            break;
        case ERR_ARGS_VALGRIND: cerr << "ERROR: El argumento VALGRIND no puede estar activado a la vez que ARGS. Por favor, elimina uno de los 2 argumentos" << endl;
    }
    exit(0);
}
void argsCheck(int argc, char* args[],TFlags &flag){
    for(int i = 1; i < argc && !flag.Error; i++){
        if(!strcmp("-h", args[i]) || !strcmp("--hide-compilation", args[i])){
            if(!flag.Hide){
                flag.Hide = true;
            } else error(ERR_ARG_REP, args[i]);
            continue;
        }
        if(!strcmp("-val", args[i]) || !strcmp("--valgrind", args[i])){
            if(!flag.Valgrind){
                flag.Valgrind = true;
            } else error(ERR_ARG_REP, args[i]);
            continue;
        }
        if(!strcmp("-f", args[i]) || !strcmp("--folder", args[i])){
            if(!flag.Folder){
                if(i < argc - 1){
                    flag.Fdir = args[i+1];
                    flag.Folder = true;
                    i++;
                } else error(ERR_FOLDER, "");
            }else error(ERR_ARG_REP, args[i]);
            continue;
        }
        if(!strcmp("-t", args[i]) || !strcmp("--test", args[i])){
            if(!flag.Test){
                if(i < argc){
                    flag.Tdir = args[i+1];
                    flag.Test = true;
                    i++;
                } else error(ERR_TEST, "");
            }else error(ERR_ARG_REP, args[i]);
            continue;
        }
        if(!strcmp("--help", args[i])){
            if(argc == 2){
                flag.Help = true;
                return;
            } else error(ERR_TOO_ARG, args[i]);
        }
        if(!strcmp("--version",args[1]) || !strcmp("-v",args[1])){
            if(argc == 2){
                flag.Version = true;
                return;
            } else error(ERR_TOO_ARG, args[i]);
        }
        if(!strcmp("--args",args[i]) || !strcmp("-a",args[i])){
            if(!flag.Args){
                flag.Args = true;
            } else error(ERR_ARG_REP,args[i]);
            continue;
        }
        else error(ERR_ARG, args[i]);
    }
    if(flag.Args && !flag.Test){
        error(ERR_ARGS_NOT_TEST, "");
    }
    if(flag.Args && flag.Valgrind){
        error(ERR_ARGS_VALGRIND, "");
    }
    if(flag.Folder && flag.Test){
        error(ERR_FILE_AND_TEST, "");
    }
}
void help(){
    cout << "\nEl uso correcto de este tester es:" << endl << endl;
    cout << "        doki [-a | --args] [-f | --folder <carpeta:...>] [-h | --hide] [--help] [-t | --test <test:...>] [-val | --valgrind] [-v | --version]" << endl << endl;
    cout << "    -a | --args                Pasa el test especificado una vez por linea de argumentos especificados en args.in.\n                               Es imprescindible que cada linea siga esta sintaxis: nombre_fichero_salida_sin_extension#argumentos" << endl;
    cout << "    -f | --folder <carpeta>    Pasa solo los tests que hayan en las carpetas en cuestion (subcarpetas incluidas). Ej: para ir a\n                               ./tests/ejemplos/, habria que poner -f ejemplos y para ./tests/ejemplos/pruebas1/, habria que\n                               poner ejemplos/pruebas1" << endl;
    cout << "    -h | --hide                RECOMENDADO: Oculta las salidas de las compilaciones de los tests" << endl;
    cout << "         --help                Muestra esta informacion" << endl;
    cout << "    -t | --test <test>         Pasa solo el test en cuestion. Ej: para el test ./tests/ejemplo/test34.cpp habria que poner ejemplo/test34.cpp" << endl;
    cout << "  -val | --valgrind            Pasa valgrind en los test especificados (todos, una carpeta o un test) y comprueba si se libera toda\n                               la memoria y si no hay errores. En caso de que no, guarda la salida en concreto en un archivo en la\n                               carpeta principal." << endl;
    cout << "    -v | --version             Muestra la version y derechos de autor" << endl << endl;
    cout << "   NOTA: Para evitar errores debido a tests que necesitan archivos que no pueden compilar (errores, ...) usar -f o -t para hacer solo los tests\n         que no requieran de esos archivos." << endl << endl;
}
void version(){
    cout << "\nDokimase by Alejandro Galan Perez ©" << endl;
    cout << "Version " << VERSION << endl << endl;
    cout << "       El autor solo se hace responsable del programa que compila, ejecuta y compara ficheros ajenos a él" << endl;
    cout << "y no se hace responsable de la procedencia de estos ni su mal uso. Además queda prohibida cualquier venta" << endl;
    cout << "o distribucion del testeador de forma ajena al autor." << endl << endl;
    cout << "Cualquier reporte de bugs o peticiones de cambio constructivas seran bien recibidas a alexgalanp@gmail.com" << endl << endl;
}
TConfig readConfigFile(){
    TConfig config;
    ifstream f("dokimase.config");
    if(f.is_open()){
        string aux;
        while(getline(f, aux, ' ')){
            if(aux == "outputExtension"){
                getline(f, aux);
                stringstream ss(aux);
                    while(getline(ss, aux, ' '))
                        config.outputExtensions.push_back("." + aux);
                continue;
            }
            if(aux == "outputFilenameWithExtension"){
                getline(f, aux);
                config.outputFilenameWithExtension = aux == "true";
            }
            else
                error(ERR_CONFIG_FILE, aux);
        }
        f.close();
    }
    return config;
}
string changeBarToUnderscore(const string &dir){
    string newDir;
    for(char i : dir){
        newDir += (i == '/') ? '_' : i;
    }
    newDir = newDir.erase(0,8);
    return newDir;
}
string changeBarToArrow(const string &dir){
    string newDir;
    string arrow = " > ";
    for(const char &i : dir){
        if(i == '/')
            newDir += arrow;
        else
            newDir += i;
    }
    newDir = newDir.erase(0,10);
    return newDir;
}
bool isCplusplusFile(const string &name){
    return name.length() >= 3 && name.substr(name.length()-2,2) == ".c" ||
           name.length() >= 4 && name.substr(name.length()-3,3) == ".cc" ||
           name.length() >= 5 && name.substr(name.length()-4,4) == ".cpp";
}
string getTestName(const string &dir){
    stringstream ss(dir);
    string s, name;
    while(getline(ss, s, '/')){
        name = s;
    }
    return name;
}
string eraseExtension(string name){
    if(name.length() >= 3 && name.substr(name.length()-2,2) == ".c"){
        return name.erase(name.length()-2, 2);
    }
    if(name.length() >= 4 && name.substr(name.length()-3,3) == ".cc"){
        return name.erase(name.length()-3, 3);
    }
    if(name.length() >= 5 && name.substr(name.length()-4,4) == ".cpp"){
        return name.erase(name.length()-4, 4);
    }
    if(name.length() >= 3 && name.substr(name.length()-2,2) == ".h"){
        return name.erase(name.length()-2, 2);
    }
    return name;
}
vector<TTest> readFolder(const string &direction){
    DIR *dir;
    struct dirent *ent;
    static vector<TTest> files;
    if ((dir = opendir (direction.c_str())) != nullptr) {
        while ((ent = readdir (dir)) != nullptr) {
            string name = ent->d_name;
            if(name.length() < 3)
                continue;
            if(isCplusplusFile(name)){
                TTest test;
                test.dir = direction + name;
                test.name = name;
                test.nameCompiled = changeBarToUnderscore(eraseExtension(test.dir));
                files.push_back(test);
            } else {
                readFolder(direction + name + "/");
            }
        }
        closedir (dir);
    }
    return files;
}
void getHAndCplusplusFiles(vector<TNecessaryFile> &necessaryFiles, const string &direction){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (direction.c_str())) != nullptr) {
        while ((ent = readdir (dir)) != nullptr) {
            string name = ent->d_name;
            if(name.length() < 3)
                continue;
            if(isCplusplusFile(name)){
                string noExtensionName = eraseExtension(name);
                bool exists = false;
                for(TNecessaryFile nf : necessaryFiles){
                    if(noExtensionName == nf.name){
                        nf.cppFile = name;
                        nf.cppFileDir.append(direction + "/");
                        nf.cppFileDir.append(name);
                        exists = true;
                        break;
                    }
                }
                if(!exists){
                    TNecessaryFile nf;
                    nf.name = noExtensionName;
                    nf.cppFile = name;
                    nf.cppFileDir.append(direction + "/");
                    nf.cppFileDir.append(name);
                    necessaryFiles.push_back(nf);
                }
            }
            else if (name.length() >= 3 && name.substr(name.length()-2,2) == ".h"){
                string noExtensionName = eraseExtension(name);
                bool exists = false;
                for(TNecessaryFile &nf : necessaryFiles){
                    if(noExtensionName == nf.name){
                        nf.hFile = name;
                        exists = true;
                        break;
                    }
                }
                if(!exists){
                    TNecessaryFile nf;
                    nf.name = noExtensionName;
                    nf.hFile = name;
                    necessaryFiles.push_back(nf);
                }
            }
            else getHAndCplusplusFiles(necessaryFiles , direction + "/" + name);
        }
        closedir (dir);
    }
}
void compilingHAndCplusplusFiles(vector<TNecessaryFile> &necessaryFiles, bool flag){
    DIR *dir;
    struct dirent *ent;
    for(const TNecessaryFile &nf : necessaryFiles){
        if(!nf.name.empty() && !nf.cppFile.empty() && !nf.hFile.empty()){
            string command = "g++ -Wall -g -c -Iinclude -o compilados/archivos-necesarios/" + nf.name + ".o " + nf.cppFileDir ;
            if(flag)
                command += " >& ./trash/" + nf.name + "_comp.out";
            system(command.c_str());
        }
    }
    string direction = "./compilados/archivos-necesarios/";
    vector<TNecessaryFile> newNecessaryFiles;
    for(const TNecessaryFile &nf : necessaryFiles){
        if ((dir = opendir (direction.c_str())) != nullptr) {
            while ((ent = readdir (dir)) != nullptr) {
                string name = ent->d_name;
                if(name.length() < 3)
                    continue;
                if(name == nf.name + ".o")
                    newNecessaryFiles.push_back(nf);
            }
            closedir (dir);
        }
    }
    necessaryFiles = newNecessaryFiles;




}
void showProcess(const unsigned &done, const unsigned &size, const string &name ){
    cout << name;
    for(unsigned i = 0; i < done; i++)
        cout << '|';
    for(unsigned i = done; i < size; i++)
        cout << '-';
    cout << ']' << endl;
}
void compilingTests(const vector<TTest> &tests, const vector<TNecessaryFile> &necessaryFiles, bool Hide, bool Valgrind){
    string command_obj_template;
    for(const TNecessaryFile &nf : necessaryFiles){
        command_obj_template += " ./compilados/archivos-necesarios/" + nf.name + ".o";
    }
    unsigned done = 0;
    for(const TTest &test : tests){
        if(Hide){
            showProcess(done, tests.size(), "\nProceso de compilacion [");
        }
        string command = "g++ -Wall -g -Iinclude -D DEBUG " + test.dir + command_obj_template + " -o compilados/" +
                         test.nameCompiled;
        if(Hide)
            command += " >& trash/" + test.nameCompiled + "_comp.out";
        system(command.c_str());
        done++;
        if(Hide) system("clear");
    }

}
void clearDirCompiladosAndTrash(){
    system("rm -r compilados");
    system("mkdir compilados");
    system("mkdir compilados/archivos-necesarios");
    system("mkdir compilados/salidas");
    system("rm -r trash");
    system("mkdir trash");
}
void executeTests(const vector<TTest> &tests){
    for(const TTest &test : tests){
        string command = "./compilados/" + test.nameCompiled + " >> compilados/salidas/" + test.nameCompiled + ".out";
        system(command.c_str());
    }
}
vector<tuple<string, string>> executeTestArgs(const vector<TTest> &tests, const bool &Hide){
    ifstream f("args.in");
    vector<tuple<string, string>> arguments;
    if(f.is_open()){
        string aux, auxx;
        while(getline(f, aux)){
            stringstream ss(aux);
            getline(ss, aux, '#');
            getline(ss, auxx);
            arguments.emplace_back(aux, auxx);
        }
        f.close();
    }
    if(tests.size() == 1)
        for(const tuple<string, string> &arg : arguments){
            string command = "./compilados/" + tests[0].nameCompiled + " " + get<1>(arg) + " >& compilados/salidas/" + get<0>(arg) + ".out";
            system(command.c_str());
        }
    if(Hide) system("clear");
    return arguments;
}
void comparisonOutputs(ifstream &waitedOutputFile, ifstream &obtainedOutputFile){
    string sObtained, sWaited;
    unsigned line = 1;
    while(true){
        getline(obtainedOutputFile, sObtained);
        getline(waitedOutputFile, sWaited);
        if(obtainedOutputFile.eof() || waitedOutputFile.eof())
            break;
        stringstream ss(sObtained);
        stringstream sse(sWaited);
        char c, cc;
        while(true){
             ss.get(c);
             sse.get(cc);
             if(cc == '\r' && ss.eof()) //caso para arreglar bug en PED porque los desgrrasiados usan \r al final
                 break;
             if(ss.eof() && sse.eof())
                 break;

            if(ss.eof() || sse.eof() || c != cc){
                cout << " DESIGUALDAD EN LA LINEA " << line << "." << endl;
                cout << "\tOBTENIDO -->|" << sObtained << endl;
                cout << "\tESPERADO -->|" << sWaited << endl;
                return;
            }
        }
        line++;
    }
    if(obtainedOutputFile.eof() && waitedOutputFile.eof())
        cout << " CORRECTO :)" << endl;
    else if(!obtainedOutputFile.eof()){
        string aux;
        getline(obtainedOutputFile, aux);
        if(aux.empty() && obtainedOutputFile.eof())
            cout << " DESIGUALDAD EN EL FICHERO OBTENIDO: EL FICHERO OBTENIDO TIENE UN SALTO DE LINEA MAS :(" << endl;
        else
            cout << " DESIGUALDAD EN EL FICHERO OBTENIDO: EL NUMERO DE LINEAS ES MAYOR AL ESPERADO :(" << endl;

    }
    else if(!waitedOutputFile.eof()){
        string aux;
        getline(waitedOutputFile, aux);
        if(aux.empty() && waitedOutputFile.eof())
            cout << " DESIGUALDAD EN EL FICHERO OBTENIDO: EL FICHERO OBTENIDO TIENE UN SALTO DE LINEA MENOS :(" << endl;
        else
            cout << " DESIGUALDAD EN EL FICHERO OBTENIDO: EL NUMERO DE LINEAS ES MENOR AL ESPERADO :(" << endl;
    }

}
void compareOutput(const string &waitedOutputWhithoutExtension, const string &obtainedOutput, string &extension, const TConfig &config){
    ifstream obtainedOutputFile(obtainedOutput);
    ifstream waitedOutputFile;
    waitedOutputFile.open(waitedOutputWhithoutExtension + extension);
    if(obtainedOutputFile.is_open()){
        if(waitedOutputFile.is_open()){
            comparisonOutputs(waitedOutputFile, obtainedOutputFile);
            waitedOutputFile.close();
        }
        else {
            bool found = false;
            for(const string &ext : config.outputExtensions){
                waitedOutputFile.open(waitedOutputWhithoutExtension + ext);
                if(waitedOutputFile.is_open()){
                    comparisonOutputs(waitedOutputFile, obtainedOutputFile);
                    waitedOutputFile.close();
                    extension = ext;
                    found = true;
                    break;
                }
            }
            if(!found){
                cout << "ERROR: no se ha encontrado el fichero esperado" << endl;
            }
        }
        obtainedOutputFile.close();
    } else {
        cout << "ERROR: no se ha encontrado el fichero obtenido del test" << endl;
    }
}
void compareOutputsTests(const vector<TTest> &tests, const TConfig &config){
    cout << "---------------------------------------------------------" << endl;
    cout << "\tCOMPARANDO LOS TESTS" << endl;
    cout << "---------------------------------------------------------" << endl;
    string actual = config.outputExtensions[0];
    for(const TTest &test : tests){
        string waitedOutput = "./salidas-esperadas/" ;
        if(config.outputFilenameWithExtension){
            string aux = test.dir;
            waitedOutput += aux.erase(0,8);
        }
        else {
            waitedOutput += eraseExtension(test.dir).erase(0,8);
        }
        cout << changeBarToArrow(eraseExtension(test.dir)) << ": ";
        compareOutput(waitedOutput, "compilados/salidas/" + test.nameCompiled + ".out", actual, config);
    }
    cout << endl;
}
void compareOutputsTestsArg(const vector<tuple<string, string>> &args, const TConfig &config){
    cout << "---------------------------------------------------------" << endl;
    cout << "\tCOMPARANDO LOS TESTS" << endl;
    cout << "---------------------------------------------------------" << endl;
    string actual = config.outputExtensions[0];
    for(const tuple<string, string> &arg : args){
        string waitedOutput = "./salidas-esperadas/" + get<0>(arg);
        cout << "> " << get<0>(arg) << ": ";
        compareOutput(waitedOutput, "compilados/salidas/" + get<0>(arg) + ".out", actual, config);
    }
    cout << endl;
}
void executeValgrind(const vector<TTest> &tests, const bool &Hide){
    unsigned done = 0;
    for(const TTest &test : tests){
        if(Hide) showProcess(done, tests.size(), "\nEjecutando Valgrind: [");
        string command = "valgrind --tool=memcheck --leak-check=full ./compilados/" + test.nameCompiled + " >& compilados/salidas/" + test.nameCompiled + "_valgrind.out";
        system(command.c_str());
        done++;
        if(Hide) system("clear");
    }
}
void showValgrindOutput(const vector<TTest> &tests){
    cout << "---------------------------------------------------------" << endl;
    cout << "\tVALGRIND EN LOS TESTS" << endl;
    cout << "---------------------------------------------------------" << endl;
    ifstream f;
    for(const TTest &test : tests){
        f.open("./compilados/salidas/" + test.nameCompiled + "_valgrind.out");
        cout << changeBarToArrow(eraseExtension(test.dir)) << ": " ;
        if(f.is_open()){
            string aux, errorString;
            stringstream ss;
            while(!f.eof()){
                getline(f, aux);
                if(!f.eof())
                    errorString = aux;
            }
            ss << errorString;
            getline(ss, aux, ':');
            ss.ignore();
            unsigned errors;
            ss >> errors;
            if(errors == 0)
                cout << "SIN ERRORES :)" << endl;
            else
                cout << "SE HAN DETECTADO " << errors << " ERRORES. POR FAVOR, REVISA LA SALIDA PARA MAS INFORMACION" << endl;
            f.close();
        } else {
            cout << "ERROR: no se ha encontrado el fichero de salida de valgrind" << endl;
        }
    }
    cout << endl;
}
vector<string> splitDir(const string &dirs){
    vector<string> directions;
    stringstream ss(dirs);
    string aux;
    while(getline(ss, aux, ':')){
        directions.push_back(aux);
    }
    //TODO revisar no repetidos
    return directions;
}
int main(int argc,char* args[]){
    //Check args
    TFlags flag;
    argsCheck(argc, args, flag);
    //Do help if flag
    if(flag.Help){
        help();
        return 0;
    }
    //Do version if flag
    if(flag.Version){
        version();
        return 0;
    }
    system("clear");
    //Leer .config
    TConfig config = readConfigFile();
    //Get a vector of tests
    vector<TTest> tests;
    if(flag.Folder){
        vector<string> directions = splitDir(flag.Fdir);
        for(const string &dir : directions){
            tests = readFolder("./tests/" + dir + "/");
        }
        if(tests.empty()){
            error(ERR_FOLDER_EMPTY, "");
        }
    }
    else if(flag.Test){
        vector<string> directions = splitDir(flag.Tdir);
        for(const string &dir : directions){
            TTest test;
            test.dir = "./tests/" + dir;
            test.name = getTestName(dir);
            test.nameCompiled = changeBarToUnderscore(eraseExtension(test.dir));
            tests.push_back(test);
        }
    }
    else {
        tests = readFolder("./tests/");
        if(tests.empty()){
            error(ERR_FOLDER_EMPTY, "");
        }
    }
    //ordenar los tests
    sort(tests.begin(), tests.end(), compareByName);
    //compilar .h//.cpp
    vector<TNecessaryFile> necessaryFiles;
    getHAndCplusplusFiles(necessaryFiles, "./lib");
    getHAndCplusplusFiles(necessaryFiles, "./include");
    //hacer funcion que dada una direccion borre toda y la cree
    clearDirCompiladosAndTrash();
    compilingHAndCplusplusFiles(necessaryFiles, flag.Hide);
    //compilar tests (comprobar si hay que mostrar o no)
    compilingTests(tests, necessaryFiles, flag.Hide, flag.Valgrind);
    //ejecuta todos, una carpeta o un test
    if(flag.Valgrind){
        executeValgrind(tests, flag.Hide);
        showValgrindOutput(tests);
    }
    else if(flag.Args){
        vector<tuple<string, string>> arguments = executeTestArgs(tests, flag.Hide);
        compareOutputsTestsArg(arguments,config);
    }
    else{
        executeTests(tests);
        compareOutputsTests(tests, config);
    }
}