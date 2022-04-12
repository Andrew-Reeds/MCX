CC:=
CFLAGS:=

OBJS:=$(patsubst %.c,%.o,$(wildcard c/*.c))
DFILES:=$(patsubst %.o,%.d,$(OBJS))

all: mcx

mcx: $(OBJS)

clean:
	rm -rf $(OBJS) $(DFILES)

$(OBJS):
-include $(DFILES)
