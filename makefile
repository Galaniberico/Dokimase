compilar: bin/Dokimase.cpp include compilados ficheros-entrada lib salidas-esperadas tests trash
	g++ -o doki ./bin/Dokimase.cpp
include: 
	mkdir include
compilados: 
	mkdir compilados
	mkdir compilados/archivos-necesarios
	mkdir compilados/salidas
ficheros-entrada:
	mkdir ficheros-entrada
lib:
	mkdir lib
salidas-esperadas:
	mkdir salidas-esperadas
tests:
	mkdir tests
trash: 
	mkdir trash
