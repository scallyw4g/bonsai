#! /bin/bash

META_SLOC=$(find generated   -name \*.cpp -print -o -name \*.h -print | xargs cat | wc -l)
SHDR_SLOC=$(find shaders                                              | xargs cat | wc -l)
EXMP_SLOC=$(find examples    -name \*.cpp -print -o -name \*.h -print | xargs cat | wc -l)
PRNT_SLOC=$(find debug_print -name \*.cpp -print -o -name \*.h -print | xargs cat | wc -l)
DEPS_SLOC=$(find include     -name \*.cpp -print -o -name \*.h -print | xargs cat | wc -l)
POOF_SLOC=$(find src         -name \*.cpp -print -o -name \*.h -print | xargs cat | wc -l)

echo ""
echo " Hand written   $POOF_SLOC"
echo " Shaders        $SHDR_SLOC"
echo " Examples       $EXMP_SLOC"
echo " Meta output    $META_SLOC"
echo " Debug print    $PRNT_SLOC"
echo " Deps           $DEPS_SLOC"
echo "--------------------------"
echo " Total          $(( POOF_SLOC + SHDR_SLOC + EXMP_SLOC + META_SLOC + DEPS_SLOC + PRNT_SLOC )) sloc"
echo ""
