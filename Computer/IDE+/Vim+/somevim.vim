" tags父目录递归查找 ;表示持续查找
"set tags=tags;
" Regenerate tags when saving Python files.
"autocmd BufWritePost *.py silent! !ctags -R &
"autocmd BufWritePost *.c silent! !ctags -R &
"autocmd BufWritePost *.cpp silent! !ctags -R &


