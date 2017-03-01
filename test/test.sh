#!/bin/sh

echo '2015-09-20T17:00:00;!BSVDM,1,1,,B,13mBwe0P00Pa6:hTHmD>4?vT2<08,0*12' \
    | ./ais2geojson | grep '"mmsi":"257212340"'


## these are test cases with SMI 0, 1 and not there (ais message class
## 18).
## expected output is from https://github.com/anteoas/ais-converter

echo
echo '2017-03-01T13:41:14;!BSVDM,1,1,,A,33m:Ul0P@t0w2ITVsUk=cJtL01q@,0*6A' | ./ais2geojson
echo expected:
echo '{"type":"Feature","geometry":{"type":"Point","coordinates":[13.77075,68.03703333333334]},"properties":{"mmsi":257074640,"cog":350.1,"sog":6,"trueHeading":350,"aistype":3,"smi":0,"ts":"2017-03-01T13:41:14"}}'
echo

echo
echo '2017-03-01T13:41:14;!BSVDM,1,1,,A,B3m6?EP0@H9sCka8ibdtKwVUoP06,0*21' | ./ais2geojson
echo expected:
echo '{"type":"Feature","geometry":{"type":"Point","coordinates":[8.674091666666667,63.87313833333334]},"properties":{"mmsi":257003350,"cog":96.6,"sog":6.5,"trueHeading":511,"aistype":18,"ts":"2017-03-01T13:41:14"}}'
echo

echo
echo '2017-03-01T13:41:14;!BSVDM,1,1,,B,13nW`F001V0pk1jV0r5s@8rHP<0i,0*13' | ./ais2geojson
echo expected:
echo '{"type":"Feature","geometry":{"type":"Point","coordinates":[12.407561666666668,66.43459833333333]},"properties":{"mmsi":258599000,"cog":288,"sog":10.2,"trueHeading":285,"aistype":1,"smi":1,"ts":"2017-03-01T13:41:14"}}'
echo
