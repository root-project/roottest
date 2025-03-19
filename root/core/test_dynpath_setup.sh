#! /bin/bash

set -e

mkdir -p rootlibpath_test
cd rootlibpath_test

root="$1"
libdir="$("$root" -b -x -q -l -e 'cout << TROOT::GetLibDir();' | tail -1)"

set_rootrc() {
	if [ -n "$1" ]
	then
		echo "Unix.*.Root.DynamicPath $1" >.rootrc
	else
		echo "# Unix.*.Root.DynamicPath" >.rootrc
	fi
}

get_dynpath() {
	cur_dynpath="$("$root" -b -x -q -l -e 'cout << gSystem->GetDynamicPath();' | tail -1)"
}

check_begin() {
	case "$cur_dynpath:" in
		"$1":*)
			;;
		*)
			echo "dynamic path: ${cur_dynpath}"
			echo "dynamic path should start with: $1"
			exit 1
			;;
	esac
}
check_mid() {
	case ":$cur_dynpath:" in
		*:"$1":*)
			;;
		*)
			echo "dynamic path: ${cur_dynpath}"
			echo "dynamic path should contain: $1"
			exit 1
			;;
	esac
}
check_begin_and_mid() {
	check_begin "$1"
	check_mid "$2"
}


mkdir -p rootlibpath
rootlibpath="$PWD/rootlibpath"
mkdir -p rootrcpath
rootrcpath="$PWD/rootrcpath"
mkdir -p ldpath
ldpath="$PWD/ldpath"


export ROOT_LIBRARY_PATH="$rootlibpath"
set_rootrc "$rootrcpath"
get_dynpath
check_begin_and_mid "$ROOT_LIBRARY_PATH" "$rootrcpath:$libdir"

export ROOT_LIBRARY_PATH="$rootlibpath"
set_rootrc "$libdir:$rootrcpath"
get_dynpath
check_begin_and_mid "$ROOT_LIBRARY_PATH" "$libdir:$rootrcpath"

export ROOT_LIBRARY_PATH="$rootlibpath"
set_rootrc ""
get_dynpath
check_begin_and_mid "$ROOT_LIBRARY_PATH" ".:$libdir"

unset ROOT_LIBRARY_PATH
set_rootrc ""
get_dynpath
check_mid ".:$libdir"


# #########################
# with LD_LIBRARY_PATH & Co

if [ -n "$LD_LIBRARY_PATH" ]
then
	LD_LIBRARY_PATH="$ldpath:$LD_LIBRARY_PATH"
else
	export LD_LIBRARY_PATH="$ldpath"
fi
if [ -n "$DYLD_LIBRARY_PATH" ]
then
	DYLD_LIBRARY_PATH="$ldpath:$DYLD_LIBRARY_PATH"
else
	export DYLD_LIBRARY_PATH="$ldpath"
fi
if [ -n "$LIBPATH" ]
then
	LIBPATH="$ldpath:$LIBPATH"
else
	export LIBPATH="$ldpath"
fi


export ROOT_LIBRARY_PATH="$rootlibpath"
set_rootrc "$rootrcpath"
get_dynpath
check_begin_and_mid "$ROOT_LIBRARY_PATH:$ldpath" "$rootrcpath:$libdir"

export ROOT_LIBRARY_PATH="$rootlibpath"
set_rootrc "$libdir:$rootrcpath"
get_dynpath
check_begin_and_mid "$ROOT_LIBRARY_PATH:$ldpath" "$libdir:$rootrcpath"

export ROOT_LIBRARY_PATH="$rootlibpath"
set_rootrc ""
get_dynpath
check_begin_and_mid "$ROOT_LIBRARY_PATH:$ldpath" ".:$libdir"

unset ROOT_LIBRARY_PATH
set_rootrc ""
get_dynpath
check_begin_and_mid "$ldpath" ".:$libdir"
