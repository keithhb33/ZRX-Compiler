call gcc -o zrx_compiler src/zrx_compiler.c
call ./zrx_compiler "$1"
call gcc output.c -o output
call rm output.c
call ./output