PACKAGE = invalid-only-for-proof

include Makefile.arch

SRCS     = $(wildcard *.cxx)
HDRS     = $(SRCS:.cxx=.h)
DHDR     = $(PACKAGE)LinkDef.h

ifndef PACKCXXFLAGS
   PACKCXXFLAGS = $(CXXFLAGS)
endif

ILCINC = -I.

### define include dir for local case and par case

ifneq ($(STEERBase_INCLUDE),)
  ILCINC += -I../$(STEERBase_INCLUDE)
endif

# only if no par file was loaded before
#ifeq ($(ILCINC),-I.)
  ifneq ($(ILC_ROOT),)
    ILCINC += -I$(ILC_ROOT)/include
  endif
#endif

CXXFLAGS += $(ILCINC) -g

SRCS         += G__$(PACKAGE).cxx
OBJS          = $(SRCS:.cxx=.o)

lib$(PACKAGE).so: $(OBJS)
	@echo "Linking" $@ ...
	@/bin/rm -f $@
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS)
	@chmod a+x $@
	@echo "done"

%.o:    %.cxx %.h
	$(CXX) $(PACKCXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) *.so G__$(PACKAGE).*

G__$(PACKAGE).cxx G__$(PACKAGE).h: $(HDRS) $(DHDR)
	@echo "Generating dictionaries ..." $(ILCINC)
	rootcint -f $@ -c $(CINTFLAGS) $(ILCINC) $^

