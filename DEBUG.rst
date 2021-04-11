Instrumenting arrays::

  sed -e -f Firmware/guard-arr.sed Firmware/*.h Firmware/*.c*

Instrumenting functions for recursion::

  for f in Firmware/*.h Firmware/*.c*l; do
    perl -p -f Firmware/guard-rec.pl $f > $f.tmp && mv $f.tmp $f
  done
