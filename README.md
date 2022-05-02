# DOKIMASE
Dokimase, o también llamado Doki, es un tester creado con la intención de poder comprobar el correcto funcionamiento de programas escritos en C++ cuyos tests vienen dados en ficheros con main y salidas esperadas individuales, como frecuentemente sucede en algunas instituciones educativas. 
Doki ha sido desarrollado con la intencion de poder usarse en el mayor número de situaciones distintas que puedan darse: desde programas de un solo archivo a programas con múltiples archivos; programas con o sin argumentos; programas con o sin clases, etc, y se espera contemplar más situaciones en futuras actualizaciones.

## ESTRUCTURA DE FICHEROS
Doki hace uso de distintas carpetas identificativas en las cuales se espera que residan los diferentes archivos necesarios para el correcto uso del programa a probar. Las carpetas y sus usos son los siguientes:
> NOTA: Nada mas obtener el programa, este no incluirá las carpetas necesarias, por eso habrá que ejecutar el comando ***make*** (desde la carpeta del tester) el cual generara las carpetas y el ejecutable del programa.


  | Nombre de la carpeta | Uso |
  | --- | --- |
  | bin | Esta carpeta no debe modificarse puesto a que es la carpeta contenedora del codigo fuente de Doki |
  | compilados | En esta carpeta se guardan los tests compilados. El nombre de los tests compilados es d_x, donde d es la direccion desde .tests/ (separados con \_) y x el nombre del test. P.ej: el test _./tests/probando1/probando2/test34.cpp_ se llamará _probando1\_probando2\_test34_ | 
  | compilados/archivos-necesarios | En esta carpeta se guardan los archivos necesarios compilados. A diferencia de los tests, los archivos necesarios se llaman como x.o donde x es el nombre del archivo |
  | compilados/salidas | En esta carpeta se guardan las salidas de los tests una vez ejecutados. Las salidas se llaman igual que los compilados añadiendo al final la extension _\_valgrind.out_ o _.out_ según si esta activada la flag -val o no respectivamente |
  | ficheros-entrada | Esta carpeta es una carpeta de cortesía que se ofrece para albergar aquellos ficheros que sean requeridos por los tests, aunque puede ser ignorada y usarse la que desee el usuario |
  | include | En esta carpeta el usuario tendra que introducir los archivos .h de aquellas clases necesarias para los tests (los creados por el usuario). |
  | lib | En esta carpeta el usuario tendrá que introducir los archivos .cpp/.cc de aquellas clases necesarias para los tests (los creados por el usuario). El propio tester ignora las clases que no sean necesarias. Si alguna clase presenta errores de compilación, se recomienda usar los argumentos -f o -t para ejecutar los tests que no necesiten de esa clase, ya que estas no deberían verse afectadas por la clase en cuestión|
  | salidas-esperadas | En esta carpeta el usuario tendra que introducir los ficheros de salida esperados (aquellos que contienen la salida por pantalla que debe mostrar el test). Debe haber un fichero por test (si hay de más, se ignoraran los extra; si faltan, se mostrará un error). Las salidas deberán tener el mismo nombre que los tests, es decir, tendrán que presentar la misma estructura de directorios que los tests; p.ej: un test _./tests/direc/test.cpp_ tendrá su salida en _./salidas-esperadas/direc/test.sal_( en el .config del programa se dan más opciones sobre estos ficheros) |
  | tests | En esta carpeta el usuario tendrá que introducir los tests con extensión .cpp/.cc/.c | 
  | trash | En esta carpeta se guardaran archivos propios de la ejecución del tester que no tienen relevancia, como las salidas de compilacion al usar -h |

  > NOTA: Si las salidas esperadas y los tests vienen dados en la misma carpeta, no hace falta separarlos, se puede introducir el mismo contenido en _salidas-esperadas_ y _tests_ ya que Doki solo ejecuta los tests con extensión .cpp/.cc/.c y para las salidas mirará las especificadas en el .config
  
  > NOTA: Las carpetas **compilados** y **trash** seran limpiadas cada ejecución, por lo que no debe introducirse ningún archivo que no deba ser borrado.

## FICHEROS DE CONFIGURACION
Doki se sirve de 2 ficheros de configuración:
- ***args.in***: Este es el fichero que emplea Doki para poder usar el argumento _--args_ de forma correcta. Este fichero contendra diversas lineas, las cuales, cada una, indicará el nombre del fichero de salida para el test con esos argumentos y los propios argumentos que se añadirán a la ejecución. Esta información debera seguir la rigurosa estructura de _nombreDelFicheroDeSalidaSinExtensión#argumentos_. P.ej: dado un fichero de salida _salida1.sal_, para la ejecución de _test.cpp_ de la forma _./test -as --ignore-a_, se incluirá la linea _salida1#-as --ignore-a_.
- ***dokimase.config***: Este es el fichero de configuración de Doki. Desde este fichero, de momento, se puede modificar: 
  - _outputExtension_: A continuación de esta opción se introducirán todas las extensiones, separadas por espacios, que puedan tener los ficheros de salida, de forma que el programa pueda identificar todos los ficheros de salida esperados de forma correta. Por defecto, se dan las siguientes extensiones: sal sol out.
  - _outputFilenameWithExtension_: A continuación de esta opción se introducirá _true_ o _false_ si los ficheros de salida esperada llevan la extensión del test además de la de fichero de salida. P.ej: dado un test _test2.cpp_ y un fichero de salida _test2.cpp.sal_, si esta opción está establecida a _true_, se ignorará el _.cpp_ en el nombre del fichero de salida. Por defecto, esta opción está establecida a _true_. Doki ignorará esta opción en los ficheros en los que no encuentre extensión, evitando que se tenga que modificar si algunos ficheros llevan o no extensión. P.ej: si el fichero de salida se llama _salida1.sal_ y está activada la opción, al no presentar la extensión, para esta salida, Doki funcionará como si no estuviera activada la opción.


## ARGUMENTOS
