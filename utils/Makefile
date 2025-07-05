OUTPUTDIR = ../
CC := gcc
 
 
USR_TEST_SRC = app.c
OBJ = $(patsubst %.c,$(OUTPUTDIR)/%.o,$(notdir $(SRC)))
TARGET = app

$(TARGET):
	$(CC) $(USR_TEST_SRC) -g -o $(TARGET)
	cp $(TARGET) ../build



clean:
	rm -f $(TARGET)