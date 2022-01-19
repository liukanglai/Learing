
" 配色
let &t_ut=''
set autochdir

set encoding=utf-8

exec "nohlsearch"

filetype plugin indent on

set number
set relativenumber

"用浅色高亮显示当前行"
set nocursorcolumn "竖
set cursorline "横
autocmd InsertLeave * se cul
autocmd InsertEnter * se nocul

set hidden
set expandtab " tab 展开为空格
set smarttab
set shiftwidth=2
set tabstop=2

"开启新行时使用智能自动缩进"
set smartindent
set cindent
set list
set scrolloff=5 "光标移动到buffer的顶部和底部时保持5行距离
set ttimeoutlen=100

set viewoptions=cursor,folds,slash,unix
set wrap "自动换行”
set tw=0 "光标超过0列折行
set indentexpr=
"set showmatch " 括号匹配
set sm "显示括号配对情况。打开这个选项后，当输入后括号(包括小括号、中括号、大括号) 的时候，光标会跳回前括号片刻，然后跳回来，以此显示括号的配对情况。

" autocmd BUfRead * normal zR " open a file, open all fold.

"set foldenable
" 使用 foldexpr 指定的方式折叠代码
"set foldmethod=expr
" 使用 treesitter 根据语言语法折叠代码
"set foldexpr=nvim_treesitter#foldexpr()
" 默认从第一级开始，大家可以去掉看有什么效果
"set foldlevel=1

" set split
set splitright
set splitbelow
set noshowmode
set showcmd " last command
set wildmenu "增强模式中的命令行自动完成操作"
" use together to search
set ignorecase
set smartcase
syntax on
set autoread
set report=0 "通过使用: commands命令，告诉我们文件的哪一行被改变过"
"被分割的窗口间显示空白，便于阅读"
set fillchars=vert:\ ,stl:\ ,stlnc:\
"set spell " 拼写检查 (c-x, than s)
set ruler "显示标尺"

set backspace=eol,start,indent "使回格键（backspace）正常处理indent, eol, start等"
highlight Search ctermbg=black ctermfg=white guifg=white guibg=black
set hlsearch
" incremental search, for all search highlight
set incsearch
set wildignorecase

set nobackup

" Don't pass messages to |ins-completion-menu|.
set shortmess+=c

"文件类型自动检测，代码智能补全"
set completeopt=menu,longest,noinsert,menuone,noselect,preview
set ttyfast "should make scrolling faster
set lazyredraw "same as above
set visualbell

"set colorcolumn=100 中间线
set updatetime=100
set virtualedit=block

" back to last location
au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif

" fcitx
"let g:fcitx5-remote

autocmd InsertLeave * :silent !fcitx5-remote -c " 退出插入模式时禁用输入法加
autocmd BufCreate *  :silent !fcitx5-remote -c " 创建 Buf 时禁用输入法
autocmd BufEnter *  :silent !fcitx5-remote -c " 进入 Buf 时禁用输入法
autocmd BufLeave *  :silent !fcitx5-remote -c " 离开 Buf 时禁用输入法
