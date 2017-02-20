# ais2geojson

A really fast and simple tool to parse AIS raw data into geojson. It processes AIS position message classes 1, 2, 3, 4, 9, 18, 19 line-by-line and outputs GeoJson:

    echo '!BSVDM,1,1,,A,145<vf70000LfCJSffM>49en0@Rf,0*55' | ais2geojson 
    {"type":"Feature","geometry":{"type":"Point","coordinates":[6.274742,62.442860]},"properties":{"mmsi":"273891000","sog":0.0,"ts":false}}

It can process around 150 messages per second. It currently takes in no arguments.

# Timestamps

As timestamps are not accurately present in the AIS messages themselves, ais2geojson will include the timestamp proceeding a `;` if present. This lets you do:

    echo '2017-02-20T00:00:00;!BSVDM,1,1,,A,13oUhr0000151NhWKUi1vPvJ08Re,0*3A' | ais2geojson 
    {"type":"Feature","geometry":{"type":"Point","coordinates":[15.078333,68.910833]},"properties":{"mmsi":"259617000","cog":50.6,"sog":0.0,"ts":1487545200}}

## TODO

- Make the timestamp dumber: copy whatever's the `;` into the `ts` property, no need to parse
- Customizaeable output
  - Include the `smi` field
- Allow command line arguments options to include/exclude fields

## Credits

`aisparser` stolen from https://github.com/bcl/aisparser, simplified
and reorganized here.
