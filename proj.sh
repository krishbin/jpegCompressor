#! /bin/zsh

case $1 in
	(configure)
		cmake -S . -B ./out/build/
		if [[ -a ./compile_commands.json ]];then
			rm compile_commands.json
			echo "deleted existing compile_commands.json"
		fi
		cp ./out/build/compile_commands.json ./compile_commands.json
		echo "copied compile_commands.json"
		;;
	(build)
		cmake --build ./out/build/
		;;
	(run)
		cd out/build
		./jpegCompressor
		;;
	(*)
		echo "select configure build shader or run"
		;;
esac
