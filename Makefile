CC      = g++
PROF    =
NOCRYPT =
C_FLAGS = -Wall -ggdb3 -O2 -march=nocona -pipe

L_FLAGS = 

O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o act_special.o \
          comm.o const.o db.o fight.o handler.o healer.o interp.o \
          magic.o save.o skills.o special.o update.o \
          mob_commands.o olc.o string.o bit.o recycle.o \
	  gamble.o scr.o raedit.o tedit.o room_commands.o \
	  obj_commands.o oedit.o note.o redit.o medit.o gedit.o \
	  forge.o

O_FILES2 = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o act_special.o \
          comm.o const.o db.o fight.o handler.o healer.o interp.o \
          magic.o save.o skills.o special.o update.o \
          mob_commands.o olc.o string.o bit.o recycle.o \
	  gamble.o scr.o raedit.o tedit.o room_commands.o \
	  obj_commands.o oedit.o note.o redit.o medit.o gedit.o \
	  forge.o

.SUFFIXES: .cxx


dreams: $(O_FILES)
	rm -f dreams
	$(CC) $(L_FLAGS) -o dreams $(O_FILES) -lm

scr: $(O_FILES2)
	rm -f dreams
	$(CC) $(L_FLAGS) -o dreams $(O_FILES2)

.c.o: merc.h funct.h
	$(CC) -c $(C_FLAGS) $< 

.cxx.o: merc.h funct.h
	$(CC) -c $(C_FLAGS) $<

clean:
	-rm -f dreams *.o *~ DEADJOE  
