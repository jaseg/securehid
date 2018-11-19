

SRCDIR := ../src
PYGMENTDIR := pygments
#SOURCES := $(shell ls $(SRCDIR)/*.cpp $(SRCDIR)/*.h $(SRCDIR)/*.pro|grep -v /moc_|grep -v Java|grep -v /qrc_)
PYGMENTIZE := pygmentize -f latex -l cpp -O verboptions='formatcom=\scriptsize',linenos=True
LINTER := clang-format -style="{BasedOnStyle: llvm, ColumnLimit: 48}"
TARGET := architecture

all: $(TARGET).pdf


.PHONY: stats
stats:
	@echo -e "\e[1mPages:\e[91m" $(shell cat $(TARGET).toc|grep References|egrep -o '[0-9]+') "\e[0m"
	@echo -e "\e[1mFile size:\e[92m" $(shell ls -sh $(TARGET).pdf|cut -d' ' -f1) "\e[0m"
	@echo -e "\e[1mAbstract:\e[92m" $(shell grep -A1000 '\\paragraph{English}' $(TARGET).tex|sed '/\\newpage/Q'|tail -n+2|grep -v '^%'|wc -w) "\e[0mwords"
	@echo -e "\e[1mFixmes:\e[93m" $(shell grep FIXME $(TARGET).tex|wc -l) "\e[0m"
	@grep -no '%FIXME.*' $(TARGET).tex|sed 's/%FIXME//;s/^/\x1b[93m/;s/: /\x1b[0m: /1'

$(TARGET).pdf: $(TARGET).tex bibliography.bib
	pdflatex $< 
	biber $(TARGET)
	pdflatex $< 

pygments/excerpt%.cpp: $(SOURCES)
	python extract_excerpts.py $(*F) $^ | $(LINTER) > $@

$(PYGMENTDIR)/%.cpp: $(SRCDIR)/%.cpp
	$(LINTER) $< > $@

$(PYGMENTDIR)/%.h: $(SRCDIR)/%.h
	$(LINTER) $< > $@

$(PYGMENTDIR)/%.pro: $(SRCDIR)/%.pro
	cp $< $@

$(PYGMENTDIR)/%.tex: $(PYGMENTDIR)/%
	$(PYGMENTIZE) -o $@ $<

sources.zip: $(SOURCES)
	zip -j $@ $^

.PHONY: sources
sources: $(PYGMENTDIR)/sourcelist.tex $(PYGMENTDIR)/pygmentdefs.tex sources.zip


$(PYGMENTDIR)/sourcelist.tex: $(addprefix $(PYGMENTDIR)/,$(notdir $(patsubst %.cpp,%.cpp.tex,$(patsubst %.h,%.h.tex,$(patsubst %.pro,%.pro.tex,$(SOURCES))))))
	@echo $^ | tr ' ' '\n'> sources_output.txt
	@echo $(notdir $(patsubst %.tex,%,$^)) | tr ' ' '\n'> sources_input.txt
	paste -d! sources_input.txt sources_output.txt | sed 's/^\(.*\)!\(.*\)$$/\\includesource{\1}{\2}/g' > $(PYGMENTDIR)/sourcelist.tex

$(PYGMENTDIR)/pygmentdefs.tex:
	pygmentize -S default -f latex > $(PYGMENTDIR)/pygmentdefs.tex

.PHONY: clean
clean:
	rm -f *.aux *.log *.out *.toc *.bbl *.blg
	rm -f pygments/*

