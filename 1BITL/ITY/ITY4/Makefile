PROJ4=proj4

all:
	latex $(PROJ4).tex
	bibtex $(PROJ4).aux
	latex $(PROJ4).tex
	latex $(PROJ4).tex
	dvips -t a4 $(PROJ4).dvi
	ps2pdf $(PROJ4).ps

clean:
	rm -f $(PROJ4).aux $(PROJ4).dvi $(PROJ4).ps $(PROJ4).bbl $(PROJ4).blg $(PROJ4).log
