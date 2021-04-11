
# Program settings
INCLUDES	:=	include
LIBRARIES	:=
SOURCES		:=	source
TARGET		:=	bin/nds_iconconverter

# Language settings
CFLAGS	:=	-Wall -O2 -std=c++17
BUILD	:=	build
CXX		:=	g++
CC		:=	gcc

########################################################################################

CFILES		:=	$(foreach dir,$(SOURCES), \
				$(wildcard $(dir)/*.c))
CCFILES		:=	$(foreach dir,$(SOURCES), \
				$(wildcard $(dir)/*.cc))
OFILES		:=	$(patsubst %.cc,%.o,$(CCFILES))
OFILES		+=	$(patsubst %.c,%.o,$(CFILES))
INCLUDES	:=	$(addprefix -I,$(INCLUDES))

OBJMAKE		 =	-c $(<) -o $(@)
LDMAKE		 =	-o $(TARGET) $(^)

############################################################

ifeq ($(strip $(CCFILES)),)
	LD	:=	$(CC)
else
	LD	:=	$(CXX)
endif

ifeq ($(OS),Windows_NT)
	CFLAGS	+=	-D__windows__
endif

############################################################

.PHONY: all
all: $(TARGET)
	@echo "🎉 All done"


.PHONY: clean
clean:
	@echo "🧹 Cleaning ... "
	@rm -rf	$(foreach dir,$(SOURCES), \
			$(wildcard $(dir)/*.o)) \
			$(foreach dir,$(SOURCES), \
			$(wildcard $(dir)/*.d)) \
			$(TARGET) $(BUILD)

###########################################################

DEPS	:=	$(addprefix $(BUILD)/,$(patsubst %.o,%.d,$(OFILES)))
DEPFLAG	 =	-MMD -MF $(BUILD)/$(@:.o=.d)
-include $(DEPS)

###########################################################

$(TARGET): $(OFILES)
	@mkdir -p bin
	@echo "🔗 Linking $(@) ... "
	@$(LD) $(LDMAKE) $(CFLAGS) $(LIBRARIES)

%.o: %.c
	@echo "🚧 Compiling C object $(<) -> $(@) ... "
	@mkdir -p $(dir $(BUILD)/$(@))
	@$(CC) $(OBJMAKE) $(INCLUDES) $(CFLAGS) $(DEPFLAG)

%.o: %.cc
	@echo "🚧 Compiling CXX object $(<) -> $(@) ... "
	@mkdir -p $(dir $(BUILD)/$(@))
	@$(CXX) $(OBJMAKE) $(INCLUDES) $(CFLAGS) $(DEPFLAG)
