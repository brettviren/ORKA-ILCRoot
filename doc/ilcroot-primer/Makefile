FILE		= primer
EPSS		= picts/Parab.eps			\
		  picts/ReconstructionFramework.eps	\
		  picts/SimulationFramework.eps	        \
		  picts/alienfig7.eps			\
		  picts/ilcgen.eps			\
		  picts/cocktail.eps			\
		  picts/evglib.eps			\
		  picts/folderstruct.eps		\
		  picts/kaon.eps			\
		  picts/loaderdiagram.eps		\
		  picts/partcuts.eps			\
		  picts/phipp.eps			\
		  picts/vmc.eps			        \
		  picts/tagana.eps		        \
		  picts/tagsources.eps	                \
		  picts/tagpost.eps
PDFS		= $(EPSS:%.eps=%.pdf)
LATEX		= latex
PDFLATEX	= pdflatex
DVIPS		= dvips 
PSTOPDF		= epstopdf 

%.aux:	%.tex
	$(LATEX) $< 

%.dvi:	%.aux
	$(LATEX) $* 

%.ps:	%.dvi 
	$(DVIPS) $< -o $@ 

%.pdf:	%.tex
	$(PDFLATEX) $*
	$(PDFLATEX) $*

%.pdf:	%.eps
	$(PSTOPDF) $< 

default: $(FILE).pdf

clean:
	rm -f 	$(FILE).dvi 	\
		$(FILE).aux 	\
		$(FILE).toc 	\
		$(FILE).log 	\
		$(FILE).ps 	\
		$(FILE).out	\
		$(FILE).lol	\
		$(FILE).pdf $(PDFS)


dist:
	mkdir -p ilcroot-primer 
	cp -a $(FILE).tex ilcroot-primer/
	cp -a $(EPSS)     ilcroot-primer/
	cp -a Makefile    ilcroot-primer/
	cp -a scripts     ilcroot-primer/
	tar -cvf - ilcroot-primer | gzip - > ilcroot-primer.tar.gz 
	rm -rf ilcroot-primer 

$(FILE).dvi:$(FILE).aux	$(EPSS)
$(FILE).pdf:$(FILE).tex	$(PDFS)
