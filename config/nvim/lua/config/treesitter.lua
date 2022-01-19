require("nvim-treesitter.configs").setup({
    ensure_installed = "maintained", -- one of "all", "maintained" (parsers with maintainers), or a list of languages
    -- sync_install = true, -- install languages synchronously (only applied to `ensure_installed`)
    -- ensure_installed = {"python", "cpp", "lua", "vim"},
    ignore_install = {}, -- List of parsers to ignore installing
    highlight = {
        enable = true, -- false will disable the whole extension
        disable = {} -- list of language that will be disabled
    },
    indent = {enable = true}
})
