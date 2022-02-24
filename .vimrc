set ruler
set mouse=a
set ts=4
set cindent
syntax on
syntax enable
set hlsearch
set incsearch
set nu
set shiftwidth=4

colorscheme uwu

" cursor options 

hi MatchParen cterm=bold ctermbg=red ctermfg=magenta
let &t_ti.="\e[2 q"
let &t_SI.="\e[2 q"
let &t_EI.="\e[2 q"
let &t_te.="\e[2 q"

let &t_SI = "\e[2 q"


if &term =~ "xterm\\|rxvt"
   " use an orange cursor in insert mode
   silent! let &t_SI = "\e[2 q\e]12;red\x7"
   " use a red cursor in replace mode
   silent! let &t_SR = "\e[2 q\e]12;red\x7"
   " use a green cursor otherwise
   silent! let &t_EI = "\e[2 q\e]12;red\x7"
   silent !echo -ne "\033]12;green\007"
endif

" ~~ cursor


" Vundle
set nocompatible              " be iMproved, required
filetype off                  " required
 
" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()

Plugin 'VundleVim/Vundle.vim'
Plugin 'The-NERD-Tree'
Plugin 'Raimondi/delimitMate'

call vundle#end()            " required
filetype plugin indent on    " required


let delimitMate_expand_cr=1

nmap nerd :NERDTreeToggle<CR>
