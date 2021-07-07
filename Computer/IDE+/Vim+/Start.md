# install

## windows

1. install Cygwin
2. gvim
> create .bat files for command line use

## linux

1. install 
    
        git clone https://github.com/vim/vim.git
        cd vim/src
        ./configure --with-features=huge --enable-python3interp
        make
        sudo make install
> you may need install other lib

## macOS

1. Homebrew
- brew install vim
- brew upgrade vim
2. MacVim.dmg

# config

- .vimrc: ~/.vim/vimrc  (win: \_vimrc)
- echo %USERPROFILE%
- .vimrc using: open vim, :echo $MYVIMRC 

# swap file (a file's backup)

- put all swap file in a directory: set directory = $HOME/.vim/swap//
- win: = % USERDATA%\.vim\swap//

> set noswapfile

# undofile

    set undofile
    if !isdirectory("$HOME/.vim/undodir")
        call mkdir("$HOME/.vim/undodir", "p")
    endif
    set undodir = "$HOME/.vim/undodir" or "%USERPROFILE%_vim"

# help
- :help
- :h cc
- :h search(if don't know name, do c-d)

# synchronize
- ln -s ~/dotfiles/.vimrc .vim/.vimrc


