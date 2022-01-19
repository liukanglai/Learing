" rainbow
"let g:rainbow_active = 1 "0 if you want to enable it later via :RainbowToggle
"let g:rbpt_colorpairs = [
    "\ ['brown',       'RoyalBlue3'],
    "\ ['Darkblue',    'SeaGreen3'],
    "\ ['darkgray',    'DarkOrchid3'],
    "\ ['darkgreen',   'firebrick3'],
    "\ ['darkcyan',    'RoyalBlue3'],
    "\ ['darkred',     'SeaGreen3'],
    "\ ['darkmagenta', 'DarkOrchid3'],
    "\ ['brown',       'firebrick3'],
    "\ ['gray',        'RoyalBlue3'],
    "\ ['black',       'SeaGreen3'],
    "\ ['darkmagenta', 'DarkOrchid3'],
    "\ ['Darkblue',    'firebrick3'],
    "\ ['darkgreen',   'RoyalBlue3'],
    "\ ['darkcyan',    'SeaGreen3'],
    "\ ['darkred',     'DarkOrchid3'],
    "\ ['red',         'firebrick3'],
    "\ ]
"let g:rbpt_max = 16
"let g:rbpt_loadcmd_toggle = 0
"au VimEnter * RainbowParenthesesToggle
"au Syntax * RainbowParenthesesLoadRound
"au Syntax * RainbowParenthesesLoadSquare
"au Syntax * RainbowParenthesesLoadBraces

" neoformat
augroup fmt
  autocmd!
  autocmd BufWritePre * undojoin | Neoformat
augroup END

" Config

" latex
let g:livepreview_engine = 'xelatex'
let g:livepreview_previewer = 'okular'

" indent
let g:indentLine_enabled = 1
"let g:indentLine_char_list = ['┆']
let g:indentLine_char_list = ['¦']


"NERDTree
"autocmd VimEnter * NERDTree | wincmd p
"let NERDTreeShowBookmarks = 1
" directory
autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * if argc() == 1 && isdirectory(argv()[0]) && !exists('s:std_in') |
    \ execute 'NERDTree' argv()[0] | wincmd p | enew | execute 'cd '.argv()[0] | endif
" Exit Vim if NERDTree is the only window remaining in the only tab.
autocmd BufEnter * if tabpagenr('$') == 1 && winnr('$') == 1 && exists('b:NERDTree') && b:NERDTree.isTabTree() | quit | endif
" Open the existing NERDTree on each new tab.
"autocmd VimEnter * NERDTree | wincmd p

" prettier
let g:prettier#autoformat = 1
let g:prettier#autoformat_require_pragma = 0
" when running at every change you may want to disable quickfix
let g:prettier#quickfix_enabled = 0

"autocmd TextChanged,InsertLeave *.js,*.jsx,*.mjs,*.ts,*.tsx,*.css,*.less,*.scss,*.json,*.graphql,*.md,*.vue,*.svelte,*.yaml,*.html PrettierAsync

let g:ale_fix_on_save = 0

"let g:ale_sign_column_always = 1
let g:ale_sign_error = '●'
let g:ale_sign_warning = '▶'

let g:ale_disable_lsp = 1
"let g:airline#extensions#ale#enabled = 1

let g:ale_completion_enable = 0

let g:ale_set_loclist = 0
let g:ale_set_quickfix = 1
"let g:ale_open_list = 1
"let g:ale_keep_list_window_open = 1
"let g:ale_list_window_size = 5

" nmap <silent> <C-k> <Plug>(ale_previous_wrap)
nmap <silent> tam <Plug>(ale_previous_wrap)
" nmap <silent> <C-j> <Plug>(ale_next_wrap)
nmap <silent> tan <Plug>(ale_next_wrap)
"nmap <silent> tae :ALECodeAction<CR>
