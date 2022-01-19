local function spell()
    if vim.o.spell then return string.format("[SPELL]") end

    return ""
end

local function trailing_space()
    -- Get the positions of trailing whitespaces from plugin 'jdhao/whitespace.nvim'.
    local trailing_space_pos = vim.b.trailing_whitespace_pos

    local msg = ""
    if #trailing_space_pos > 0 then
        -- Note that lua index is 1-based, not zero based!!!
        local line = trailing_space_pos[1][1]
        msg = string.format("[%d]trailing", line)
    end

    return msg
end

local function mixed_indent()
    local space_pat = [[\v^ +]]
    local tab_pat = [[\v^\t+]]
    local space_indent = vim.fn.search(space_pat, 'nwc')
    local tab_indent = vim.fn.search(tab_pat, 'nwc')
    local mixed = (space_indent > 0 and tab_indent > 0)
    local mixed_same_line
    if not mixed then
        mixed_same_line = vim.fn.search([[\v^(\t+ | +\t)]], 'nwc')
        mixed = mixed_same_line > 0
    end
    if not mixed then return '' end
    if mixed_same_line ~= nil and mixed_same_line > 0 then
        return 'MI:' .. mixed_same_line
    end
    local space_indent_cnt = vim.fn.searchcount({
        pattern = space_pat,
        max_count = 1e3
    }).total
    local tab_indent_cnt = vim.fn.searchcount({
        pattern = tab_pat,
        max_count = 1e3
    }).total
    if space_indent_cnt > tab_indent_cnt then
        return 'MI:' .. tab_indent
    else
        return 'MI:' .. space_indent
    end
end

require("lualine").setup({
    options = {
        icons_enabled = true,
        theme = 'gruvbox-material',
        -- component_separators = {left = "", right = ""},
        -- section_separators = {left = "", right = ""},
        section_separators = "",
        component_separators = "",
        disabled_filetypes = {},
        always_divide_middle = true
    },
    sections = {
        lualine_a = {"mode"},
        lualine_b = {"branch", "diff"},
        lualine_c = {
            {spell, color = {fg = 'black', bg = '#a7c080'}}, "filename"
        },
        lualine_x = {
            "encoding",
            {"fileformat", symbols = {unix = "unix", dos = "win", mac = "mac"}},
            "filetype"
        },
        lualine_y = {"progress"},
        lualine_z = {
            "location", {"diagnostics", sources = {"nvim_lsp"}},
            {trailing_space, color = "WarningMsg"},
            {mixed_indent, color = "WarningMsg"}
        }
    },
    inactive_sections = {
        lualine_a = {},
        lualine_b = {},
        lualine_c = {"filename"},
        lualine_x = {"location"},
        lualine_y = {},
        lualine_z = {}
    },
    tabline = {},
    extensions = {'quickfix', 'fugitive'}
})

--[[require("bufferline").setup({]]
--[[options = {]]
--[[numbers = "buffer_id",]]
--[[close_command = "bdelete! %d",]]
--[[right_mouse_command = nil,]]
--[[left_mouse_command = "buffer %d",]]
--[[middle_mouse_command = nil,]]
--[[indicator_icon = "▎",]]
--[[buffer_close_icon = "",]]
--[[modified_icon = "●",]]
--[[close_icon = "",]]
--[[left_trunc_marker = "",]]
--[[right_trunc_marker = "",]]
--[[max_name_length = 18,]]
--[[max_prefix_length = 15,]]
--[[tab_size = 10,]]
--[[diagnostics = false,]]
--[[custom_filter = function(bufnr)]]
--[[-- if the result is false, this buffer will be shown, otherwise, this]]
--[[-- buffer will be hidden.]]

--[[-- filter out filetypes you don't want to see]]
--[[local exclude_ft = {"qf", "fugitive", "git"}]]
--[[local cur_ft = vim.bo[bufnr].filetype]]
--[[local should_filter = vim.tbl_contains(exclude_ft, cur_ft)]]

--[[if should_filter then return false end]]

--[[return true]]
--[[end,]]
--[[show_buffer_icons = true,]]
--[[show_buffer_close_icons = true,]]
--[[show_close_icon = true,]]
--[[show_tab_indicators = true,]]
--[[persist_buffer_sort = true, -- whether or not custom sorted buffers should persist]]
--[[separator_style = "bar",]]
--[[enforce_regular_tabs = true,]]
--[[always_show_bufferline = true,]]
--[[sort_by = "id"]]
--[[}]]
--[[})]]

local status, bufferline = pcall(require, "bufferline")
if (not status) then return end

vim.opt.termguicolors = true
bufferline.setup {
    options = {
        -- numbers = "ordinal" | "buffer_id" | "both" | function({ ordinal, id, lower, raise }): string,
        -- numbers = "both",
        --- @deprecated, please specify numbers as a function to customize the styling
        -- number_style = "superscript" | "subscript" | "" | { "none", "subscript" }, -- buffer_id at index 1, ordinal at index 2
        -- number_style = "none",
        close_command = "bdelete! %d", -- can be a string | function, see "Mouse actions"
        right_mouse_command = "bdelete! %d", -- can be a string | function, see "Mouse actions"
        left_mouse_command = "buffer %d", -- can be a string | function, see "Mouse actions"
        middle_mouse_command = nil, -- can be a string | function, see "Mouse actions"
        -- NOTE: this plugin is designed with this icon in mind,
        -- and so changing this is NOT recommended, this is intended
        -- as an escape hatch for people who cannot bear it for whatever reason
        indicator_icon = "",
        buffer_close_icon = "",
        modified_icon = "●",
        close_icon = "",
        left_trunc_marker = "",
        right_trunc_marker = "",
        --- name_formatter can be used to change the buffer's label in the bufferline.
        --- Please note some names can/will break the
        --- bufferline so use this at your discretion knowing that it has
        --- some limitations that will *NOT* be fixed.
        name_formatter = function(buf) -- buf contains a "name", "path" and "bufnr"
            -- remove extension from markdown files for example
            if buf.name:match("%.md") then
                return vim.fn.fnamemodify(buf.name, ":t:r")
            end
        end,
        max_name_length = 15,
        max_prefix_length = 12, -- prefix used when a buffer is de-duplicated
        tab_size = 15,
        -- diagnostics = false | "nvim_lsp" | "coc",
        diagnostics = "nvim_lsp",
        diagnostics_update_in_insert = false,
        --[[diagnostics_indicator = function(count, level, diagnostics_dict, context)
      return "(" .. count .. ")"
    end,]]
        -- rest of config ...

        --- count is an integer representing total count of errors
        --- level is a string "error" | "warning"
        --- diagnostics_dict is a dictionary from error level ("error", "warning" or "info")to number of errors for each level.
        --- this should return a string
        --- Don't get too fancy as this function will be executed a lot
        diagnostics_indicator = function(count, level, diagnostics_dict, context)
            local icon = level:match("error") and " " or " "
            return " " .. icon .. count
        end,
        -- NOTE: this will be called a lot so don't do any heavy processing here
        custom_filter = function(buf_number)
            -- 如果是defx则隐藏
            local finded, _ = string.find(vim.bo[buf_number].filetype, "defx")
            if finded ~= nil then return false end
            return true
        end,
        -- offsets = {{filetype = "NvimTree", text = "File Explorer" | function , text_align = "left" | "center" | "right"}},
        -- show_buffer_icons = true | false, -- disable filetype icons for buffers
        show_buffer_icons = true, -- disable filetype icons for buffers
        -- show_buffer_close_icons = true | false,
        show_buffer_close_icons = false,
        -- show_close_icon = true | false,
        show_close_icon = false,
        -- show_tab_indicators = true | false,
        show_tab_indicators = true,
        persist_buffer_sort = true, -- whether or not custom sorted buffers should persist
        -- can also be a table containing 2 custom separators
        -- [focused and unfocused]. eg: { '|', '|' }
        -- separator_style = "slant" | "thick" | "thin" | {"any", "any"},
        separator_style = "thin",
        -- enforce_regular_tabs = false | true,
        enforce_regular_tabs = false,
        -- always_show_bufferline = true | false,
        always_show_bufferline = true
        --[[sort_by = "id" | "extension" | "relative_directory" | "directory" | "tabs" | function(buffer_a, buffer_b)
        -- add custom logic
        return buffer_a.modified > buffer_b.modified
      end]]
    }
}
