proj = proj3
all:
	latex $(proj).tex
	latex $(proj).tex
	dvips -t a4 $(proj).dvi
	ps2pdf -sPAPERSIZE=a4 $(proj).ps

clean:
	rm -f $(proj).aux $(proj).ps $(proj).dvi $(proj).log

