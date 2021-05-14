#BUILD_WITH_SHARED=y
#BUILD_WITH_STATIC=y

CC=gcc
AR=ar
C_FLAGS=-Wall -O2
LD_FLAGS=-lpthread


INC=-I./wlog -I./StdServie

WSRC=$(wildcard ./wlog/*.c)
WOBJ=$(WSRC:%.c=%.o)

SRC=main.c
OBJ=$(SRC:%.c=%.o)

WSO=libwlog.so
WA =libwlog.a
TARGET=testwlog

all:$(TARGET)

ifeq ($(BUILD_WITH_SHARED),y)
$(TARGET):$(OBJ) $(WSO)
	$(CC) $(OBJ) $(WSO) $(LD_FLAGS) -o $@		

$(WSO):$(WOBJ)
	$(CC) $(WOBJ) $(LD_FLAGS) -shared -o $@

$(WOBJ):%.o:%.c
	$(CC) $(C_FLAGS) $(INC) $< -fPIC -c -o $@
else ifeq ($(BUILD_WITH_STATIC),y)
$(TARGET):$(OBJ) $(WA)
	$(CC) $(OBJ) $(WA) $(LD_FLAGS) -o $@		

$(WA):$(WOBJ)
	$(AR) rcs $@ $(WOBJ) 

$(WOBJ):%.o:%.c
	$(CC) $(C_FLAGS) $(INC) $< -c -o $@
else
$(TARGET):$(OBJ) $(WOBJ)
	$(CC) $(OBJ) $(WOBJ) $(LD_FLAGS) -o $@

$(WOBJ):%.o:%.c
	$(CC) $(C_FLAGS) $(INC) $< -c -o $@
endif

$(OBJ):%.o:%.c
	$(CC) $(C_FLAGS) $(INC) $< -c -o $@


.PHONY:clean
clean:
	rm -f $(TARGET) $(OBJ) $(WOBJ) $(WSO) $(WA)

