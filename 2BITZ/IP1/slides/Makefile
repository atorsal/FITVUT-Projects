NAME=slides

all:
	latex $(NAME).tex
	latex $(NAME).tex
	dvips $(NAME).dvi
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps

clean:
	rm -f $(NAME).aux $(NAME).log $(NAME).dvi $(NAME).ps $(NAME).nav $(NAME).snm $(NAME).toc $(NAME).out $(NAME).pdf
