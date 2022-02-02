project('drez', 'c')
src = ['src/board.c', 'src/piece.c', 'src/main.c', 'src/fmt_parser.c']
cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : true)
curses_dep = cc.find_library('curses', required : true)
executable('drez', src, dependencies : [m_dep , curses_dep], install : false)