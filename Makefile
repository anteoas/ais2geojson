SOURCES = ./aisparser/nmea.c \
 ./aisparser/sixbit.c \
 ./aisparser/vdm_parse.c \


aisformat: aisformat.c aismsg_pos.h aismsg_pos.c

	$(CC) -O5 -lm -I./aisparser -o aisformat \
		aisformat.c \
		aismsg_pos.c \
		$(SOURCES)

test: aisformat
	./test.sh

install: aisformat
	mkdir -p ${DESTDIR}${PREFIX}/usr/bin/
	install aisformat ${DESTDIR}${PREFIX}/usr/bin/
