#! /bin/bash

    META_SLOC=$(find generated   -name \*.cpp -print -o -name \*.h -print | xargs cat | wc -l)
    EXMP_SLOC=$(find examples    -name \*.cpp -print -o -name \*.h -print | xargs cat | wc -l)
    DEPS_SLOC=$(find external    -name \*.cpp -print -o -name \*.h -print | xargs cat | wc -l)
    POOF_SLOC=$(find src         -name \*.cpp -print -o -name \*.h -print | xargs cat | wc -l)
    SHDR_SLOC=$(find shaders     -type f                                  | xargs cat | wc -l)
STD_SHDR_SLOC=$(find external/bonsai_stdlib/shaders     -type f           | xargs cat | wc -l)

    # META_SLOC=$(find generated   -name \*.cpp -print -o -name \*.h -print | xargs wc -c | cut -d" " -f 1)
    # EXMP_SLOC=$(find examples    -name \*.cpp -print -o -name \*.h -print | xargs wc -c | cut -d" " -f 1)
    # DEPS_SLOC=$(find external    -name \*.cpp -print -o -name \*.h -print | xargs wc -c | cut -d" " -f 1)
    # POOF_SLOC=$(find src         -name \*.cpp -print -o -name \*.h -print | xargs wc -c | cut -d" " -f 1)
    # SHDR_SLOC=$(find shaders     -type f                                  | xargs wc -c | cut -d" " -f 1)
# STD_SHDR_SLOC=$(find external/bonsai_stdlib/shaders     -type f           | xargs wc -c | cut -d" " -f 1)


echo ""
echo " Hand written   $POOF_SLOC"
echo " Deps           $DEPS_SLOC"
echo " Poof'd code    $META_SLOC"
echo " Examples       $EXMP_SLOC"
echo " Shaders        $SHDR_SLOC"
echo " stdlib/shaders $STD_SHDR_SLOC"
echo "--------------------------"
echo " Total          $(( POOF_SLOC + SHDR_SLOC + EXMP_SLOC + META_SLOC + DEPS_SLOC + PRNT_SLOC + STD_SHDR_SLOC )) sloc"
echo ""
