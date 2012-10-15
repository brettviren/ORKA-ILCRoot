PACKAGE = invalid-only-for-proof

include Makefile.arch

SRCS     = $(wildcard *.cxx)
HDRS     = $(SRCS:.cxx=.h)
DHDR     = $(PACKAGE)LinkDef.h


ILCINC      = -I.
ifneq ($(findstring dev,$(PACKAGE)),)
 ILCINC     += -I./DEV
endif

### define include dir for local case and par case
ifneq ($(OADB_INCLUDE),)
   ILCINC += -I../$(OADB_INCLUDE)
endif
ifneq ($(EMCALUtils_INCLUDE),)
   ILCINC += -I../$(EMCALUtils_INCLUDE)
endif
ifneq ($(JETANDEV_INCLUDE),)
  ifneq ($(FASTJET_ROOT),)
   ILCINC += -I$(FASTJET_ROOT)/include
  else
   ILCINC += -I$(ILC_ROOT)/JETAN/fastjet
  endif
endif
ifneq ($(JETAN_INCLUDE),)
 ifeq ($(findstring dev,$(PACKAGE)),)
  ifneq ($(FASTJET_ROOT),)
   ILCINC += -I$(FASTJET_ROOT)/include
  else
   ILCINC += -I$(ILC_ROOT)/JETAN/fastjet
  endif
 endif
endif

ifneq ($(ESD_INCLUDE),)
   ILCINC += -I../$(ESD_INCLUDE)
   ILCINC += -I../$(AOD_INCLUDE)
   ILCINC += -I../$(STEERBase_INCLUDE)
   ILCINC += -I../$(ANALYSIS_INCLUDE)
   ILCINC += -I../$(ANALYSISilc_INCLUDE)
   ifneq ($(JETANDEV_INCLUDE),)	
     ILCINC += -I../$(JETANDEV_INCLUDE)
   endif
   ifneq ($(JETAN_INCLUDE),)
    ifeq ($(findstring dev,$(PACKAGE)),)
     ILCINC += -I../$(JETAN_INCLUDE)
    endif
   endif
 else
    ifneq ($(ILC_ROOT),)
      ifneq ($(findstring dev,$(PACKAGE)),)
       ILCINC += -I$(ILC_ROOT)/include -I$(ILC_ROOT)/JETAN/DEV -I$(ILC_ROOT)/JETAN/fastjet -I$(ILC_ROOT)/ANALYSIS
      else
       ILCINC += -I$(ILC_ROOT)/include -I$(ILC_ROOT)/JETAN -I$(ILC_ROOT)/JETAN/fastjet -I$(ILC_ROOT)/ANALYSIS
     endif
    endif
endif

CXXFLAGS     += $(ILCINC) -g


SRCS         += G__$(PACKAGE).cxx
OBJS          = $(SRCS:.cxx=.o)

lib$(PACKAGE).so: $(OBJS)
	@echo "Linking" $@ ...
	@/bin/rm -f $@
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS)
	@chmod a+x $@
	@echo "done"

%.o:    %.cxx %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) *.so G__$(PACKAGE).*

G__$(PACKAGE).cxx G__$(PACKAGE).h: $(HDRS) $(DHDR)
	@echo "Generating dictionary ..."
	rootcint -f $@ -c $(CINTFLAGS) $(ILCINC) $^

