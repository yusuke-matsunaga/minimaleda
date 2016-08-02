## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_vl/tests/yacc.test,v $
## $Author: matsunaga $
## $Revision: 1.1.1.1 $
## $Date: 2005/12/11 17:07:29 $

# Common definitions
if test -z "$srcdir"; then
    srcdir=echo "$0" | sed 's,[^/]*$,,'
    test "$srcdir" = "$0" && srcdir=.
    test -z "$srcdir" && srcdir=.
    test "${VERBOSE+set}" != set && VERBOSE=1
fi
. ${srcdir}/local_defs

# Run the test saving stderr to a file, and showing stdout
# if VERBOSE == 1
nerr=0

for file in ${TEST_SRC}/*.blif
do
  file=`echo $file | sed -e "s|${TEST_SRC}/||g"`
  file_flat=`echo $file | sed -e 's|/|_|g'`
  out=${TEST_OUT}/$file_flat.out
  err=${TEST_OUT}/$file_flat.err

  echo -n "reading $file ..."

  ${TEST_PATH}/parsertest ${TEST_SRC}/$file 2> err-tmp | sed -e "s|${TEST_SRC}/||g" > out
  sed -e "s|${TEST_SRC}/||g" < err-tmp > err
  rm -f err-tmp

  cp out $out
  cp err $err
done
exit 0
### Local Variables:
### mode: sh
### End:
