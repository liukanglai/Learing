--[[
lvim is the global options object
Linters should be filled in as strings with either a global executable or a path to an executable
]]

-- general
lvim.log.level = "warn"
lvim.format_on_save.enabled = true
lvim.colorscheme = "lunar"
-- lvim.builtin.lualine.options.theme = "gruvbox"
-- lvim.transparent_window = true
-- to disable icons and use a minimalist setup, uncomment the following
-- lvim.use_icons = false

-- keymappings [view all the defaults by pressing <leader>Lk, or :map(nmap, vmap, imap)]
-- To see if a particular key has already been bound :verbose map <TAB>
lvim.leader = "space"
-- add your own keymapping
lvim.keys.normal_mode["<leader>0"] = "$" --leaderq???

lvim.keys.normal_mode["<C-s>"] = ":w<cr>"
lvim.keys.normal_mode["j"] = "<Plug>(accelerated_jk_gj)"
lvim.keys.normal_mode["k"] = "<Plug>(accelerated_jk_gk)"
lvim.keys.normal_mode["<S-l>"] = ":BufferLineCycleNext<CR>"
lvim.keys.normal_mode["<S-h>"] = ":BufferLineCyclePrev<CR>"

lvim.keys.visual_mode["t"] = ":TranslateW<CR>"
-- vim.keymap.del("n", "<C-Up>")
-- override a default keymapping
-- lvim.keys.normal_mode["<C-q>"] = ":q<cr>" -- or vim.keymap.set("n", "<C-q>", ":q<cr>" )
-- To modify LSP keybindings
-- add your own keymapping
-- lvim.lsp.buffer_mappings.normal_mode['<leader>lh'] = { vim.lsp.buf.hover, "Show documentation" }
-- r map default functionality to a different key
-- lvim.lsp.buffer_mappings.normal_mode['gk'] = lvim.lsp.buffer_mappings.normal_mode['K']
-- Remove a binding(LSP bindings take precedence over regular keybindings)
-- lvim.lsp.buffer_mappings.normal_mode['K'] = nil
-- lvim.keys.normal_mode['K'] = "<Cmd>echo Okay!<CR>"


-- Change Telescope navigation to use j and k for navigation and n and p for history in both input and normal mode.
-- we use protected-mode (pcall) just in case the plugin wasn't loaded yet.
-- local _, actions = pcall(require, "telescope.actions")
-- lvim.builtin.telescope.defaults.mappings = {
--   -- for input mode
--   i = {
--     ["<C-j>"] = actions.move_selection_next,
--     ["<C-k>"] = actions.move_selection_previous,
--     ["<C-n>"] = actions.cycle_history_next,
--     ["<C-p>"] = actions.cycle_history_prev,
--   },
--   -- for normal mode
--   n = {
--     ["<C-j>"] = actions.move_selection_next,
--     ["<C-k>"] = actions.move_selection_previous,
--   },
-- }
lvim.builtin.telescope.on_config_done = function(telescope)
  -- pcall(telescope.load_extension, "neoclip")
  pcall(telescope.load_extension, "notify")
  pcall(telescope.load_extension, "fzy")
  pcall(telescope.load_extension, "projects")
  -- pcall(telescope.load_extension, "undo")
  -- any other extensions loading
end

-- Change theme settings
-- lvim.builtin.theme.options.dim_inactive = true
-- lvim.builtin.theme.options.style = "storm"

-- Use which-key to add extra bindings with the leader-key prefix
-- lvim.builtin.which_key.mappings["P"] = { "<cmd>Telescope projects<CR>", "Projects" }
lvim.builtin.which_key.mappings["m"] = {
  name = "tool+",
  m = { "<cmd>MarkdownPreview<cr>", "markdown-preview" },
  a = { "<cmd>LLPStartPreview<cr>", "latex-preview" },
  -- t = { "viw:Translate ZH<cr>", "latex-preview" },
  -- t = { "<cmd>Translate ZH<cr>", "latex-preview" },
  t = { "<cmd>TranslateW<cr>", "translate word" }, -- C-w +p to jump between windows.
  l = { "<cmd>1TranslateW<cr>", "translate line" },
}
lvim.builtin.which_key.mappings["t"] = {
  name = "Diagnostics",
  t = { "<cmd>TroubleToggle<cr>", "trouble" },
  w = { "<cmd>TroubleToggle workspace_diagnostics<cr>", "workspace" },
  d = { "<cmd>TroubleToggle document_diagnostics<cr>", "document" },
  q = { "<cmd>TroubleToggle quickfix<cr>", "quickfix" },
  l = { "<cmd>TroubleToggle loclist<cr>", "loclist" },
  r = { "<cmd>TroubleToggle lsp_references<cr>", "references" },
  f = { "<cmd>Trouble lsp_definitions<cr>", "Definitions" },
}

lvim.builtin.which_key.mappings.l = vim.tbl_extend("keep", lvim.builtin.which_key.mappings.l,
  { o = { "<cmd>SymbolsOutline<cr>", "Open Outline" } })
lvim.builtin.which_key.mappings.s = vim.tbl_extend("keep", lvim.builtin.which_key.mappings.s,
  { n = { "<cmd>Telescope notify<cr>", "Notify" } })
-- --Plugin Legendary
-- "n|<C-p>" = map_cr("Legendary"):with_silent():with_noremap(),
-- -- Plugin SnipRun
-- 	["v|<leader>r"] = map_cr("SnipRun"):with_noremap():with_silent(),
-- 	["n|<leader>r"] = map_cu([[%SnipRun]]):with_noremap():with_silent(),


-- TODO: User Config for predefined plugins
-- After changing plugin config exit and reopen LunarVim, Run :PackerInstall :PackerCompile
lvim.builtin.alpha.active = true
lvim.builtin.alpha.mode = "dashboard"
lvim.builtin.terminal.active = true
lvim.builtin.nvimtree.setup.view.side = "left"
lvim.builtin.nvimtree.setup.renderer.icons.show.git = false

-- if you don't want all the parsers change this to a table of the ones you want
lvim.builtin.treesitter.ensure_installed = {
  "bash",
  "vim",
  "lua",
  "c",
  "cpp",
  "make",
  "cmake",
  "java",
  "python",
  "scheme",
  "javascript",
  "json",
  "jsonc",
  "typescript",
  "tsx",
  "css",
  "yaml",
  "html",
  "rust",
  "go",
  "gomod",
  "latex",
  "markdown",
  "regex",
}

lvim.builtin.treesitter.ignore_install = { "haskell" }
lvim.builtin.treesitter.highlight.enable = true
lvim.builtin.treesitter.rainbow.enable = true

-- generic LSP settings
-- make sure server will always be installed even if the server is in skipped_servers list
lvim.lsp.installer.setup.ensure_installed = {
  -- "jsonls",
}
-- -- change UI setting of `LspInstallInfo`
-- -- see <https://github.com/williamboman/nvim-lsp-installer#default-configuration>
-- lvim.lsp.installer.setup.ui.check_outdated_servers_on_open = false
-- lvim.lsp.installer.setup.ui.border = "rounded"
-- lvim.lsp.installer.setup.ui.keymaps = {
--     uninstall_server = "d",
--     toggle_server_expand = "o",
-- }

-- ---@usage disable automatic installation of servers
-- lvim.lsp.installer.setup.automatic_installation = false

-- ---configure a server manually. !!Requires `:LvimCacheReset` to take effect!!
-- ---see the full default list `:lua print(vim.inspect(lvim.lsp.automatic_configuration.skipped_servers))`
vim.list_extend(lvim.lsp.automatic_configuration.skipped_servers, { "clangd" })
local capabilities = require("lvim.lsp").common_capabilities()
capabilities.offsetEncoding = { "utf-16" }
local opts = { capabilities = capabilities }
require("lvim.lsp.manager").setup("clangd", opts)
-- ---The default file:
-- lvim.lsp.templates_dir = join_paths(get_runtime_dir(), "after", "ftplugin")
-- ---edit this file by running `:lua vim.cmd("edit " .. lvim.lsp.templates_dir .. "/lua.lua")`
-- require("lvim.lsp.manager").setup("sumneko_lua")

-- ---remove a server from the skipped list, e.g. eslint, or emmet_ls. !!Requires `:LvimCacheReset` to take effect!!
-- ---`:LvimInfo` lists which server(s) are skipped for the current filetype
-- lvim.lsp.automatic_configuration.skipped_servers = vim.tbl_filter(function(server)
--   return server ~= "emmet_ls"
-- end, lvim.lsp.automatic_configuration.skipped_servers)

-- -- you can set a custom on_attach function that will be used for all the language servers
-- -- See <https://github.com/neovim/nvim-lspconfig#keybindings-and-completion>
-- lvim.lsp.on_attach_callback = function(client, bufnr)
--   local function buf_set_option(...)
--     vim.api.nvim_buf_set_option(bufnr, ...)
--   end
--   --Enable completion triggered by <c-x><c-o>
--   buf_set_option("omnifunc", "v:lua.vim.lsp.omnifunc")
-- end

-- Can not be placed into the config method of the plugins.
-- lvim.builtin.cmp.formatting.source_names["copilot"] = "(Copilot)"
table.insert(lvim.builtin.cmp.sources, 1, { name = "nvim_lua" })
table.insert(lvim.builtin.cmp.sources, 2, { name = "spell" })
table.insert(lvim.builtin.cmp.sources, 3, { name = "tmux" })
table.insert(lvim.builtin.cmp.sources, 4, { name = "latex_symbols" })

-- Lazy-loading the linter/formatter/code_actions setup???
-- -- set a formatter, this will override the language server formatting capabilities (if it exists)
local formatters = require "lvim.lsp.null-ls.formatters"
formatters.setup {
  -- { command = "black", filetypes = { "python" } },
  -- { command = "isort", filetypes = { "python" } },
  { command = "stylua", filetypes = { "lua" } },
  { command = "shfmt", filetypes = { "shell" } },
  {
    -- each formatter accepts a list of options identical to https://github.com/jose-elias-alvarez/null-ls.nvim/blob/main/doc/BUILTINS.md#Configuration
    command = "prettier",
    ---@usage arguments to pass to the formatter
    -- these cannot contain whitespaces, options such as `--line-width 80` become either `{'--line-width', '80'}` or `{'--line-width=80'}`
    extra_args = { "--print-with", "100" },
    ---@usage specify which filetypes to enable. By default a providers will attach to all the filetypes it supports.
    -- filetypes = { "markdown", "typescript", "typescriptreact" },
  },
}

-- -- set additional linters
local linters = require "lvim.lsp.null-ls.linters"
linters.setup {
  {
    -- each linter accepts a list of options identical to https://github.com/jose-elias-alvarez/null-ls.nvim/blob/main/doc/BUILTINS.md#Configuration
    command = "shellcheck",
    ---@usage arguments to pass to the formatter
    -- these cannot contain whitespaces, options such as `--line-width 80` become either `{'--line-width', '80'}` or `{'--line-width=80'}`
    extra_args = { "--severity", "warning" },
  },
  { command = "codespell", filetypes = { "javascript", "python" }, },
  { command = "cpplint", filetype = { "c", "cpp" }, -- extra_args = { "--style", "{IndentWidth: 4}" }
  }, -- clangtidy is better.
  { command = "cppcheck", filetype = { "c", "cpp" }, },
  { command = "checkstyle", filetype = { "java" }, },
  { command = "pylint", filetype = { "python" }, },
  -- { command = "flake8", filetypes = { "python" } },
  -- { command = "golangcilint", filetypes = { "go" }, },
  { command = "eslint", filetypes = { "typescript", "javascript" }, },
  { command = "stylelint", filetype = { "css" }, },
  { command = "tidy", filetypes = { "html" }, },
  { command = "vint", filetypes = { "vim" }, },
  -- { command = "luacheck", filetypes = { "lua" }, },
  { command = "markdownlint", filetypes = { "markdown" }, },
}

-- -- set additional code_actions
local code_actions = require "lvim.lsp.null-ls.code_actions"
code_actions.setup {
  { command = "proselint" },
}

-- Additional Plugins
lvim.plugins = {
  -- navigation
  { "phaazon/hop.nvim", -- neovim motions on speed!
    branch = 'v2',
    event = "BufRead",
    config = function()
      require("hop").setup({ keys = "etovxqpdygfblzhckisuran" })
      vim.api.nvim_set_keymap("n", "w", ":HopWord<cr>", { silent = true })
      vim.api.nvim_set_keymap("n", "<leader>j", ":HopLine<cr>", { silent = true })
    end,
  },
  { "windwp/nvim-spectre", -- search and replace ???
    event = "BufRead",
    config = function()
      require("spectre").setup()
    end,
  },
  { "rainbowhxch/accelerated-jk.nvim",
    config = function()
      require("accelerated-jk").setup()
    end,
  },
  { "ethanholz/nvim-lastplace",
    event = "BufRead",
    config = function()
      require("nvim-lastplace").setup({
        lastplace_ignore_buftype = { "quickfix", "nofile", "help" },
        lastplace_ignore_filetype = {
          "gitcommit", "gitrebase", "svn", "hgcommit",
        },
        lastplace_open_folds = true,
      })
    end,
  },
  { "romainl/vim-cool", -- Auto clear highlight after search
    event = { "CursorMoved", "InsertEnter" },
  },
  { "edluffy/specs.nvim",
    event = "CursorMoved",
    config = function()
      require("specs").setup({
        show_jumps = true,
        min_jump = 10,
        popup = {
          delay_ms = 0, -- delay before popup displays
          inc_ms = 10, -- time increments used for fade/resize effects
          blend = 10, -- starting blend, between 0-100 (fully transparent), see :h winblend
          width = 10,
          winhl = "PMenu",
          fader = require("specs").pulse_fader,
          resizer = require("specs").shrink_resizer,
        },
        ignore_filetypes = {},
        ignore_buftypes = { nofile = true },
      })
    end,
  },
  --  Hint cursor place when jump multiple lines

  -- treesitter
  { "windwp/nvim-ts-autotag",
    config = function()
      require("nvim-ts-autotag").setup({
        filetypes = {
          "html",
          "xml",
          "javascript",
          "typescriptreact",
          "javascriptreact",
          "vue",
        },
      })
    end,
  },
  { "andymass/vim-matchup",
    event = "CursorMoved",
    config = function()
      vim.g.matchup_matchparen_offscreen = { method = "popup" }
    end,
  },
  { "p00f/nvim-ts-rainbow", },
  { "nvim-treesitter/playground",
    event = "BufRead",
  },
  { "romgrk/nvim-treesitter-context",
    config = function()
      require("treesitter-context").setup {
        enable = true, -- Enable this plugin (Can be enabled/disabled later via commands)
        throttle = true, -- Throttles plugin updates (may improve performance)
        max_lines = 0, -- How many lines the window should span. Values <= 0 mean no limit.
        patterns = { -- Match patterns for TS nodes. These get wrapped to match at word boundaries.
          -- For all filetypes
          -- Note that setting an entry here replaces all other patterns for this entry.
          -- By setting the 'default' entry below, you can control which nodes you want to
          -- appear in the context window.
          default = {
            'class',
            'function',
            'method',
          },
        },
      }
    end
  },

  -- telescope
  { "nvim-telescope/telescope-fzy-native.nvim",
    run = "make",
    event = "BufRead",
  },
  { "nvim-telescope/telescope-project.nvim",
    event = "BufWinEnter",
    setup = function()
      vim.cmd [[packadd telescope.nvim]]
    end,
  },

  -- lsp enhancement
  { "folke/lsp-colors.nvim", --lsp diagnostics highlight groups for non lsp colorschemes
    event = "BufRead",
  },
  { "norcalli/nvim-colorizer.lua", -- Display detected color
    config = function()
      require("colorizer").setup({ "css", "scss", "html", "javascript" }, {
        RGB = true, -- #RGB hex codes
        RRGGBB = true, -- #RRGGBB hex codes
        RRGGBBAA = true, -- #RRGGBBAA hex codes
        rgb_fn = true, -- CSS rgb() and rgba() functions
        hsl_fn = true, -- CSS hsl() and hsla() functions
        css = true, -- Enable all CSS features: rgb_fn, hsl_fn, names, RGB, RRGGBB
        css_fn = true, -- Enable all CSS *functions*: rgb_fn, hsl_fn
      })
    end,
  },
  { "rmagatti/goto-preview", -- previewing goto definition calls
    config = function()
      require('goto-preview').setup {
        width = 120; -- Width of the floating window
        height = 25; -- Height of the floating window
        default_mappings = false; -- Bind default mappings
        debug = false; -- Print debug information}
        opacity = nil; -- 0-100 opacity level of the floating window where 100 is fully transparent.
        post_open_hook = nil; -- A function taking two arguments, a buffer and a window to be ran as a hook.
        -- You can use "default_mappings = true" setup option
        -- Or explicitly set keybindings
        vim.cmd("nnoremap gpd <cmd>lua require('goto-preview').goto_preview_definition()<CR>");
        vim.cmd("nnoremap gpi <cmd>lua require('goto-preview').goto_preview_implementation()<CR>");
        vim.cmd("nnoremap gP <cmd>lua require('goto-preview').close_all_win()<CR>")
      }
    end
  },
  { "tzachar/cmp-tabnine", -- TabNine completion engine for hrsh7th/nvim-cmp
    run = "./install.sh",
    requires = "hrsh7th/nvim-cmp",
    event = "InsertEnter",
    require("cmp_tabnine.config"):setup({ max_line = 1000, max_num_results = 20, sort = true }),
  },
  { "ray-x/lsp_signature.nvim", -- hint when you type, 小熊猫
    event = "BufRead",
    config = function() require "lsp_signature".on_attach() end,
  },
  -- { "lukas-reineke/cmp-under-comparator", module = "cmp-under-comparator" },
  { "hrsh7th/cmp-nvim-lua", after = "cmp-nvim-lsp" },
  { "andersevenrud/cmp-tmux", after = "cmp-nvim-lua" },
  { "f3fora/cmp-spell", after = "cmp-path" },
  { "kdheepak/cmp-latex-symbols", after = "cmp-buffer" },
  { "simrat39/symbols-outline.nvim", -- a tree like view for symbols: <leader>lo
    config = function()
      require('symbols-outline').setup()
    end
  },
  { "folke/trouble.nvim", -- diagnostics, references, telescope results, quickfix and location lists
    cmd = "TroubleToggle",
  },
  { "michaelb/sniprun", -- ???
    opt = true,
    run = "bash ./install.sh",
    cmd = { "SnipRun", "'<,'>SnipRun" },
  },
  { "gelguy/wilder.nvim", -- More useful wild menu
    event = "CmdlineEnter",
    config = function()
      local wilder = require("wilder")

      wilder.setup({ modes = { ":", "/", "?" } })
      wilder.set_option("use_python_remote_plugin", 0)
      wilder.set_option("pipeline", {
        wilder.branch(
          wilder.cmdline_pipeline({ use_python = 0, fuzzy = 1, fuzzy_filter = wilder.lua_fzy_filter() }),
          wilder.vim_search_pipeline(),
          {
            wilder.check(function(_, x)
              return x == ""
            end),
            wilder.history(),
            wilder.result({
              draw = {
                function(_, x)
                  return "" .. " " .. x
                end,
              },
            }),
          }
        ),
      })

      local popupmenu_renderer = wilder.popupmenu_renderer(wilder.popupmenu_border_theme({
        border = "rounded",
        highlights = {
          border = "Title", -- highlight to use for the border
        },
        empty_message = wilder.popupmenu_empty_message_with_spinner(),
        highlighter = wilder.lua_fzy_highlighter(),
        left = {
          " ",
          wilder.popupmenu_devicons(),
          wilder.popupmenu_buffer_flags({
            flags = " a + ",
            icons = { ["+"] = "", a = "", h = "" },
          }),
        },
        right = {
          " ",
          wilder.popupmenu_scrollbar(),
        },
      }))
      local wildmenu_renderer = wilder.wildmenu_renderer({
        highlighter = wilder.lua_fzy_highlighter(),
        apply_incsearch_fix = true,
      })
      wilder.set_option(
        "renderer",
        wilder.renderer_mux({
          [":"] = popupmenu_renderer,
          ["/"] = wildmenu_renderer,
          substitute = wildmenu_renderer,
        })
      )
    end,
    requires = { { "romgrk/fzy-lua-native", after = "wilder.nvim" } },
  },

  -- General
  { "metakirby5/codi.vim", -- interactive scratchpad for hackers ???
    cmd = "Codi",
  },
  { "tpope/vim-repeat" }, -- enable repeating supported plugin maps with "."
  { "wakatime/vim-wakatime" }, -- :WakaTimeApiKey, :WakaTimeToday
  { "rcarriga/nvim-notify", --???
    config = function()
      local notify = require("notify")
      notify.setup({
        ---@usage Animation style one of { "fade", "slide", "fade_in_slide_out", "static" }
        stages = "slide",
        ---@usage Function called when a new window is opened, use for changing win settings/config
        on_open = nil,
        ---@usage Function called when a window is closed
        on_close = nil,
        ---@usage timeout for notifications in ms, default 5000
        timeout = 2000,
        -- @usage User render fps value
        fps = 30,
        -- Render function for notifications. See notify-render()
        render = "default",
        ---@usage highlight behind the window for stages that change opacity
        background_colour = "Normal",
        ---@usage minimum width for notification windows
        minimum_width = 50,
        ---@usage notifications with level lower than this would be ignored. [ERROR > WARN > INFO > DEBUG > TRACE]
        level = "TRACE",
      })

      vim.notify = notify
    end
  },
  { "j-hui/fidget.nvim", -- show plugin load
    event = "BufReadPost",
    config = function()
      require("fidget").setup({
        window = { blend = 0 },
      })
    end
  },


  -- tool
  { "iamcco/markdown-preview.nvim",
    run = "cd app && npm install",
    ft = "markdown",
    config = function()
      vim.g.mkdp_auto_start = 1
    end
  },
  { "turbio/bracey.vim", -- live edit html, css, and javascript
    cmd = { "Bracey", "BracyStop", "BraceyReload", "BraceyEval" },
    run = "npm install --prefix server",
  },
  { "chrisbra/csv.vim", opt = true, ft = "csv", },
  { "fatih/vim-go",
    opt = true,
    ft = "go",
    run = ":GoInstallBinaries",
    config = function()
      vim.g.go_doc_keywordprg_enabled = 0
      vim.g.go_def_mapping_enabled = 0
      vim.g.go_code_completion_enabled = 0
    end
  },
  { "simrat39/rust-tools.nvim",
    opt = true,
    ft = "rust",
    config = function()
      vim.api.nvim_command([[packadd nvim-lspconfig]])
      vim.api.nvim_command([[packadd lsp_signature.nvim]])

      local opts = {
        tools = { -- rust-tools options

          -- how to execute terminal commands
          -- options right now: termopen / quickfix
          executor = require("rust-tools/executors").termopen,

          -- callback to execute once rust-analyzer is done initializing the workspace
          -- The callback receives one parameter indicating the `health` of the server: "ok" | "warning" | "error"
          on_initialized = function()
            require("lsp_signature").on_attach({
              bind = true,
              use_lspsaga = false,
              floating_window = true,
              fix_pos = true,
              hint_enable = true,
              hi_parameter = "Search",
              handler_opts = {
                border = "rounded",
              },
            })
          end,

          -- automatically call RustReloadWorkspace when writing to a Cargo.toml file.
          reload_workspace_from_cargo_toml = true,

          -- These apply to the default RustSetInlayHints command
          inlay_hints = {
            -- automatically set inlay hints (type hints)
            -- default: true
            auto = true,

            -- Only show inlay hints for the current line
            only_current_line = false,

            -- whether to show parameter hints with the inlay hints or not
            -- default: true
            show_parameter_hints = true,

            -- prefix for parameter hints
            -- default: "<-"
            parameter_hints_prefix = "<- ",

            -- prefix for all the other hints (type, chaining)
            -- default: "=>"
            other_hints_prefix = "=> ",

            -- whether to align to the lenght of the longest line in the file
            max_len_align = false,

            -- padding from the left if max_len_align is true
            max_len_align_padding = 1,

            -- whether to align to the extreme right or not
            right_align = false,

            -- padding from the right if right_align is true
            right_align_padding = 7,

            -- The color of the hints
            highlight = "Comment",
          },

          -- options same as lsp hover / vim.lsp.util.open_floating_preview()
          hover_actions = {

            -- the border that is used for the hover window
            -- see vim.api.nvim_open_win()
            border = {
              { "╭", "FloatBorder" },
              { "─", "FloatBorder" },
              { "╮", "FloatBorder" },
              { "│", "FloatBorder" },
              { "╯", "FloatBorder" },
              { "─", "FloatBorder" },
              { "╰", "FloatBorder" },
              { "│", "FloatBorder" },
            },

            -- whether the hover action window gets automatically focused
            -- default: false
            auto_focus = false,
          },

          -- settings for showing the crate graph based on graphviz and the dot
          -- command
          crate_graph = {
            -- Backend used for displaying the graph
            -- see: https://graphviz.org/docs/outputs/
            -- default: x11
            backend = "x11",
            -- where to store the output, nil for no output stored (relative
            -- path from pwd)
            -- default: nil
            output = nil,
            -- true for all crates.io and external crates, false only the local
            -- crates
            -- default: true
            full = true,

            -- List of backends found on: https://graphviz.org/docs/outputs/
            -- Is used for input validation and autocompletion
            -- Last updated: 2021-08-26
            enabled_graphviz_backends = {
              "bmp",
              "cgimage",
              "canon",
              "dot",
              "gv",
              "xdot",
              "xdot1.2",
              "xdot1.4",
              "eps",
              "exr",
              "fig",
              "gd",
              "gd2",
              "gif",
              "gtk",
              "ico",
              "cmap",
              "ismap",
              "imap",
              "cmapx",
              "imap_np",
              "cmapx_np",
              "jpg",
              "jpeg",
              "jpe",
              "jp2",
              "json",
              "json0",
              "dot_json",
              "xdot_json",
              "pdf",
              "pic",
              "pct",
              "pict",
              "plain",
              "plain-ext",
              "png",
              "pov",
              "ps",
              "ps2",
              "psd",
              "sgi",
              "svg",
              "svgz",
              "tga",
              "tiff",
              "tif",
              "tk",
              "vml",
              "vmlz",
              "wbmp",
              "webp",
              "xlib",
              "x11",
            },
          },
        },

        -- all the opts to send to nvim-lspconfig
        -- these override the defaults set by rust-tools.nvim
        -- see https://github.com/neovim/nvim-lspconfig/blob/master/doc/server_configurations.md#rust_analyzer
        server = {
          -- standalone file support
          -- setting it to false may improve startup time
          standalone = true,
        }, -- rust-analyer options

        -- debugging stuff
        dap = {
          adapter = {
            type = "executable",
            command = "lldb-vscode",
            name = "rt_lldb",
          },
        },
      }

      require("rust-tools").setup(opts)
    end,
    requires = "nvim-lua/plenary.nvim",
  },
  -- { "uga-rosa/translate.nvim", },
  { "voldikss/vim-translator", },
}

-- Autocommands (https://neovim.io/doc/user/autocmd.html)
-- vim.api.nvim_create_autocmd("BufEnter", {
--   pattern = { "*.json", "*.jsonc" },
--   -- enable wrap mode for json files only
--   command = "setlocal wrap",
-- })
-- vim.api.nvim_create_autocmd("FileType", {
--   pattern = "zsh",
--   callback = function()
--     -- let treesitter use bash highlight for zsh files as well
--     require("nvim-treesitter.highlight").attach(0, "bash")
--   end,
-- })
vim.opt.backup = false -- creates a backup file
vim.opt.clipboard = "unnamedplus" -- allows neovim to access the system clipboard
vim.opt.cmdheight = 2 -- more space in the neovim command line for displaying messages
vim.opt.colorcolumn = "99999" -- fixes indentline for now
vim.opt.completeopt = { "menuone", "noselect" }
vim.opt.conceallevel = 0 -- so that `` is visible in markdown files
vim.opt.fileencoding = "utf-8" -- the encoding written to a file
vim.opt.foldmethod = "manual" -- folding set to "expr" for treesitter based folding
vim.opt.foldexpr = "" -- set to "nvim_treesitter#foldexpr()" for treesitter based folding
vim.opt.guifont = "monospace:h17" -- the font used in graphical neovim applications
vim.opt.hidden = true -- required to keep multiple buffers and open multiple buffers
vim.opt.hlsearch = true -- highlight all matches on previous search pattern
vim.opt.ignorecase = true -- ignore case in search patterns
vim.opt.mouse = "a" -- allow the mouse to be used in neovim
vim.opt.pumheight = 10 -- pop up menu height
vim.opt.showmode = false -- we don't need to see things like -- INSERT -- anymore
vim.opt.showtabline = 2 -- always show tabs
vim.opt.smartcase = true -- smart case
vim.opt.smartindent = true -- make indenting smarter again
vim.opt.splitbelow = true -- force all horizontal splits to go below current window
vim.opt.splitright = true -- force all vertical splits to go to the right of current window
vim.opt.swapfile = false -- creates a swapfile
vim.opt.termguicolors = true -- set term gui colors (most terminals support this)
vim.opt.title = true -- set the title of window to the value of the titlestring
vim.opt.titlestring = "%<%F%=%l/%L - nvim" -- what the title of the window will be set to
vim.opt.undodir = vim.fn.stdpath "cache" .. "/undo"
vim.opt.undofile = true -- enable persistent undo
vim.opt.writebackup = false -- if a file is being edited by another program (or was written to file while editing with another program) it is not allowed to be edited
vim.opt.expandtab = true -- convert tabs to spaces
vim.opt.shiftwidth = 2 -- the number of spaces inserted for each indentation
vim.opt.tabstop = 2 -- insert 2 spaces for a tab
vim.opt.cursorline = true -- highlight the current line
vim.opt.number = true -- set numbered lines
vim.opt.relativenumber = true -- set relative numbered lines
vim.opt.numberwidth = 4 -- set number column width to 2 {default 4}
vim.opt.signcolumn = "yes" -- always show the sign column otherwise it would shift the text each time
vim.opt.wrap = true -- display lines as one long line
vim.opt.spell = false
vim.opt.spelllang = "en"
vim.opt.scrolloff = 5 -- is one of my fav
vim.opt.sidescrolloff = 5

vim.opt.errorbells = true
vim.opt.visualbell = true
vim.opt.fileformats = "unix,mac,dos"
vim.opt.magic = true
vim.opt.virtualedit = "block"
vim.opt.wildignorecase = true
vim.opt.wildignore = ".git,.hg,.svn,*.pyc,*.o,*.out,*.jpg,*.jpeg,*.png,*.gif,*.zip,**/tmp/**,*.DS_Store,**/node_modules/**,**/bower_modules/**"
vim.opt.history = 2000
vim.opt.shada = "!,'300,<50,@100,s10,h"
vim.opt.smarttab = true
vim.opt.shiftround = true
vim.opt.timeout = true
vim.opt.ttimeout = true
-- You will feel delay when you input <Space> at lazygit interface if you set it a positive value like 300(ms)
vim.opt.timeoutlen = 0
vim.opt.ttimeoutlen = 0
vim.opt.updatetime = 100
vim.opt.redrawtime = 1500
vim.opt.infercase = true
vim.opt.incsearch = true
vim.opt.wrapscan = true
vim.opt.complete = ".,w,b,k"
vim.opt.inccommand = "nosplit"
vim.opt.grepformat = "%f:%l:%c:%m"
vim.opt.grepprg = "rg --hidden --vimgrep --smart-case --"
vim.opt.breakat = [[\ \	;:,!?]]
vim.opt.startofline = false
vim.opt.whichwrap = "h,l,<,>,[,],~"
vim.opt.switchbuf = "useopen"
vim.opt.backspace = "indent,eol,start"
vim.opt.diffopt = "filler,iwhite,internal,algorithm:patience"
vim.opt.jumpoptions = "stack"
vim.opt.shortmess = "aoOTIcF"
vim.opt.foldlevelstart = 99
vim.opt.ruler = true
vim.opt.cursorcolumn = true
vim.opt.list = true
vim.opt.winwidth = 30
vim.opt.winminwidth = 10
vim.opt.helpheight = 12
vim.opt.previewheight = 12
vim.opt.showcmd = true
vim.opt.cmdwinheight = 5
vim.opt.equalalways = false
vim.opt.laststatus = 2
vim.opt.display = "lastline"
vim.opt.showbreak = "↳  "
vim.opt.listchars = "tab:»·,nbsp:+,trail:·,extends:→,precedes:←"
vim.opt.autoread = true
vim.opt.autowrite = true

vim.opt.synmaxcol = 2500
vim.opt.formatoptions = "1jcroql"
vim.opt.autoindent = true
--vim.opt.tabstop = 4
--vim.opt.shiftwidth = 4
--vim.opt.softtabstop = 4
vim.opt.breakindentopt = "shift:2,min:20"
vim.opt.linebreak = true
vim.opt.foldenable = true
vim.opt.concealcursor = "niv"


vim.cmd([[
let fcitx5state=system("fcitx5-remote")
autocmd InsertLeave * :silent let fcitx5state=system("fcitx5-remote")[0] | silent !fcitx5-remote -c
autocmd InsertEnter * :silent if fcitx5state == 2 | call system("fcitx5-remote -o") | endif
]])
