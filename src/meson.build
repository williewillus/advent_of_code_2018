project('aoc', 'cpp', default_options: ['cpp_std=c++17'] )
extra_deps = {} # "dayN" -> list of extra source files that day needs besides itself and util.cpp
foreach i : [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18] # Add more as more days complete
	day = 'day' + i.to_string()
	executable(day, sources: [day + '.cpp', 'util.cpp'] + extra_deps.get(day, []))
endforeach

