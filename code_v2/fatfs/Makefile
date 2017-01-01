objs := diskio.o ff.o

fat.a: $(objs)
	${AR} -r -o $@ $^

%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f fat.a *.o		
	
