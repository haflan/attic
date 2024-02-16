autocmd BufWritePost *.c :silent exec '!gcc -o senc senc.c'
