CFLAGS		:= -Wall -Wextra
SRCS		:= $(wildcard *.c)
BINS		:= $(SRCS:.c=)
INSTALLPATH := $(HOME)/.config/i3blocks/

volume: LDLIBS += -lpulse

.PHONY: all clean install
all: $(BINS)

%: %.c
	$(CC) -o $@ $< $(CFLAGS) $(LDLIBS)

install: all
	install $(BINS) config $(INSTALLPATH)

clean:
	rm -f $(BINS)
