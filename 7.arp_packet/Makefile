CPPFLAGS = -I include -Wall -pthread

src = $(wildcard src/*.c) $(wildcard src/**/*.c)
obj = $(src:src/%.c=build/%.o)
headers = $(wildcard include/*.h) $(wildcard include/**/*.h)

lvl-ip: $(obj)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(obj) -o lvl-ip

build/%.o: src/%.c $(headers)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

debug: CFLAGS+= -g
debug: lvl-ip

clean:
	rm -rf build/*.o lvl-ip
	rm -rf build/*
	rm -rf build/lib/*.


	
