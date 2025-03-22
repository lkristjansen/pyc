CFLAGS += -std=c17 -Wall -Wextra -g -fsanitize=address -static-libasan
CPPFLAGS += 

PYC_SOURCES := $(shell find pyc -name '*.c')
PYC_HEADERS := $(shell find pyc -name '*.h')

bin/pyc: bin bin/runtime.o $(PYC_SOURCES) $(PYC_HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o bin/pyc $(PYC_SOURCES)

RUNTIME_SOURCES := $(shell find runtime -name '*.c')
RUNTIME_HEADERS := $(shell find runtime -name '*.h')

bin/runtime.o: bin $(RUNTIME_SOURCES) $(RUNTIME_HEADERS)
	$(CC) -c -o bin/runtime.o $(RUNTIME_SOURCES)

bin:
	mkdir -p bin

.PHONY: clean
clean:
	rm -rf bin
