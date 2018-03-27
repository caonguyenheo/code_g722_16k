#CC_PREFIX   =
CC          = $(CC_PREFIX)gcc
STRIP       = $(CC_PREFIX)strip
SRCDIR      = ./src
INCDIR      = ./inc
LIBDIR      = ./lib
OBJS        = $(patsubst %.c, %.o, $(wildcard $(SRCDIR)/*.c))
CFLAGS      = -Wall -I $(INCDIR)
LDFLAGS     =
TARGET      = g722_codec

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)
	$(STRIP) $(TARGET)
	
%o:%c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *~ *.bak *.a
	rm -f *.bak $(INCDIR)/*.bak $(INCDIR)/*~ *~
	rm -f $(SRCDIR)/*.bak $(SRCDIR)/*.o $(SRCDIR)/*~

pack:
	$(MAKE) clean
	rm -rf $(TARGET).tar.bz2
	tar -cjvf $(TARGET).tar.bz2 inc lib src Makefile
