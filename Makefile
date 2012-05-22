EXENAME=matcher
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin

.PHONY: all
all: $(EXENAME)

$(EXENAME): main.c matcher.c
	$(CC) $(CFLAGS) -O3 -Wall $^ -o $@

.PHONY: install
install: $(EXENAME)
	install -d $(DESTDIR)$(PREFIX)
	install -m 0755 $< $(DESTDIR)$(PREFIX)/bin
