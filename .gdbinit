tui enable

source scripts/gdb_pretty_printers.py
source breakpoints


define hookpost-delete
save breakpoints breakpoints
echo "\nsaving breakpoints\n"
end

define hookpost-condition
save breakpoints breakpoints
echo "\nsaving breakpoints\n"
end

define hookpost-disable
save breakpoints breakpoints
echo "\nsaving breakpoints\n"
end

define hookpost-break
save breakpoints breakpoints
echo "\nsaving breakpoints\n"
end


start
continue
