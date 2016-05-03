
echo '2015-09-20T17:00:00;!BSVDM,1,1,,B,13mBwe0P00Pa6:hTHmD>4?vT2<08,0*12' \
    | ./ais2geojson | grep '"mmsi":"257212340"'
