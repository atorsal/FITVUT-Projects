filename=proj2

${filename}.pdf: ${filename}.tex
	latex ${filename}.tex
	latex ${filename}.tex
	dvips -t a4 ${filename}.dvi
	ps2pdf ${filename}.ps