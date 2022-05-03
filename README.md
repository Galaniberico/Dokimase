# DOKIMASE
Dokimase, o también llamado Doki, es un tester creado con la intención de poder comprobar el correcto funcionamiento de programas escritos en C++ cuyos tests vienen dados en ficheros con main y salidas esperadas individuales, como frecuentemente sucede en algunas instituciones educativas. 
Doki ha sido desarrollado con la intención de poder usarse en el mayor número de situaciones distintas que puedan darse: desde programas de un solo archivo a programas con múltiples archivos; programas con o sin argumentos; programas con o sin clases... y se espera contemplar más situaciones en futuras actualizaciones.

## ESTRUCTURA DE FICHEROS
Doki hace uso de distintas carpetas identificativas en las cuales se espera que residan los diferentes archivos necesarios para el correcto uso del programa a probar. Las carpetas y sus usos son los siguientes:
> NOTA: Nada más obtener el programa, este no incluirá las carpetas necesarias, por eso habrá que ejecutar el comando `make` (desde la carpeta del tester) el cual generará las carpetas y el ejecutable del programa.


  | Nombre de la carpeta | Uso |
  | --- | --- |
  | bin | Esta carpeta no debe modificarse puesto a que es la carpeta contenedora del código fuente de Doki |
  | compilados | En esta carpeta se guardan los tests compilados. El nombre de los tests compilados sigue la sintaxis _d_x_, donde _d_ es la dirección desde _.tests/_ (separados con \_) y x el nombre del test. P.ej: el test _./tests/probando1/probando2/test34.cpp_ se llamará _probando1\_probando2\_test34_ | 
  | compilados/archivos-necesarios | En esta carpeta se guardan los archivos necesarios compilados. A diferencia de los tests, los archivos necesarios se llaman siguiendo la sintaxis _x_.o donde _x_ es el nombre del archivo |
  | compilados/salidas | En esta carpeta se guardan las salidas de los tests una vez ejecutados. Las salidas se llaman igual que los compilados añadiendo al final la extensión _\_valgrind.out_ o _.out_ según si esta activada la flag -val o no respectivamente |
  | ficheros-entrada | Esta carpeta es una carpeta de cortesía que se ofrece para albergar aquellos ficheros que sean requeridos por los tests, aunque puede ser ignorada y usarse la que desee el usuario |
  | include | En esta carpeta el usuario tendrá que introducir los archivos .h de aquellas clases necesarias para los tests (los creados por el usuario) |
  | lib | En esta carpeta el usuario tendrá que introducir los archivos .cpp/.cc de aquellas clases necesarias para los tests (los creados por el usuario). El propio tester ignora las clases que no sean necesarias. Si alguna clase presenta errores de compilación, se recomienda usar los argumentos -f o -t para ejecutar los tests que no necesiten de esa clase, ya que estas no deberían verse afectadas por la clase en cuestión|
  | salidas-esperadas | En esta carpeta el usuario tendrá que introducir los ficheros de salida esperados (aquellos que contienen la salida por pantalla que debe mostrar el test). Debe haber un fichero por test (si hay de más, se ignorarán los extras; si faltan, se mostrará un error). Las salidas deberán tener el mismo nombre que los tests, es decir, tendrán que presentar la misma estructura de directorios que los tests; p.ej: un test _./tests/direc/test.cpp_ tendrá su salida en _./salidas-esperadas/direc/test.sal_ ( en el .config del programa se dan más opciones sobre estos ficheros) |
  | tests | En esta carpeta el usuario tendrá que introducir los tests con extensión .cpp/.cc/.c | 
  | trash | En esta carpeta se guardarán archivos propios de la ejecución del tester que no tienen relevancia, como las salidas de compilación al usar -h |

  > NOTA: Si las salidas esperadas y los tests vienen dados en la misma carpeta, no hace falta separarlos, se puede introducir el mismo contenido en _salidas-esperadas_ y _tests_ ya que Doki solo ejecuta los tests con extensión .cpp/.cc/.c y para las salidas mirará las especificadas en el .config
  
  > NOTA: Las carpetas _compilados_ y _trash_ serán limpiadas cada ejecución, por lo que no debe introducirse ningún archivo que no deba ser borrado.

## FICHEROS DE CONFIGURACION
Doki se sirve de 2 ficheros de configuración:
- ***args.in***: Este es el fichero que emplea Doki para poder usar el argumento _--args_ de forma correcta. Este fichero contendrá diversas líneas, las cuales, cada una, indicará el nombre del fichero de salida para el test con esos argumentos y los propios argumentos que se añadirán a la ejecución. Esta información deberá seguir la rigurosa estructura de _nombreDelFicheroDeSalidaSinExtensión#argumentos_. P.ej: dado un fichero de salida _salida1.sal_, para la ejecución de _test.cpp_ de la forma _./test -as --ignore-a_, se incluirá la línea _salida1#-as --ignore-a_.
- ***dokimase.config***: Este es el fichero de configuración de Doki. Desde este fichero, de momento, se puede modificar: 
  - _outputExtension_: A continuación de esta opción se introducirán todas las extensiones, separadas por espacios, que puedan tener los ficheros de salida, de forma que el programa pueda identificar todos los ficheros de salida esperados de forma correcta. Por defecto, se dan las siguientes extensiones: sal sol out.
  - _outputFilenameWithExtension_: A continuación de esta opción se introducirá _true_ o _false_ si los ficheros de salida esperada llevan la extensión del test además de la de fichero de salida. P.ej: dado un test _test2.cpp_ y un fichero de salida _test2.cpp.sal_, si esta opción está establecida a _true_, se ignorará el _.cpp_ en el nombre del fichero de salida. Por defecto, esta opción está establecida a _true_. Doki ignorará esta opción en los ficheros en los que no encuentre extensión, evitando que se tenga que modificar si algunos ficheros llevan o no extensión. P.ej: si el fichero de salida se llama _salida1.sal_ y está activada la opción, al no presentar la extensión, para esta salida, Doki funcionará como si no estuviera activada la opción.


## ARGUMENTOS
Doki presenta la posibilidad de ser usado junto a varios argumentos para personalizar la ejecución de este. Los argumentos que se pueden usar en Doki son los siguientes:

| Argumento | Uso |
| --- | --- |
| -a<br>--args | Con este argumento, Doki ejecuta el test seleccionado pasándole como argumentos aquellos preestablecidos en el fichero _args.in_. De esta forma, lo que se comparará con las salidas esperadas son las salidas del mismo test ejecutado repetidas veces con distintos argumentos (o iguales en el caso de que asi se desee)|
| -f <br>--folder | Con este argumento, Doki ejecuta solo los tests específicos de la o las carpetas especificadas, lo cual puede resultar practico cuando solo se quieran probar ciertas carpetas de tests. La sintaxis aceptada para _<carpeta>_ es el nombre de una carpeta o una lista de carpetas separadas por `:` empezando desde dentro de la carpeta _tests_ hasta llegar a la carpeta en cuestión. P.ej: Dadas las carpetas _./tests/tests3_ y _./tests/testsZ/tests46_, si se quieren pasar los tests de la carpeta _tests3_, el argumento será `-f tests3`; si por el contrario se quieren realizar los tests de las 2 carpetas, el argumento será `-f tests3:tests46` |
| -h<br>--hide | Con este argumento, Doki oculta los mensajes de compilación de forma que el proceso de testeo sea más agradable a la vista, ya que evita mostrar mensajes que pueden molestar en la lectura del test |
| --help | Muestra el uso de cada argumento desde el propio programa |
| -t<br>--test | Con este argumento, Doki ejecuta solo el o los tests especificados, lo cual puede resultar practico cuando se trata de pasar muchas veces un solo test o varios. Al igual que con _<carpeta>_, _<tests>_ es el nombre o lista de nombres separados por `:` empezando desde dentro de la carpeta de tests. P.ej: Dados los tests _./tests/testsuelto.cpp_ y _./tests/testsZ/tests46/test0002.cpp_, si se quiere pasar el test _testsuelto.cpp_, el argumento será `-t testsuelto.cpp`; si por el contrario se quieren realizar los 2 tests, el argumento será `-t testsuelto.cpp:testsZ/tests46/test0002.cpp` |
| -val<br>--valgrind | Con este argumento, Doki en vez de comparar las salidas esperadas con las salidas de los tests, ejecutará _valgrind_ con cada test en busca de errores o memoria no borrada. Doki mostrará si hay o no errores, por lo que en el caso de que haya errores, el usuario debe ir a _compilados/salidas/_ y en el fichero correspondiente verá la salida de valgrind |
| -v<br>--versión | Muestra la versión de Doki | 

>NOTA: El argumento ***--args*** requiere del argumento ***--test*** y no se asegura que el funcionamiento de ***--args*** sea el esperado con más de un test, por lo que no se recomienda el uso de múltiples tests con este argumento. Además, no es compatible con el argumento ***--valgrind***
  
>NOTA: No se permite el uso de ***--folder*** y ***--tests*** a la vez.
  
>NOTA: El uso repetido de argumentos no está permitido para evitar problemas de ambigüedad.

>NOTA: El argumento ***--help*** y ***--versión*** no pueden usarse junto a otros argumentos.

## CONTACTO
  Si se detecta algún bug, error, mal funcionamiento... o si simplemente se quiere sugerir un cambio o añadir algún nuevo contenido, se puede enviar un correo a ***alexgalanp@gmail.com*** informando del problema o del cambio que se quiera realizar. También se pueden crear pull requests que sean constructivos, es decir, que aporten mejoría de funcionamiento, eficacia, etc, por lo que se rechazara inmediatamente todo aquel que no cumpla estos requisitos o los que se impongan durante el pull request.

Gracias por leer el readme y espero que te guste Doki y lo compartas con tus amigos ;)
