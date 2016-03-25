add-and-resave () 
{ 
    grep --colour=auto -E "/$X\$" -v <<< "$*" | for_each -f -x '(Introjucer --fix-broken-include-paths "${1%/*}"; Introjucer --add-exporter "MinGW32 Makefile" "$1" ; Introjucer --add-exporter "Linux Makefile";  Introjucer --resave "$1") 2>&1 | addprefix "${1%/*}: "'
}
