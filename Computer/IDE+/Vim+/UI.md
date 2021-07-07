# color

## Terminal Colors https://gist.github.com/XVilka/8346728

- printf "\x1b[38;2;255;100;0mTRUECOLOR\x1b[0m\n"
- colorscheme elflord
- 24位颜色(真彩色), set termguicolors
> .tmux.conf: set -g default-terminal "xterm-256color"
>> .screenrc: term "xterm-256color"
> .zshrc: TERM=xterm-256color // not a good idea.

## colorthem

- https://github.com/morhetz/gruvbox
- :colorscheme gruvbox
- set background=dark
> plugin: scrollcolors, vim-colorschemes

# StatusBar
- set laststatus=2 "总显示状态栏
- set showcmd " 在状态栏显示最后的命令

1. powerline
    - python3 -m pip install powerline-status
    - .zshrc: PATH=$HOME/.local/bin:$PATH
    - set laststatus=2
    - python3 from powerline.vim import setup as powerline_setup
    - python3 powerline_setup()
    - python3 del powerline_setup

2. Airline
    - vim-airline/vim-airline
