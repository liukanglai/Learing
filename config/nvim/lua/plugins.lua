local utils = require("utils")
local fn = vim.fn

vim.g.package_home = fn.stdpath("data") .. "/site/pack/packer/"
local packer_install_dir = vim.g.package_home .. "/opt/packer.nvim"

local plug_url_format = ""
-- if vim.g.is_linux then
--  plug_url_format = "https://hub.fastgit.org/%s"
-- else
plug_url_format = "https://github.com/%s"
-- end

local packer_repo = string.format(plug_url_format, "wbthomason/packer.nvim")
local install_cmd = string.format("10split |term git clone --depth=1 %s %s",
                                  packer_repo, packer_install_dir)

-- Auto-install packer in case it hasn't been installed.
if fn.glob(packer_install_dir) == "" then
    vim.api.nvim_echo({{"Installing packer.nvim", "Type"}}, true, {})
    vim.cmd(install_cmd)
end

-- Load packer.nvim
vim.cmd("packadd packer.nvim")
local util = require('packer.util')

require("packer").startup({
    function(use)
        use({"wbthomason/packer.nvim", opt = true})

        -- "Plug 'morhetz/gruvbox'
        -- Plug 'sainnhe/gruvbox-material'
        -- "Plug 'dracula/vim', { 'as': 'dracula' }
        -- " Plug 'ajmwagar/vim-deus'

        use "mhinz/vim-startify"
        -- use 'luochen1990/rainbow'
        -- use 'kien/rainbow_parentheses.vim'

        -- tabline
        -- use {
        -- 'nvim-lualine/lualine.nvim',
        -- event = 'VimEnter',
        -- requires = {'kyazdani42/nvim-web-devicons', opt = true}
        -- }
        -- use({
        -- "akinsho/bufferline.nvim",
        -- event = "VimEnter",
        -- after = 'lualine.nvim',
        -- config = [[require('config.line')]]
        -- })
        use "vim-airline/vim-airline"
        use "vim-airline/vim-airline-themes" -- 综合图标支持such vim-airline lightline, vim-startify
        use "ryanoasis/vim-devicons"

        use "sainnhe/gruvbox-material"

        -- look!!!

        -- " File navigation
        -- Plug 'preservim/nerdtree', {'on': 'NERDTreeToggle'}
        -- Plug 'tiagofumo/vim-nerdtree-syntax-highlight'

        -- use {
        -- 'nvim-telescope/telescope.nvim',
        -- requires = {{'nvim-lua/plenary.nvim'}}
        -- }
        ---- search emoji and other symbols
        -- use 'nvim-telescope/telescope-symbols.nvim'

        -- " register
        -- Plug 'junegunn/vim-peekaboo'
        -- use({"jdhao/better-escape.vim", event = {"InsertEnter"}})

        -- " Undo Tree 修改历史
        -- "noremap <f5> :GundoToggle<cr>
        -- " Plug 'sjl/gundo.vim'
        -- "Plug 'mbbill/undotree'

        -- " mark
        use 'kshenoy/vim-signature'
        -- " Bookmarks
        -- " Plug 'MattesGroeger/vim-bookmarks'

        -- Clear highlight search automatically for you
        use({"romainl/vim-cool", event = "VimEnter"})
        -- Highlight URLs inside vim
        use({"itchyny/vim-highlighturl", event = "VimEnter"})
        -- " see
        use 'Yggdroot/indentLine'
        -- " 相同字符串下划线
        use 'itchyny/vim-cursorword'
        -- Automatic insertion and deletion of a pair of characters
        use({"Raimondi/delimitMate", event = "InsertEnter"})
        -- Show match number for search
        use {'kevinhwang91/nvim-hlslens', branch = 'dev', event = "VimEnter"}

        -- type!!!

        -- "Plug 'rlue/vim-barbaric' " change input?
        -- " Plug 'lyokha/vim-xkbswitch' let g:XkbSwitchEnabled = 1
        -- "Plug 'lilydjwg/fcitx.vim'
        -- " Find & Replace
        -- "Plug 'brooth/far.vim', { 'on': ['F', 'Far', 'Fardo'] }
        -- " search fzf ack
        -- " <leader><leader>+
        use 'easymotion/vim-easymotion'

        -- code !!!

        use 'dense-analysis/ale'

        -- auto-completion engine
        use {
            "hrsh7th/nvim-cmp",
            after = "lspkind-nvim",
            config = [[require('config.cmp')]]
        }
        use {"hrsh7th/cmp-nvim-lsp", after = "nvim-cmp"}
        use({ -- lspconfig
            "neovim/nvim-lspconfig",
            after = "cmp-nvim-lsp"
        })
        use {
            'williamboman/nvim-lsp-installer',
            after = "nvim-lspconfig",
            config = [[require('config.lsp')]]
        }
        -- use {'glepnir/lspsaga.nvim', config = [[require('config.lspsaga')]]}
        -- Plug 'nvim-lua/lsp-status.nvim'

        use {"onsails/lspkind-nvim", event = 'BufEnter'} -- 美化自动完成提示信息
        use {"hrsh7th/cmp-nvim-lua", after = "nvim-cmp"} -- nvim-cmp source for neovim Lua API.
        use {"hrsh7th/cmp-path", after = "nvim-cmp"} -- 自动提示硬盘上的文件
        use {"hrsh7th/cmp-buffer", after = "nvim-cmp"} -- 从buffer中智能提示
        -- use {"hrsh7th/cmp-cmdline", after = "nvim-cmp"}
        -- use {"hrsh7th/cmp-emoji", after = 'nvim-cmp'} -- 输入: 可以显示表情
        use {"octaltree/cmp-look", after = 'nvim-cmp'} -- 用于完成英语单词
        -- use {"hrsh7th/cmp-calc", after = 'nvim-cmp'} -- 输入数学算式（如1+1=）自动计算
        use {"f3fora/cmp-spell", after = 'nvim-cmp'} -- nvim-cmp 的拼写源基于 vim 的拼写建议

        -- notification plugin
        -- use({
        -- "rcarriga/nvim-notify",
        -- event = "BufEnter",
        -- config = [[require('config.nvim-notify')]]
        -- })

        -- "" Treesitter
        use({
            "nvim-treesitter/nvim-treesitter",
            event = 'BufEnter',
            run = ":TSUpdate",
            config = [[require('config.treesitter')]]
        })

        -- " post install (yarn install | npm install) then load plugin only for editing supported files
        use {
            'prettier/vim-prettier',
            run = 'yarn install',
            ft = {
                'javascript', 'typescript', 'css', 'less', 'scss', 'graphql',
                'markdown', 'vue', 'html'
            }
        }
        -- use {
        -- 'prettier/vim-prettier',
        -- run = "yarn install --frozen-lockfile --production"
        -- }
        --
        -- Auto format tools
        use({"sbdchd/neoformat", cmd = {"Neoformat"}})

        -- Comment plugin
        -- use({"tpope/vim-commentary", event = "VimEnter"})
        -- " in <space>cc to comment a line
        use 'scrooloose/nerdcommenter'

        use({'dhruvasagar/vim-table-mode', ft = {'markdown', 'vim-plug'}})
        -- Another markdown plugin
        -- "Plug 'godlygeek/tabular' "必要插件，安装在vim-markdown前面
        -- use({"plasticboy/vim-markdown", ft = {"markdown"}})

        -- Faster footnote generation
        -- use({"vim-pandoc/vim-markdownfootnotes", ft = {"markdown"}})

        -- Plug 'neoclide/jsonc.vim'
        -- Plug 'othree/html5.vim'
        -- Plug 'pangloss/vim-javascript', { 'for': ['vim-plug', 'php', 'html', 'javascript', 'css', 'less'] }
        -- Plug 'jelera/vim-javascript-syntax', { 'for': ['vim-plug', 'php', 'html', 'javascript', 'css', 'less'] }
        -- Markdown JSON header highlight plugin
        -- use({"elzr/vim-json", ft = {"json", "markdown"}})

        use({
            "iamcco/markdown-preview.nvim",
            run = function() fn["mkdp#util#install"]() end,
            ft = {"markdown"}
        })

        -- Additional powerful text object for vim, this plugin should be studied
        -- carefully to use its full power
        use({"wellle/targets.vim", event = "VimEnter"})

        -- Plugin to manipulate character pairs quickly
        -- use 'tpope/vim-surround'
        use({"machakann/vim-sandwich", event = "VimEnter"})

        -- .tmux.conf syntax highlighting and setting check
        use({"tmux-plugins/vim-tmux", ft = {"tmux"}})

        -- Asynchronous command execution
        use({"skywind3000/asyncrun.vim", opt = true, cmd = {"AsyncRun"}})

        -- use({"cespare/vim-toml", ft = {"toml"}, branch = "main"})

        -- Debugger plugin
        -- if vim.g.is_win or vim.g.is_linux then
        -- use({
        -- "sakhnik/nvim-gdb",
        -- run = {"bash install.sh"},
        -- opt = true,
        -- setup = [[vim.cmd('packadd nvim-gdb')]]
        -- })
        -- end

        -- The missing auto-completion for cmdline!
        use({
            "gelguy/wilder.nvim",
            opt = true,
            setup = [[vim.cmd('packadd wilder.nvim')]]
        })

        use({"lervag/vimtex", ft = {"tex"}})
        -- Plug 'xuhdev/vim-latex-live-preview', { 'for': 'tex' }

    end,
    config = {
        max_jobs = 16,
        compile_path = util.join_paths(vim.fn.stdpath('config'), 'lua',
                                       'packer_compiled.lua'),
        git = {default_url_format = plug_url_format}
    }
})

local status, _ = pcall(require, 'packer_compiled')
if not status then
    vim.notify("Error requiring packer_compiled.lua: run PackerSync to fix!")
end

