# ************************************************************************* #
# Organizacion del Computador II                                            #
#                                                                           #
#   Informe Trabajo Practico 2                                              #
#   Necesito setear ./packages en la variable de entorno TEXINPUTS, así     #
#   me toma el paquete de la caratula, sino hay que instalar el paquete a   #
#   mano en el home en no se que directorio.                                #
#                                                                           #
#                                                                           #
#  Si se quiere correr a mano el pdflatex desde la consola primero hay que  #
#  que correr:                                                              #
#            export TEXINPUTS=".:./packages:"                               #
#                                                                           #
#                                                                           #
#  Para que genere la tabla de contenidos hay que compilar dos veces.       #
#                                                                           #
# ************************************************************************* #

all: informe

informe: 
	TEXINPUTS=".:./packages:" pdflatex informeTP2.tex
	TEXINPUTS=".:./packages:" pdflatex informeTP2.tex

clean:
	rm -f *.aux *.log *.out *.toc
	rm -f *.pdf
