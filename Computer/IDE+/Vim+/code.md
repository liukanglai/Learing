# complete

1. vim内置
- c-p/n
- 插入补全模式：c-x, than: c-i(补全整行) c-j(补全标签) c-f(补全文件名) s(拼写建议的补全, set spell)
> :help ins-completion :help 'complete'

2. YouCompleteMe

# browse


- gd/D: 跳转函数定义

1. Exuberant ctag
    - pacman -S ctags
    - ctags -R(create tags)
    - c-]: 会跳转, c-t: turn back, 若有同名 :tn(next) :tp(previous) :ts(select)
    - g]: select
    - vim -t tags(直接跳转)
    - " Regenerate tags when saving Python files.  autocmd BufWritePost \*.py silent! !ctags -R &

            set tags=tags;  " 在父目录中递归查找tags，; 使持续查找
            set tags=/home/tags  " set a folder
2. taglist/tagbar


# undotree
- gundo.vim
- noremap <f5> :GundoToggle<cr>
> help: undo-tree

# git(VCS)
vim-fugitive
- :Gstatus
        
        c-n/p to move
        -: 移入/出暂存区
        cc/:Gcommit 提交
        D/:GDiff 对比
        g? help
- :Glog
- :Gblame

        C/A/D to resize
        Enter
        o分割打开
        g?

- :Gread read the specified file
- :Ggrep 
- :Gmove move file
- :Gdelete
> :help fugitive

# vimdiff
- vimdiff \*.md \*.md 
- - ]c [c to move
- do/:diffget get the different content
- dp/:diffput put the different content
- :%diffget/:%diffput all file to copy
- 手动修改需更新高亮 :diffupdate/:diffu
- 可多个文件, 但无do/dp :diffget/:diffput 需指定参数:ls!或文件名

## for git

1. git config
        
        $ git config --global merge.tool vimdiff
        $ git config --global merge.conflictstyle diff3
        $ git config --global mergetool.prompt false

2. 略

# with tmux
vim-tmux-navigator
- c-h/j/k/l to move


# screen

- see it in vim wikia

# make

- :compiler gcc
- :set errorformat 错误格式
- :set makeprg 运行make使用哪个命令行程序
- :make

# 测试
plug: vim-test

# Linting

> python use Pylint Flake8 autopep8

1. Linter
    - :make | copen

        " Use :make to run pylint for Python files.
        autocmd filetype python setlocal makeprg=pylint3\ --reports=n\ --msg-template=\"{path}:{line}:\ {msg_id}\ {symbol},\ {obj}\ {msg}\"\ %:p
        autocmd filetype python setlocal errorformat=%f:%l:\ %m

2. syntastic(plug)
    - 显示的是位置列表
        
        " Plug 'vim-syntastic/syntastic'

        " set statusline+=%#warningmsg#
        " set statusline+=%{SyntasticStatuslineFlag()}
        " set statusline+=%*
        
        " let g:syntastic_always_populate_loc_list = 1
        " let g:syntastic_auto_loc_list = 1
        " let g:syntastic_check_on_open = 1
        " let g:syntastic_check_on_wq = 0
        
        " let g:syntastic_python_pylint_exe = 'pylint3'

3. ale(异步调用)
    - :lopen to open
    - :ALEToggle to open and close
    - :ALEGoToDefinition 跳转定义
    - :ALEFindReferences 查找定义的引用
    "自动补全
    let g:ale_completion_enable = 1

# arglist(argument list) 

do a same operation in multiple files

- :arg \*\*/\*.py (define files)
- :argdo %s/hell/hello/ge | update
- :args (to display files)

- :set hidden (same with e)
- :wa (save all)

- :bufdo(argdo) (all buffer do)

# refactor

- use regex to substitute, search, 宏...
- google: vim refactoring plugins

# debug

- termdebug(plug)

