# lunarvim

## config

1. 按照文档安装

- install cargo: run the script it gets
- sudo pacman -S lazygit zoxide ripgrep sqlite fd yarn lldb go unzip
- 记得设置 npm 的 prefix 以设置可访问的全局安装路径: `npm config set perfix ~/.local`

1. lsp

- The default html server has bug which does not support embeded javascript completion.
- npm i -g vscode-html-languageserver-bin (will be started when you open html file)
- MasonInstall cpplint markdownlint prettier proselint
- sudo pacman -S cppcheck

## use

1. comment:

- first v, then gb
- gcc, gcA, gco, gcO

1. lsp:

- K, gd, gD, gr!!!, gI, gs, gl, leader+lr

1. in completion:

- enter to confirm
- C-e to abort
- Tab or C-j/k to select

- use snippet: enter+tab, back: shift+tab (难用，当有补全时，先按 C-e，再 Tab)

1. telescope

- leader+sr
- leader+st
- leader+f

1. packer

- leader+ps
- leader+pc
- leader+pS
