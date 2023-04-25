file="$1"

if test ! -f $1
    then echo "Not a regular file"
    exit 1
else
    echo "\n-------------------------------------------------------------------\n"
    echo "The provided argument is a C file. In addition, it will be compiled:"

    gccOutput=$(gcc -Wall -Wextra "$file" 2>&1)
    noOfErrors=$(echo "$gccOutput" | grep -c 'error')
    noOfWarnings=$(echo "$gccOutput" | grep -c 'warning')

    echo "Errors: $noOfErrors"
    echo "Warnings: $noOfWarnings"
    echo "\n-------------------------------------------------------------------\n"
fi