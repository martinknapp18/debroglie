
set print pretty on
set mem inaccessible-by-default off

define reload
  load
  run
end

define reset
  monitor reset halt
end

file BUILD/DeBroglie.elf
target remote localhost:3333
reset
