COMPILER  = clang
CFLAGS    = -O0 -g -MD -Wall
INCLUDE   = -I./include
TARGET    = ./VMMLoaderPkg/vmm_entry.elf
SRCDIR    = ./src
SOURCES := $(shell find $(SRCDIR) -name *.c -or -name *.asm)
OBJDIR    = ./obj
OBJECTS   = $(OBJDIR)/vmm_entry.o $(OBJDIR)/window.o $(OBJDIR)/font.o $(OBJDIR)/memory.o $(OBJDIR)/assembly.o $(OBJDIR)/serial.o $(OBJDIR)/segment.o $(OBJDIR)/util.o $(OBJDIR)/interrupt.o $(OBJDIR)/vmx_init.o 
DEPENDS   = $(OBJECTS:.o=.d)
LDFLAGS = --entry vmmEntry --image-base 0x7526000 --static -nostdlib 

$(TARGET): $(OBJECTS)
	ld.lld $(LDFLAGS) -o $(TARGET) $(OBJECTS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	-mkdir -p $(OBJDIR)
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<

$(OBJDIR)/%.o: $(SRCDIR)/%.asm
	-mkdir -p $(OBJDIR)
	nasm -f elf64 -o $@ $<

all: clean $(TARGET)

clean:
	rm -rf $(OBJECTS) $(DEPENDS) $(TARGET)

-include $(DEPENDS)