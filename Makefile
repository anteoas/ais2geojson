

SOURCES = ./aisparser/nmea.c \
 ./aisparser/sixbit.c \
 ./aisparser/vdm_parse.c \


ais2geojson: ais2geojson.c aismsg_pos.h aismsg_pos.c

	$(CC) -O5 -lm -I./aisparser -o ais2geojson \
		ais2geojson.c \
		aismsg_pos.c \
		$(SOURCES)

test: ais2geojson
	./test.sh

install: ais2geojson
	install ais2geojson /usr/bin/
