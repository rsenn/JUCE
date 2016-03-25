set -x
IFS="
"

sed -i '/^\t\s*@/ { /@echo/! s|@|$(Q)| ;  /@echo/ s|@echo|@$(ECHO)| ; }; /(ECHO)/ s,^,#,;' \
	"$@"

uniq <<<"${*%/*}"

