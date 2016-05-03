

SOURCES = ./aisparser/nmea.c \
 ./aisparser/sixbit.c \
 ./aisparser/vdm_parse.c \


ais2geojson: ais2geojson.c

	$(CC) -lm -I./aisparser -o ais2geojson \
		ais2geojson.c \
		aismsg_pos.c \
		$(SOURCES)

test: ais2geojson
	./test.sh
