TARGET = lcc

CC = gcc
CFLAGS = -g -Wfatal-errors

OUTDIR = bin
DATADIR = data
SUBDIR = src
DIR_OBJ = obj

INCS = $(wildcard *.h $(foreach fd, $(SUBDIR), $(fd)/*.h))
SRCS = $(wildcard *.c $(foreach fd, $(SUBDIR), $(fd)/*.c))
NODIR_SRC = $(notdir $(SRCS))
OBJS = $(addprefix $(DIR_OBJ)/, $(SRCS:c=o)) # obj/xxx.o obj/folder/xxx .o
INC_DIRS = -I./ $(addprefix -I, $(SUBDIR))
LIBS =
LIB_DIRS =

PHONY := $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $(OUTDIR)/$@ $(OBJS) $(LIB_DIRS) $(LIBS)

$(DIR_OBJ)/%.o: %.c $(INCS)
	mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $< $(INC_DIRS)

PHONY += clean
clean:
	rm -rf $(OUTDIR)/* $(DATADIR)/* $(DIR_OBJ)/*

PHONY += echoes
echoes:
	@echo "INC files: $(INCS)"
	@echo "SRC files: $(SRCS)"
	@echo "OBJ files: $(OBJS)"
	@echo "LIB files: $(LIBS)"
	@echo "INC DIR: $(INC_DIRS)"
	@echo "LIB DIR: $(LIB_DIRS)"

.PHONY = $(PHONY)
