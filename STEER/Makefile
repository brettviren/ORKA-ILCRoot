PACKAGE = invalid-only-for-proof

include Makefile.arch

SRCS     = $(wildcard *.cxx)
HDRS     = $(SRCS:.cxx=.h)
DHDR     = $(PACKAGE)LinkDef.h


ifndef PACKCXXFLAGS
   PACKCXXFLAGS = $(CXXFLAGS)
endif

ILCINC = -I.
ILCINC += -I./$(PACKAGE)/

ifneq ($(ESD_INCLUDE),)
   ILCINC += -I../$(ESD_INCLUDE) 
endif

ifneq ($(AOD_INCLUDE),)
   ILCINC += -I../$(AOD_INCLUDE) 
endif

ifneq ($(STEERBase_INCLUDE),)
   ILCINC += -I../$(STEERBase_INCLUDE)
endif

# only if no par file was loaded before
#ifeq ($(ILCINC),-I.)
  ifneq ($(ILC_ROOT),)
    ILCINC += -I$(ILC_ROOT)/include
  endif
#endif

ifeq ($(PLATFORM),macosx)
ROOTLIBDIR     := $(shell $(RC) --libdir)
ifeq ($(PACKAGE),STEERBase)
   PACKLDFLAGS = -L$(ROOTLIBDIR)  -lVMC -lMinuit -lEG -lGeom
endif
ifeq ($(PACKAGE),ESD)
   PACKLDFLAGS = $(STEERBase_LIBS) -L$(ROOTLIBDIR) -lVMC -lEG -lGeom
endif
ifeq ($(PACKAGE),AOD)
   PACKLDFLAGS = $(STEERBase_LIBS) -L$(ROOTLIBDIR) -lEG -lGeom
endif
ifeq ($(PACKAGE),CDB)
   PACKLDFLAGS = $(STEERBase_LIBS) -L$(ROOTLIBDIR) -lMinuit -lTree -lXMLParser
endif
endif


CXXFLAGS += $(ILCINC) -g

SRCS         += G__$(PACKAGE).cxx
OBJS          = $(SRCS:.cxx=.o)

lib$(PACKAGE).so: $(OBJS)
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
	$(CXX) $(PACKCXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) *.so G__$(PACKAGE).*

G__$(PACKAGE).cxx G__$(PACKAGE).h: $(HDRS) $(DHDR)
	@echo "Generating dictionaries ..." $(ILCINC)
	rootcint -f $@ -c $(CINTFLAGS) $(ILCINC) $^

