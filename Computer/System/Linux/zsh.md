# oh-my-zsh 

# plugin
- in .zshrc, plugins=(git zsh-syntax-highlighting ...)

- git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting

- git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions

- in .zshrc: bindkey ';' autosuggest-accept 修改auto的 右方向键为;

# themes

- sudo pacman -S powerline-fonts

- git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ${ZSH_CUSTOM:-$HOME/.oh-my-zsh/custom}/themes/powerlevel10k
- Set ZSH_THEME="powerlevel10k/powerlevel10k" in ~/.zshrc.
- p10k configure

        git clone https://github.com/ryanoasis/nerd-fonts.git --depth 1
        cd nerd-fonts
        ./install.sh
        cd ..
        rm -rf nerd-fonts

