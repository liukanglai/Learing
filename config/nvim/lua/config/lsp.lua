local api = vim.api
local lsp = vim.lsp

local M = {}

function M.show_line_diagnostics()
    local opts = {
        focusable = false,
        close_events = {"BufLeave", "CursorMoved", "InsertEnter", "FocusLost"},
        border = 'rounded',
        source = 'always', -- show source in diagnostic popup window
        prefix = ' '
    }
    vim.diagnostic.open_float(nil, opts)
end

local custom_attach = function(client, bufnr)
    local function buf_set_keymap(...) api.nvim_buf_set_keymap(bufnr, ...) end

    -- Mappings.
    local opts = {noremap = true, silent = true}

    buf_set_keymap('n', '<space>ld', '<cmd>lua vim.lsp.buf.declaration()<CR>',
                   opts)
    buf_set_keymap('n', '<space>lf', '<cmd>lua vim.lsp.buf.definition()<CR>',
                   opts)
    buf_set_keymap('n', 'K', '<cmd>lua vim.lsp.buf.hover()<CR>', opts)
    buf_set_keymap('n', '<space>li',
                   '<cmd>lua vim.lsp.buf.implementation()<CR>', opts)
    buf_set_keymap('n', '<space>lh',
                   '<cmd>lua vim.lsp.buf.signature_help()<CR>', opts)
    buf_set_keymap('n', '<space>wa',
                   '<cmd>lua vim.lsp.buf.add_workspace_folder()<CR>', opts)
    buf_set_keymap('n', '<space>wr',
                   '<cmd>lua vim.lsp.buf.remove_workspace_folder()<CR>', opts)
    buf_set_keymap('n', '<space>wl',
                   '<cmd>lua print(vim.inspect(vim.lsp.buf.list_workspace_folders()))<CR>',
                   opts)
    buf_set_keymap('n', '<space>D',
                   '<cmd>lua vim.lsp.buf.type_definition()<CR>', opts)
    buf_set_keymap('n', '<space>lr', '<cmd>lua vim.lsp.buf.rename()<CR>', opts)
    buf_set_keymap('n', '<space>lc', '<cmd>lua vim.lsp.buf.code_action()<CR>',
                   opts)
    buf_set_keymap('n', '<space>gr', '<cmd>lua vim.lsp.buf.references()<CR>',
                   opts)
    buf_set_keymap('n', '<space>le',
                   '<cmd>lua vim.lsp.diagnostic.show_line_diagnostics()<CR>',
                   opts)
    buf_set_keymap('n', '<space>lm',
                   '<cmd>lua vim.lsp.diagnostic.goto_prev()<CR>', opts)
    buf_set_keymap('n', '<space>ln',
                   '<cmd>lua vim.lsp.diagnostic.goto_next()<CR>', opts)
    buf_set_keymap('n', '<space>q',
                   '<cmd>lua vim.lsp.diagnostic.set_loclist()<CR>', opts)
    -- buf_set_keymap('n', '<space>f', '<cmd>lua vim.lsp.buf.formatting()<CR>',
    -- opts)

    vim.cmd([[
    autocmd CursorHold <buffer> lua require('config.lsp').show_line_diagnostics()
  ]])

    -- Set some key bindings conditional on server capabilities
    -- if client.resolved_capabilities.document_formatting then
    -- buf_set_keymap("n", "<space>f",
    -- "<cmd>lua vim.lsp.buf.formatting_sync()<CR>", opts)
    -- end
    -- if client.resolved_capabilities.document_range_formatting then
    -- buf_set_keymap("x", "<space>f",
    -- "<cmd>lua vim.lsp.buf.range_formatting()<CR><ESC>", opts)
    -- end

    -- The blow command will highlight the current variable and its usages in the buffer.
    if client.resolved_capabilities.document_highlight then
        vim.cmd([[
      hi link LspReferenceRead Visual
      hi link LspReferenceText Visual
      hi link LspReferenceWrite Visual
      augroup lsp_document_highlight
        autocmd! * <buffer>
        autocmd CursorHold <buffer> lua vim.lsp.buf.document_highlight()
        autocmd CursorMoved <buffer> lua vim.lsp.buf.clear_references()
      augroup END
    ]])
    end

    if vim.g.logging_level == 'debug' then
        local msg = string.format("Language server %s started!", client.name)
        vim.notify(msg, 'info', {title = 'Nvim-config'})
    end
end

local capabilities = require('cmp_nvim_lsp').update_capabilities(lsp.protocol
                                                                     .make_client_capabilities())
capabilities.textDocument.completion.completionItem.snippetSupport = true

local lspconfig = require("lspconfig")

lspconfig.pylsp.setup({
    on_attach = custom_attach,
    settings = {
        pylsp = {
            plugins = {
                pylint = {enabled = true, executable = "pylint"},
                pyflakes = {enabled = false},
                pycodestyle = {enabled = false},
                jedi_completion = {fuzzy = true},
                pyls_isort = {enabled = true},
                pylsp_mypy = {enabled = true}
            }
        }
    },
    flags = {debounce_text_changes = 200},
    capabilities = capabilities
})

-- lspconfig.pyright.setup{
--   on_attach = custom_attach,
--   capabilities = capabilities
-- }

-- lspconfig.clangd.setup({
-- on_attach = custom_attach,
-- capabilities = capabilities,
-- filetypes = {"c", "cpp", "cc"},
-- flags = {debounce_text_changes = 500}
-- })

require'lspconfig'.ccls.setup {}
lspconfig.ccls.setup {
    init_options = { -- let ccls search co*.json in build, because it will only search in root...
        compilationDatabaseDirectory = "build",
        index = {threads = 0},
        clang = {excludeArgs = {"-frounding-math"}}
    },
    on_attach = custom_attach,
    capabilities = capabilities,
    filetypes = {"c", "cpp", "cc"},
    flags = {debounce_text_changes = 500}
}

-- set up vim-language-server
-- lspconfig.vimls.setup({
-- on_attach = custom_attach,
-- flags = {debounce_text_changes = 500},
-- capabilities = capabilities
-- })

local sumneko_binary_path = vim.fn.exepath("lua-language-server")
if vim.g.is_mac or vim.g.is_linux and sumneko_binary_path ~= "" then
    local sumneko_root_path = vim.fn.fnamemodify(sumneko_binary_path, ":h:h:h")

    local runtime_path = vim.split(package.path, ";")
    table.insert(runtime_path, "lua/?.lua")
    table.insert(runtime_path, "lua/?/init.lua")

    require("lspconfig").sumneko_lua.setup({
        on_attach = custom_attach,
        cmd = {sumneko_binary_path, "-E", sumneko_root_path .. "/main.lua"},
        settings = {
            Lua = {
                runtime = {
                    -- Tell the language server which version of Lua you're using (most likely LuaJIT in the case of Neovim)
                    version = "LuaJIT",
                    -- Setup your lua path
                    path = runtime_path
                },
                diagnostics = {
                    -- Get the language server to recognize the `vim` global
                    globals = {"vim"}
                },
                workspace = {
                    -- Make the server aware of Neovim runtime files
                    library = api.nvim_get_runtime_file("", true)
                },
                -- Do not send telemetry data containing a randomized but unique identifier
                telemetry = {enable = false}
            }
        },
        capabilities = capabilities
    })
end

-- Change diagnostic signs.
vim.fn.sign_define("DiagnosticSignError",
                   {text = "●", texthl = "DiagnosticSignError"})
vim.fn.sign_define("DiagnosticSignWarn",
                   {text = "", texthl = "DiagnosticSignWarn"})
vim.fn.sign_define("DiagnosticSignInformation",
                   {text = "", texthl = "DiagnosticSignInfo"})
vim.fn.sign_define("DiagnosticSignHint",
                   {text = "", texthl = "DiagnosticSignHint"})

-- sign = '✗','   ''●' !

-- global config for diagnostic
vim.diagnostic.config({
    underline = true,
    virtual_text = false,
    signs = true,
    severity_sort = true
})

-- lsp.handlers["textDocument/publishDiagnostics"] = lsp.with(lsp.diagnostic.on_publish_diagnostics, {
--   underline = false,
--   virtual_text = false,
--   signs = true,
--   update_in_insert = false,
-- })

-- Change border of documentation hover window, See https://github.com/neovim/neovim/pull/13998.
lsp.handlers["textDocument/hover"] = lsp.with(vim.lsp.handlers.hover,
                                              {border = "rounded"})

local lsp_installer = require("nvim-lsp-installer")

-- Register a handler that will be called for all installed servers.
-- Alternatively, you may also register handlers on specific server instances instead (see example below).
lsp_installer.on_server_ready(function(server)
    local opts = {on_attach = custom_attach, capabilities = capabilities}

    -- (optional) Customize the options passed to the server
    -- if server.name == "tsserver" then
    --     opts.root_dir = function() ... end
    -- end

    -- This setup() function is exactly the same as lspconfig's setup function.
    -- Refer to https://github.com/neovim/nvim-lspconfig/blob/master/doc/server_configurations.md
    server:setup(opts)
end)

return M

-- require("lsp-colors").setup({
-- Error = "#db4b4b",
-- Warning = "#e0af68",
-- Information = "#0db9d7",
-- Hint = "#10B981"
-- })