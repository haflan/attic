function spew()
    local output = vim.fn.system(vim.api.nvim_get_current_line())
    vim.api.nvim_put({ output }, "l", true, false)
end

vim.api.nvim_set_keymap("n", "<leader>a", ":lua spew()<CR>", { noremap = true })
