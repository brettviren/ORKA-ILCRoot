
include Makefile.arch

PACKAGE = ANALYSISilc

default-target: lib$(PACKAGE).so

ILCINC      = -I.

# add include paths from other par files
ifneq ($(ESD_INCLUDE),)
   ILCINC += -I../$(ESD_INCLUDE)
endif

ifneq ($(AOD_INCLUDE),)
   ILCINC += -I../$(AOD_INCLUDE)
endif

ifneq ($(STEERBase_INCLUDE),)
   ILCINC += -I../$(STEERBase_INCLUDE)
endif

ifneq ($(ANALYSIS_INCLUDE),)
   ILCINC += -I../$(ANALYSIS_INCLUDE)
endif

ifneq ($(OADB_INCLUDE),)
   ILCINC += -I../$(OADB_INCLUDE)
endif

ifneq ($(ANALYSISilc_INCLUDE),)
   ILCINC += -I../$(ANALYSISilc_INCLUDE)
endif

# only if no par file was loaded before
#ifeq ($(ILCINC),-I.)
  ifneq ($(ILC_ROOT),)
    ILCINC += -I$(ILC_ROOT)/include -I$(ILC_ROOT)/ANALYSIS/Tender -I$(ILC_ROOT)/TOF -I$(ILC_ROOT)/TRD/Cal -I$(ILC_ROOT)/VZERO -I$(ILC_ROOT)/TPC -I$(ILC_ROOT)/T0 -I$(ILC_ROOT)/EMCAL
  endif
#endif

ifeq ($(PLATFORM),macosx)
ROOTLIBDIR     := $(shell $(RC) --libdir)
ifeq ($(PACKAGE),ANALYSIS)
   PACKLDFLAGS = $(STEERBase_LIBS) $(ESD_LIBS) $(AOD_LIBS) -L$(ROOTLIBDIR)
endif
ifeq ($(PACKAGE),ANALYSISilc) 
   PACKLDFLAGS = $(STEERBase_LIBS) $(ESD_LIBS) $(AOD_LIBS) $(ANALYSIS_LIBS) $(OADB_LIBS) -L$(ROOTLIBDIR) -lXMLIO -lEG -lTreePlayer
endif
endif

CHECKXML      := $(shell root-config --has-xml)

ifeq ($(CHECKXML),yes)
  CXXFLAGS += -DWITHXML
  CINTFLAGS += -DWITHXML
endif

CXXFLAGS += $(ILCINC) -g

SRCS     = $(wildcard *.cxx)
HDRS     = $(SRCS:.cxx=.h)
DHDR     = $(PACKAGE)LinkDef.h

DHDR_ANALYSIS := $(DHDR)
HDRS_ANALYSIS := $(HDRS)
SRCS_ANALYSIS := $(SRCS) G__$(PACKAGE).cxx
OBJS_ANALYSIS := $(SRCS_ANALYSIS:.cxx=.o)

PARFILE       = $(PACKAGE).par


lib$(PACKAGE).so: $(OBJS_ANALYSIS)
	@echo "Linking" $@ ...
	@/bin/rm -f $@
ifeq ($(PLATFORM),macosx)
	$(LD) $(SOFLAGS)$@ $(LDFLAGS) $^ $(OutPutOpt) $@ $(PACKLDFLAGS) $(EXPLLINKLIBS)
else
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@ $(PACKLDFLAGS) $(EXPLLINKLIBS)
endif
	@chmod a+x $@
	@echo "done"

%.o:    %.cxx %.h
	$(CXX) $(CXXFLAGS) $(PACKCXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS_ANALYSIS) *.so G__$(PACKAGE).* $(PARFILE)

G__$(PACKAGE).cxx G__$(PACKAGE).h: $(HDRS) $(DHDR)
	@echo "Generating dictionary ..."
	rootcint -f $@ -c $(CINTFLAGS) $(ILCINC) $^

### CREATE PAR FILE

$(PARFILE): $(patsubst %,$(PACKAGE)/%,$(filter-out G__%, $(HDRS_ANALYSIS) $(SRCS_ANALYSIS) $(DHDR_ANALYSIS) Makefile Makefile.arch PROOF-INF))
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
