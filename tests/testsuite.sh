#!/bin/bash

VRAI_OUTPUT=".vraibash"
NOTRE_OUTPUT=".42shbash"

reussite=0

test() {
    ./$1 > "$VRAI_OUTPUT"
    vraioutput1=$VRAI_OUTPUT;
    timeout 1s ./../src/42sh -c "$(cat $1)" > "$NOTRE_OUTPUT"
    if [ $? -eq 124 ]; then
        echo --------------------------
        echo "Test $1"
        echo -e shell :; cat $vraioutput1;echo -e '\n'42sh :; echo -e '\n'infinity loop'\n';
        return 0
    fi
    var1=$(diff $VRAI_OUTPUT $NOTRE_OUTPUT)
    notreoutput=$NOTRE_OUTPUT

    if [ -z "$var1" ]
    then
        ((reussite++));
    else
        echo --------------------------
        echo "Test $1"
        echo -e shell :; cat $vraioutput1;echo -e '\n'42sh :; cat $notreoutput ; echo ;
    fi
}

for file in "../tests/liste_test"/*; do
    if [ -f "$file" ]; then
        test "$file"
    fi
done

echo ---------------------------------
echo Number of test passed $reussite / 19
echo ---------------------------------