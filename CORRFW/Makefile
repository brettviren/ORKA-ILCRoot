
include Makefile.arch

default-target: libCORRFW.so

ILCINC      = -I.

# add include paths from other par files
ifneq ($(STEERBase_INCLUDE),)
   ILCINC += -I../$(STEERBase_INCLUDE)
endif

ifneq ($(ESD_INCLUDE),)
   ILCINC += -I../$(ESD_INCLUDE)
endif

ifneq ($(AOD_INCLUDE),)
   ILCINC += -I../$(AOD_INCLUDE)
endif

ifneq ($(ANALYSIS_INCLUDE),)
   ILCINC += -I../$(ANALYSIS_INCLUDE)
endif

ifneq ($(ANALYSISilc_INCLUDE),)
ILCINC += -I../$(ANALYSISilc_INCLUDE)
endif

# only if no par file was loaded before
#ifeq ($(ILCINC),-I.)
  ifneq ($(ILC_ROOT),)
    ILCINC += -I$(ILC_ROOT)/include
  endif
#endif

CXXFLAGS     += $(ILCINC) -g

PACKAGE = CORRFW

SRCS     = $(wildcard *.cxx)
HDRS     = $(SRCS:.cxx=.h)
DHDR     = $(PACKAGE)LinkDef.h

DHDR_CORRFW := $(DHDR)
HDRS_CORRFW := $(HDRS)
SRCS_CORRFW := $(SRCS) G__$(PACKAGE).cxx
OBJS_CORRFW := $(SRCS_CORRFW:.cxx=.o)

PARFILE       = $(PACKAGE).par


lib$(PACKAGE).so: $(OBJS_CORRFW)
	@echo "Linking" $@ ...
	@/bin/rm -f $@
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS)
	@chmod a+x $@
	@echo "done"

%.o:    %.cxx %.h
	$(CXX) $(CXXFLAGS) $(PACKCXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS_CORRFW) *.so G__$(PACKAGE).* $(PARFILE)

G__$(PACKAGE).cxx G__$(PACKAGE).h: $(HDRS) $(DHDR)
	@echo "Generating dictionary ..."
	rootcint -f $@ -c $(CINTFLAGS) $(ILCINC) $^

### CREATE PAR FILE

$(PARFILE): $(patsubst %,$(PACKAGE)/%,$(filter-out G__%, $(HDRS_CORRFW) $(SRCS_CORRFW) $(DHDR_CORRFW) Makefile Makefile.arch PROOF-INF))
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
