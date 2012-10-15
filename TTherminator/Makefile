# $Id: Makefile 54863 2012-02-28 12:10:29Z hristov $

include Makefile.arch

default-target: libTTherminator.so

ILCINC      = -I. -I$(ILC_ROOT)/include -ITherminator

### define include dir for local case and par case
ifneq ($(ILC_ROOT),)
  ILCINC += -I$(ILC_ROOT)/include -I$(ILC_ROOT)/PYTHIA6 -I$(ILC_ROOT)/EVGEN -I$(ILC_ROOT)/RAW -I$(ILC_ROOT)/TPC
else
  ifneq ($(STEERBase_INCLUDE),)
    ILCINC += -I../$(STEERBase_INCLUDE)
  endif
  ifneq ($(ESD_INCLUDE),)
    ILCINC += -I../$(ESD_INCLUDE)
  endif
endif

PACKAGE = TTherminator

DHDR =
HDRS =
SRCS =
FSRCS =
DHDR_TTherminator := $(DHDR)
HDRS_TTherminator := $(HDRS)
SRCS_TTherminator := $(SRCS) G__$(PACKAGE).cxx
OBJS_TTherminator := $(SRCS_TTherminator:.cxx=.o)

PARFILE       = $(PACKAGE).par

lib$(PACKAGE).so: $(OBJS_TTherminator)
	@echo "Linking" $@ ...
	@/bin/rm -f $@
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS)
	@chmod a+x $@
	@echo "done"

%.o:    %.cxx %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(ILCINC)

G__TTherminator.cxx G__TTherminator.h: $(HDRS_TTherminator) $(DHDR_TTherminator)
	@echo "Generating dictionary ..."
	rootcint -f $@ -c $(ILCINC) $^

clean:
	@rm -f $(OBJS_TTherminator) *.so G__TTherminator.* $(PARFILE)

### CREATE PAR FILE

$(PARFILE): $(patsubst %,$(PACKAGE)/%,$(filter-out G__%, $(HDRS_TTherminator) $(SRCS_TTherminator) $(DHDR_TTherminator) Makefile Makefile.arch lib$(PACKAGE).pkg PROOF-INF))
	@echo "Creating archive" $@ ...
	@tar cfzh $@ $(PACKAGE)
	@rm -rf $(PACKAGE)
	@echo "done"

$(PACKAGE)/Makefile: Makefile #.$(PACKAGE)
	@echo Copying $< to $@ with transformations
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@sed 's/include \$$(ROOTSYS)\/test\/Makefile.arch/include Makefile.arch/' < $^ > $@

$(PACKAGE)/Makefile.arch: $(ROOTSYS)/test/Makefile.arch
	@echo Copying $< to $@
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@cp -a $^ $@

$(PACKAGE)/PROOF-INF: PROOF-INF.$(PACKAGE)
	@echo Copying $< to $@
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@cp -a -r $^ $@

$(PACKAGE)/%: %
	@echo Copying $< to $@
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@cp -a $< $@

test-%.par: %.par
	@echo "INFO: The file $< is now tested, in case of an error check in par-tmp."
	@mkdir -p par-tmp
	@cd par-tmp; tar xfz ../$<;	cd $(subst .par,,$<); PROOF-INF/BUILD.sh
	@rm -rf par-tmp
	@echo "INFO: Testing succeeded (already cleaned up)"

