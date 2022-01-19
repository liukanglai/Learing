let mapleader = ' '

" 可视模式下系统粘贴
vnoremap Y "+y

noremap <LEADER>qq :q!<CR>
noremap <LEADER>wq :q<CR>
noremap <LEADER>ww :wq<CR>

noremap <LEADER>oi :e $HOME/.config/nvim/init.vim<CR>
noremap <LEADER>om :e $HOME/.config/nvim/core/map.vim<CR>
noremap <LEADER>or :e $HOME/.config/nvim/core/vimrc.vim<CR>

map <LEADER>oo :noh<CR>
map <LEADER>0  $
" for long sentence, go next line
noremap j gj
noremap k gk

"map <LEADER>ff za


" Tab management
noremap tt :e 
noremap tm :bp<CR>
noremap tn :bn<CR>
noremap tq :bd<CR>

noremap <LEADER>n :NERDTree<CR>
noremap <LEADER>t :Tagbar<CR>
noremap <LEADER>ml :LLPStartPreview<CR>
noremap <LEADER>mm :MarkdownPreview<CR>
map <LEADER>mt :TableModeToggle<CR>
"nnoremap <LEADER>u :UndotreeToggle<CR>

" === Window management
"noremap <LEADER>wk <C-w>k
"noremap <LEADER>wj <C-w>j
"noremap <LEADER>wh <C-w>h
"noremap <LEADER>wl <C-w>l


" split the screens to up (horizontal), down (horizontal), left (vertical), right (vertical)
noremap <LEADER>ws :set splitbelow<CR>:split<CR>
noremap <LEADER>wv :set splitright<CR>:vsplit<CR>

"" Press space twice to jump to the next '<++>' and edit it
"noremap <LEADER><LEADER> <Esc>/<++><CR>:nohlsearch<CR>c4l

"" Compile function
noremap <LEADER>rr :call CompileRunGcc()<CR>
func! CompileRunGcc()
  exec "w"
  if &filetype == 'c'
    exec "!gcc % -o %<"
    "exec "! ./%<"
    exec "!time ./%<"
    exec "! rm %<"
  elseif &filetype == 'cpp'
    set splitbelow
    exec "!g++ -std=c++11 % -Wall -o %<"
    :sp
    :res -15
    :term ./%<
  elseif &filetype == 'java'
    set splitbelow
    :sp
    :res -5
    term javac % && time java %<
  elseif &filetype == 'sh'
    :!time bash %
"exec "!chmod a+x %"
"exec "!./%"
  elseif &filetype == 'python'
    set splitbelow
    :sp
    :term python3 %
  elseif &filetype == 'html'
    silent! exec "!".g:mkdp_browser." % &"
  elseif &filetype == 'markdown'
    exec "InstantMarkdownPreview"
  elseif &filetype == 'tex'
    silent! exec "VimtexStop"
    silent! exec "VimtexCompile"
  elseif &filetype == 'dart'
    exec "CocCommand flutter.run -d ".g:flutter_default_device." ".g:flutter_run_args
    silent! exec "CocCommand flutter.dev.openDevLog"
  elseif &filetype == 'javascript'
    set splitbelow
    :sp
    :term export DEBUG="INFO,ERROR,WARNING"; node --trace-warnings .
  elseif &filetype == 'go'
    set splitbelow
    :sp
    :term go run .
  endif
endfunc


" gdb调试
map <LEADER>rg :call Debug()<cr>
func!  Debug()
exec "w"
exec "!gcc -g -o %< %"
exec ":packadd termdebug"
exec ":Termdebug %<"
endfunc

autocmd BufNewFile *.py,*.c,*.sh,*.java exec ":call SetTitle()"
func SetTitle()
    if expand ("%:e") == 'sh'
        call setline(1, "#!/bin/bash")
        call setline(2, "#Author:bert")
        call setline(3, "#Blog:https://blog.51cto.com/zpf666")
        call setline(4, "#Time:".strftime("%F %T"))
        call setline(5, "#Name:".expand("%"))
        call setline(6, "#Version:V1.0")
        call setline(7, "#Description:This is a production script.")
      elseif expand ("%:e") == 'c'
        call setline(1, "// Time:".strftime("%F %T"))
        call setline(2, "#include <stdio.h>")
        call setline(3, "")
        call setline(4, "int main(void){")
        call setline(5, "}")
      elseif expand ("%:e") == 'py'
        call setline(1, "#!/bin/bash")
        call setline(2, "#Author:bert")
        call setline(3, "#Blog:https://blog.51cto.com/zpf666")
        call setline(4, "#Time:".strftime("%F %T"))
        call setline(5, "#Name:".expand("%"))
        call setline(6, "#Version:V1.0")
        call setline(7, "#Description:This is a production script.")
      elseif expand ("%:e") == 'java'
        call setline(1, "#!/bin/bash")
        call setline(2, "#Author:bert")
        call setline(3, "#Blog:https://blog.51cto.com/zpf666")
        call setline(4, "#Time:".strftime("%F %T"))
        call setline(5, "#Name:".expand("%"))
        call setline(6, "#Version:V1.0")
        call setline(7, "#Description:This is a production script.")
    endif
endfunc



