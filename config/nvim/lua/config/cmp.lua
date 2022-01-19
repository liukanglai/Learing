-- Setup nvim-cmp.
local cmp = require 'cmp'
local lspkind = require 'lspkind'

cmp.setup({
    mapping = {
        ['<Tab>'] = function(fallback)
            if cmp.visible() then
                cmp.select_next_item()
            else
                fallback()
            end
        end,
        ['<C-b>'] = function(fallback)
            if cmp.visible() then
                cmp.select_prev_item()
            else
                fallback()
            end
        end,
        -- ['<Esc>'] = cmp.mapping.close(),
        ['<CR>'] = cmp.mapping.confirm({select = true}),
        ['<C-d>'] = cmp.mapping.scroll_docs(-4),
        ['<C-f>'] = cmp.mapping.scroll_docs(4),
        ['<C-y>'] = cmp.config.disable -- Specify `cmp.config.disable` if you want to remove the default `<C-y>` mapping.
    },
    sources = {
        {name = 'nvim_lsp'}, -- For nvim-lsp
        -- { name = 'ultisnips' }, -- For ultisnips user.
        {name = 'nvim_lua'}, -- for nvim lua function
        {name = 'path'}, -- for path completion
        {name = 'buffer', keyword_length = 4}, -- for buffer word completion
        {name = 'emoji', insert = true} -- emoji completion
    },
    completion = {keyword_length = 1, completeopt = "menu,noselect"},
    experimental = {ghost_text = false},
    formatting = {
        format = lspkind.cmp_format({
            with_text = false,
            menu = {
                nvim_lsp = "[LSP]",
                -- ultisnips = "[US]",
                nvim_lua = "[Lua]",
                path = "[Path]",
                buffer = "[Buffer]",
                emoji = "[Emoji]"
            }
        })
    }
})

vim.cmd("hi link CmpItemMenu Comment")
