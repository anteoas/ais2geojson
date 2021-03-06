# ais2geojson

A really fast and simple tool to parse AIS raw data into geojson. It processes AIS position message classes 1, 2, 3, 4, 9, 18, 19 line-by-line and outputs GeoJson:

```
echo '!BSVDM,1,1,,A,13oUhr0000151NhWKUi1vPvJ08Re,0*3A' | ais2geojson  | jq '.'
{
  "type": "Feature",
  "geometry": {
    "type": "Point",
    "coordinates": [
      15.078333,
      68.910833
    ]
  },
  "properties": {
    "mmsi": "259617000",
    "cog": 50.6,
    "sog": 0,
    "ts": false
  }
}
```

It can process around 350k messages per second. It currently takes in
no arguments.

# Timestamps prefix

As timestamps are not accurately present in the AIS messages
themselves, ais2geojson will include the timestamp proceeding a `;` if
present. This lets you do:

```
echo '2017-02-20T00:00:00;!BSVDM,1,1,,A,13oUhr0000151NhWKUi1vPvJ08Re,0*3A' | ais2geojson  | jq '.'
{
  "type": "Feature",
  "geometry": {
    "type": "Point",
    "coordinates": [
      15.078333,
      68.910833
    ]
  },
  "properties": {
    "mmsi": "259617000",
    "cog": 50.6,
    "sog": 0,
    "ts": "2017-02-20T00:00:00"
  }
}
```

If the timestamp can be parsed as an integer, the JSON printout will
reflect this:

```
$ echo '123;!BSVDM,1,1,,A,13oUhr0000151NhWKUi1vPvJ08Re,0*3A' | ./ais2geojson   | jq -c .properties
{"mmsi":"259617000","aistype":1,"cog":50.6,"sog":0,"smi":0,"ts":123}
```

## TODO

- Allow command line arguments options to include/exclude fields

## Credits

`aisparser` stolen from https://github.com/bcl/aisparser, simplified
and reorganized here.

[Anteo AS](http://anteo.no/) is publishing this piece of software with the BSD license.
