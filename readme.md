# ais2geojson

A really fast and simple tool to parse AIS raw data into geojson:

```
klm@carmine:~/ais-data$ time cat /home/arp/log/kystverket/2016/05/12/09.log | ais2geojson | grep 257041440 >/dev/null # havlys

real	0m4.028s
user	0m4.022s
sys	0m1.303s
```

It takes 4 seconds to replay an hour of AIS. That's 10 times faster
than the old
[`ais-converter`](https://github.com/anteoas/ais-converter) (which
takes 4 seconds to bootstrap alone).

## TODO

- Make the geojson output identical to ais-converter for backwards compatibility
  - Make the timestamp not an integer (but keep the unix epoch int too, perhaps)
  - Include the `smi` field (for valve status)
- Allow cla options to include/exclude fields

## Credits

`aisparser` stolen from https://github.com/bcl/aisparser, simplified
and reorganized here.
