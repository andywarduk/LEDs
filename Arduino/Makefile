LEDs.zip: lib
	if [ -f $@ ]; then \
	  rm $@; \
	fi; \
	cd build-lib && zip -r ../$@ *

lib: libsrc examples

libsrc: libraries/LEDs/*
	mkdir -p build-lib >/dev/null 2>&1; \
	cp $^ build-lib

examples: */*.ino
	mkdir -p build-lib/examples >/dev/null 2>&1; \
	tar cf - $^ | tar xvf - -C build-lib/examples
clean:
	rm -rf build-lib LEDs.zip
	